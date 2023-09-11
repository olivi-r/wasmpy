#include "nativelib.hpp"

std::vector<void *> registeredPages = {};
std::vector<size_t> registeredPageSizes = {}, standaloneFuncs = {};
std::vector<int> globalMut = {};
bytes globalTable = {}, globalTypes = {}, localTypes = {};
std::vector<uint32_t> localOffsets = {};
uint64_t errorPageAddr, globalTableAddr;
uint32_t page_size;

uint8_t blankPage[65536] = {0};

typedef struct
{
    void *address;
    int32_t currentPage;
    uint32_t numPages;
    uint32_t maxPages;
    int fd;
#ifdef PLATFORM_WINDOWS
    HANDLE hFile;
    HANDLE hMap;
#endif
} memory_t;

std::vector<memory_t *> memories = {};

bytes errorPage = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, // successful result struct
    1,                         // unreachable
    2,                         // division by zero
    3,                         // division overflow
    4,                         // integer modulo by zero
    5,                         // unrepresentable truncation result
    0, 0, 0, 0, 0, 0, 0, 0,    // padding
};

bytes text = {};

template <typename T>
std::vector<T> concat(std::vector<T> v0, std::vector<std::vector<T>> vn)
{
    for (size_t i = 0; i < vn.size(); i++)
        v0.insert(v0.end(), vn.at(i).begin(), vn.at(i).end());

    return v0;
}

void *writePage(bytes data)
{
    void *buf;
    size_t size = page_size * (1 + data.size() / page_size);
#ifdef PLATFORM_LINUX
    buf = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
#elif PLATFORM_WINDOWS
    buf = VirtualAlloc(nullptr, size, MEM_COMMIT, PAGE_READWRITE);
#endif
    memcpy(buf, data.data(), data.size());
    registeredPages.push_back(buf);
    registeredPageSizes.push_back(size);
    return buf;
}

void unmapMemory(memory_t *mem)
{
    if (mem->currentPage != -1)
    {
#ifdef PLATFORM_LINUX
        munmap(mem->address, 65536);
#elif PLATFORM_WINDOWS
        UnmapViewOfFile(mem->address);
#endif
        mem->currentPage = -1;
    }
}

void freePages()
{
    for (size_t i = 0; i < memories.size(); i++)
    {
        unmapMemory(memories.at(i));
#ifdef PLATFORM_WINDOWS
        CloseHandle(memories.at(i)->hMap);
        CloseHandle(memories.at(i)->hFile);
#endif
        close(memories.at(i)->fd);
    }

    for (size_t i = 0; i < registeredPages.size(); i++)
    {
#ifdef PLATFORM_LINUX
        munmap(registeredPages.at(i), registeredPageSizes.at(i));
#elif PLATFORM_WINDOWS
        VirtualFree(registeredPages.at(i), 0, MEM_RELEASE);
#endif
    }
}

size_t writeFunction(bytes code)
{
    size_t offset = text.size();
    text = concat(text, {code});
    return offset;
}

static PyObject *writeFunctionPage(PyObject *self, PyObject *args)
{
    void *buf = writePage(text);
#ifdef PLATFORM_LINUX
    mprotect(buf, text.size(), PROT_READ | PROT_EXEC);
#elif PLATFORM_WINDOWS
    DWORD dummy;
    VirtualProtect(buf, text.size(), PAGE_EXECUTE_READ, &dummy);
#endif
    text = {}; // flush function code
    return PyLong_FromVoidPtr(buf);
}

int32_t growMemory(memory_t *mem, uint32_t n)
{
    uint32_t size = mem->numPages;
    if (size + n > mem->maxPages)
        return -1;

    lseek(mem->fd, 0, SEEK_END);
    for (uint32_t i = 0; i < n; i++)
        write(mem->fd, blankPage, 65536);

    mem->numPages = size + n;
    return size;
}

typedef struct operation
{
    uint8_t opcode;
    std::vector<wchar_t *> arguments;
    std::vector<wchar_t *> results;
    long consumes;
    uint32_t data;
    bytes code;
} operation;

static PyObject *createFunction(PyObject *self, PyObject *args)
{
    char ret, standalone;
    const char *codebuf, *argbuf, *localbuf;
    Py_ssize_t codelen, arglen, locallen;
    PyObject *consumes, *signatures;
    if (!PyArg_ParseTuple(args, "by#y#y#OOb", &ret, &codebuf, &codelen, &argbuf, &arglen, &localbuf, &locallen, &consumes, &signatures, &standalone))
        return NULL;

    // register parameters
    bytes loadLocals = regParam(argbuf, arglen);

    int32_t localOff = 0;

    // register local variables
    size_t localSize = 0;
    for (Py_ssize_t i = 0; i < locallen; i++)
    {
        localTypes.push_back(localbuf[i]);

        if (localbuf[i] == 0x7F || localbuf[i] == 0x7D)
        {
            localSize += 4;
            localOff -= 4;
        }
        else
        {
            localSize += 8;
            localOff -= 8;
        }
        localOffsets.push_back((uint32_t)localOff);
    }

    // allocate space for local variables
    for (size_t i = 0; i < localSize / 8; i++)
        loadLocals = concat(loadLocals, {pushq0});

    if (localSize % 8 == 4)
        loadLocals = concat(loadLocals, {pushl0});

    bytes code(codebuf, codelen + codebuf);

    std::vector<operation *> stack = {};
    operation *current_operation;

    std::vector<wchar_t *> validateStack = {};
    for (size_t i = 0; i < code.size(); i++)
    {
        uint8_t op = code.at(i);

        std::vector<wchar_t *> operandVector = {};
        std::vector<wchar_t *> resultVector = {};

        current_operation = new operation();

        current_operation->opcode = op;
        current_operation->data = 0;

        // local instruction validation
        if (op == 0x20 || op == 0x21 || op == 0x22)
        {
            uint32_t localidx = code.at(i + 4) << 24 | code.at(i + 3) << 16 | code.at(i + 2) << 8 | code.at(i + 1);

            if (localidx >= localTypes.size())
            {
                PyErr_SetString(PyExc_ValueError, "attempted access of undefined local");
                return NULL;
            }

            current_operation->data = localOffsets.at(localidx);

            uint8_t localType = localTypes.at(localidx);
            const wchar_t *localStr;

            // local.set and local.tee
            if (op == 0x21 || op == 0x22)
            {
                switch (localType)
                {
                case 0x7F:
                    localStr = L"i32";
                    break;

                case 0x7E:
                    localStr = L"i64";
                    break;

                case 0x7D:
                    localStr = L"f32";
                    break;

                case 0x7C:
                    localStr = L"f64";
                    break;

                default:
                    PyErr_SetString(PyExc_TypeError, "unrecognized local type");
                    return NULL;
                }

                if (validateStack.size() < 1)
                {
                    PyErr_SetString(PyExc_TypeError, "missing stack arguments");
                    return NULL;
                }
                wchar_t *lastOp = validateStack.at(validateStack.size() - 1);
                if (wcscmp(localStr, lastOp) != 0)
                {
                    PyErr_SetString(PyExc_TypeError, "incorrect stack arguments");
                    return NULL;
                }

                operandVector.push_back((wchar_t *)localStr);
                validateStack.pop_back();
            }

            // local.get and local.tee
            if (op == 0x20 || op == 0x22)
            {
                switch (localType)
                {
                case 0x7F:
                    resultVector.push_back((wchar_t *)L"i32");
                    break;

                case 0x7E:
                    resultVector.push_back((wchar_t *)L"i64");
                    break;

                case 0x7D:
                    resultVector.push_back((wchar_t *)L"f32");
                    break;

                case 0x7C:
                    resultVector.push_back((wchar_t *)L"f64");
                    break;

                default:
                    PyErr_SetString(PyExc_TypeError, "unrecognized local type");
                    return NULL;
                }

                validateStack = concat(validateStack, {resultVector});
            }
        }
        // global instruction validation
        else if (op == 0x23 || op == 0x24)
        {
            uint32_t globalidx = code.at(i + 4) << 24 | code.at(i + 3) << 16 | code.at(i + 2) << 8 | code.at(i + 1);

            current_operation->data = globalidx;

            if (globalidx >= globalMut.size())
            {
                PyErr_SetString(PyExc_ValueError, "attempted access of undefined global");
                return NULL;
            }

            if (op == 0x24 && globalMut.at(globalidx) == 0)
            {
                PyErr_SetString(PyExc_PermissionError, "attempted modification of immutable global");
                return NULL;
            }

            const wchar_t *globalStr;
            if (op == 0x24)
            {
                switch (globalTypes[globalidx])
                {
                case 0x7F:
                    globalStr = L"i32";
                    break;

                case 0x7E:
                    globalStr = L"i64";
                    break;

                case 0x7D:
                    globalStr = L"f32";
                    break;

                case 0x7C:
                    globalStr = L"f64";
                    break;

                default:
                    PyErr_SetString(PyExc_TypeError, "unrecognized global type");
                    return NULL;
                }

                if (validateStack.size() < 1)
                {
                    PyErr_SetString(PyExc_TypeError, "missing stack arguments");
                    return NULL;
                }
                wchar_t *lastOp = validateStack.at(validateStack.size() - 1);
                if (wcscmp(globalStr, lastOp) != 0)
                {
                    PyErr_SetString(PyExc_TypeError, "incorrect stack arguments");
                    return NULL;
                }

                operandVector.push_back((wchar_t *)globalStr);
                validateStack.pop_back();
            }
            if (op == 0x23)
            {
                switch (globalTypes[globalidx])
                {
                case 0x7F:
                    resultVector.push_back((wchar_t *)L"i32");
                    break;

                case 0x7E:
                    resultVector.push_back((wchar_t *)L"i64");
                    break;

                case 0x7D:
                    resultVector.push_back((wchar_t *)L"f32");
                    break;

                case 0x7C:
                    resultVector.push_back((wchar_t *)L"f64");
                    break;

                default:
                    PyErr_SetString(PyExc_TypeError, "unrecognized global type");
                    return NULL;
                }

                validateStack = concat(validateStack, {resultVector});
            }
        }
        else
        {

            wchar_t *any = (wchar_t *)L"any";

            // stack validation to ensure argument types match operation signatures
            PyObject *signature = PyDict_GetItem(signatures, PyLong_FromLong(op));
            PyObject *operands = PyList_GetItem(signature, 0);
            PyObject *results = PyList_GetItem(signature, 1);
            Py_ssize_t operandsSize = PyList_Size(operands);
            Py_ssize_t resultsSize = PyList_Size(results);

            for (Py_ssize_t j = 0; j < operandsSize; j++)
            {
                PyObject *operand = PyList_GetItem(operands, j);
                Py_ssize_t size = PyUnicode_GetLength(operand);
                wchar_t *operandStr = PyUnicode_AsWideCharString(operand, &size);

                if (validateStack.size() >= (size_t)operandsSize)
                {
                    wchar_t *lastOp = validateStack.at(validateStack.size() - operandsSize + j);
                    if (wcscmp(operandStr, lastOp) != 0 && wcscmp(operandStr, L"any") != 0 && wcscmp(lastOp, L"any") != 0)
                    {
                        PyErr_SetString(PyExc_TypeError, "incorrect stack arguments");
                        return NULL;
                    }
                }
                else
                {
                    PyErr_SetString(PyExc_TypeError, "missing stack arguments");
                    return NULL;
                }

                if (wcscmp(operandStr, L"any") != 0)
                    operandVector.push_back(operandStr);

                else
                {
                    wchar_t *new_any = validateStack.at(validateStack.size() - operandsSize + j);
                    if (wcscmp(any, L"any") != 0 && wcscmp(any, new_any) != 0)
                    {
                        PyErr_SetString(PyExc_TypeError, "incorrect stack arguments");
                        return NULL;
                    }
                    any = new_any;
                    operandVector.push_back(any);
                }
            }
            for (Py_ssize_t j = 0; j < operandsSize; j++)
                validateStack.pop_back();

            for (Py_ssize_t j = 0; j < resultsSize; j++)
            {
                PyObject *result = PyList_GetItem(results, j);
                Py_ssize_t size = PyUnicode_GetLength(result);
                wchar_t *resultStr = PyUnicode_AsWideCharString(result, &size);
                if (wcscmp(resultStr, L"any") != 0)
                    resultVector.push_back(resultStr);

                else
                    resultVector.push_back(any);
            }

            validateStack = concat(validateStack, {resultVector});
        }

        current_operation->arguments = operandVector;
        current_operation->results = resultVector;
        stack.push_back(current_operation);
        if (PyDict_Contains(consumes, PyLong_FromLong(op)))
        {
            long opConsumes = PyLong_AsLong(PyDict_GetItem(consumes, PyLong_FromLong(op)));
            stack.at(stack.size() - 1)->consumes = opConsumes;
            i += opConsumes;
        }
    }

    if ((validateStack.size() != 1 && ret != 0x40) || (validateStack.size() != 0 && ret == 0x40))
    {
        PyErr_SetString(PyExc_ValueError, "stack not empty after execution");
        return NULL;
    }

    if (ret != 0x40)
    {
        wchar_t *retStr = validateStack.at(0);
        if ((wcscmp(retStr, L"i32") == 0 && ret != 0x7F) || (wcscmp(retStr, L"i64") == 0 && ret != 0x7E) || (wcscmp(retStr, L"f32") == 0 && ret != 0x7D) || (wcscmp(retStr, L"f64") == 0 && ret != 0x7C))
        {
            PyErr_SetString(PyExc_TypeError, "incorrect return type on stack");
            return NULL;
        }
    }

    // fetch machine code for opcodes
    size_t offset = 0;
    bytes funcBody = {};
    for (size_t i = 0; i < stack.size(); i++)
    {
        operation *current = stack.at(i);

        // drop
        if (current->opcode == 0x1A)
        {
            if (wcscmp(current->arguments.at(0), L"i32") == 0 || wcscmp(current->arguments.at(0), L"f32") == 0)
                current->code = drop_32;

            else
                current->code = drop_64;
        }
        // select
        else if (current->opcode == 0x1B)
        {
            if (wcscmp(current->results.at(0), L"i32") == 0 || wcscmp(current->results.at(0), L"f32") == 0)
                current->code = select_32;

            else
                current->code = select_64;
        }
        // local.get
        else if (current->opcode == 0x20)
        {
            if (wcscmp(current->results.at(0), L"i32") == 0 || wcscmp(current->results.at(0), L"f32") == 0)
                current->code = local_get_32(current->data);

            else
                current->code = local_get_64(current->data);
        }
        // local.set
        else if (current->opcode == 0x21)
        {
            if (wcscmp(current->arguments.at(0), L"i32") == 0 || wcscmp(current->arguments.at(0), L"f32") == 0)
                current->code = local_set_32(current->data);

            else
                current->code = local_set_64(current->data);
        }
        // local.tee
        else if (current->opcode == 0x22)
        {
            if (wcscmp(current->results.at(0), L"i32") == 0 || wcscmp(current->results.at(0), L"f32") == 0)
                current->code = local_tee_32(current->data);

            else
                current->code = local_tee_64(current->data);
        }
        // global.get
        else if (current->opcode == 0x23)
        {
            if (wcscmp(current->results.at(0), L"i32") == 0 || wcscmp(current->results.at(0), L"f32") == 0)
                current->code = global_get_32(globalTableAddr, current->data);

            else
                current->code = global_get_64(globalTableAddr, current->data);
        }
        // global.set
        else if (current->opcode == 0x24)
        {
            if (wcscmp(current->arguments.at(0), L"i32") == 0 || wcscmp(current->arguments.at(0), L"f32") == 0)
                current->code = global_set_32(globalTableAddr, current->data);

            else
                current->code = global_set_64(globalTableAddr, current->data);
        }
        // memory.grow
        else if (current->opcode == 0x40)
        {
#ifdef ARCH_X86_64
            uint64_t funcAddr = (uint64_t)&growMemory;
            size_t idx = memories.size() - 1;
            uint64_t memAddr = (uint64_t) * (memory_t **)((uint64_t)memories.data() + idx * sizeof(memory_t *));
#ifdef PLATFORM_LINUX
            current->code = memory_grow_linux(funcAddr, memAddr);
#elif PLATFORM_WINDOWS
            current->code = memory_grow_win(funcAddr, memAddr);
#endif
#elif ARCH_X86
            uint32_t funcAddr = (uint32_t)&growMemory;
            size_t idx = memories.size() - 1;
            uint32_t memAddr = (uint32_t) * (memory_t **)((uint32_t)memories.data() + idx * sizeof(memory_t *));
            current->code = memory_grow(funcAddr, memAddr);
#endif
        }
#ifdef ARCH_X86
        // i64.div_s
        else if (current->opcode == 0x7F)
            current->code = div64_s((uint32_t)&func_div64_s, errorPageAddr);

        // i64.div_u
        else if (current->opcode == 0x80)
            current->code = div64_u((uint32_t)&func_div64_u, errorPageAddr);

        // i64.rem_s
        else if (current->opcode == 0x81)
            current->code = rem64((uint32_t)&func_rem64_s, errorPageAddr);

        // i64.rem_u
        else if (current->opcode == 0x82)
            current->code = rem64((uint32_t)&func_rem64_u, errorPageAddr);

#endif
        else
        {
            bytes insts;
            if (!decodeOperation(code, offset, &insts))
                return NULL;

            current->code = insts;
        }

        funcBody = concat(funcBody, {current->code});
        offset += current->consumes;
        offset += 1;
    }
    // flush locals ready for next function
    localTypes = {};
    localOffsets = {};

    bytes returnCode = {};
    if (ret == 0x7F || ret == 0x7D)
        returnCode = ret_v32(errorPageAddr);

    else if (ret == 0x7E || ret == 0x7C)
        returnCode = ret_v64(errorPageAddr);

    else
        returnCode = ret_void(errorPageAddr);

    if (standalone == 1)
    {
        bytes code = concat(initStack, {loadLocals, funcBody, returnCode});
        void *buf = writePage(code);
        standaloneFuncs.push_back(registeredPages.size() - 1);
#ifdef PLATFORM_LINUX
        mprotect(buf, code.size(), PROT_READ | PROT_EXEC);
#elif PLATFORM_WINDOWS
        DWORD dummy;
        VirtualProtect(buf, code.size(), PAGE_EXECUTE_READ, &dummy);
#endif
        return PyLong_FromVoidPtr(buf);
    }

    return PyLong_FromSize_t(writeFunction(concat(initStack, {loadLocals, funcBody, returnCode})));
}

static PyObject *destructStandalones(PyObject *self, PyObject *args)
{
    for (size_t i = 0; i < standaloneFuncs.size(); i++)
    {
#ifdef PLATFORM_LINUX
        munmap(registeredPages.at(standaloneFuncs.at(i)), registeredPageSizes.at(standaloneFuncs.at(i)));
#elif PLATFORM_WINDOWS
        VirtualFree(registeredPages.at(standaloneFuncs.at(i)), 0, MEM_RELEASE);
#endif
    }
    Py_RETURN_NONE;
}

static PyObject *appendGlobal(PyObject *self, PyObject *args)
{
    unsigned long long global;
    int mut;
    char type;
    if (!PyArg_ParseTuple(args, "Kpb", &global, &mut, &type))
        return NULL;

    size_t offset = globalTable.size();
    globalMut.push_back(mut);

    if (type == 0x7F || type == 0x7D)
        globalTable = concat(globalTable, {{(uint8_t)(global >> 24), (uint8_t)(global >> 16), (uint8_t)(global >> 8), (uint8_t)global}});

    else if (type == 0x7E || type == 0x7C)
        globalTable = concat(globalTable, {{(uint8_t)(global >> 56), (uint8_t)(global >> 48), (uint8_t)(global >> 40), (uint8_t)(global >> 32), (uint8_t)(global >> 24), (uint8_t)(global >> 16), (uint8_t)(global >> 8), (uint8_t)global}});

    else
    {
        PyErr_SetString(PyExc_TypeError, "unrecognized global type");
        return NULL;
    }

    globalTypes.push_back(type);

    return Py_BuildValue("O", PyLong_FromSize_t(offset));
}

static PyObject *writeGlobals(PyObject *self, PyObject *args)
{
    void *buf = writePage(globalTable);
    globalTableAddr = (uint64_t)buf;
    return Py_BuildValue("O", PyLong_FromVoidPtr(buf));
}

static PyObject *flushGlobals(PyObject *self, PyObject *args)
{
    globalTable = {};
    globalTypes = {};
    globalMut = {};
    Py_RETURN_NONE;
}

bool createMemory(int fd, uint32_t minPages, uint32_t maxPages)
{
    memory_t *mem = new memory_t();
    mem->currentPage = -1;
    mem->numPages = minPages; // initialize to minimum required
    mem->maxPages = maxPages;
    mem->fd = fd;
    if (mem->fd == -1)
        return false;

#ifdef PLATFORM_WINDOWS
    mem->hFile = (HANDLE)_get_osfhandle(fd);
    if (mem->hFile == INVALID_HANDLE_VALUE)
        return false;

    mem->hMap = NULL;
#endif
    memories.push_back(mem);
    return true;
}

static PyObject *createMemory(PyObject *self, PyObject *args)
{
    long long fd;
    uint32_t minPages, maxPages;
    if (!PyArg_ParseTuple(args, "LLL", &fd, &minPages, &maxPages))
        return NULL;

    maxPages = maxPages < 65536 ? maxPages : 65536;

    if (!createMemory((int)fd, minPages, maxPages))
    {
        PyErr_SetString(PyExc_MemoryError, "failed to create memory");
        return NULL;
    }
    return PyLong_FromSize_t(memories.size() - 1);
}

bool loadMemoryPage(memory_t *mem, uint16_t pageNum)
{
    unmapMemory(mem);
#ifdef PLATFORM_LINUX
    mem->address = mmap(NULL, 65536, PROT_READ | PROT_WRITE, MAP_SHARED, mem->fd, 65536 * pageNum);
    if (mem->address == MAP_FAILED)
        return false;

#elif PLATFORM_WINDOWS
    mem->hMap = CreateFileMapping(mem->hFile, NULL, PAGE_READWRITE, 0, 0, NULL);
    if (mem->hMap == NULL)
        return false;

    mem->address = MapViewOfFile(mem->hMap, FILE_MAP_READ | FILE_MAP_WRITE, 0, 65536 * pageNum, 65536);
    if (mem->address == NULL)
        return false;

#endif
    mem->currentPage = pageNum;
    return true;
}

static PyObject *loadMemoryPage(PyObject *self, PyObject *args)
{
    uint32_t memoryIndex;
    uint16_t pageNum;
    if (!PyArg_ParseTuple(args, "LH", &memoryIndex, &pageNum))
        return NULL;

    if (!loadMemoryPage(memories.at(memoryIndex), pageNum))
    {
        PyErr_SetString(PyExc_MemoryError, "failed to load memory page");
        return NULL;
    }

    return PyLong_FromVoidPtr(memories.at(memoryIndex)->address);
}

static PyMethodDef methods[] = {
    {"append_global", appendGlobal, METH_VARARGS, "append_global(value, mutable, global_type, /)\n--\n\nRegister new global variable."},
    {"create_function", createFunction, METH_VARARGS, "create_function(ret_type, codebuf, argbuf, localbuf, op_con, op_sig, standalone, /)\n--\n\nGenerate a new function and add it to the function page."},
    {"create_memory", createMemory, METH_VARARGS, "create_memory(fd, min_pages, max_pages, /)\n--\n\nRegister a new memory backed by fd."},
    {"destruct_standalone", destructStandalones, METH_NOARGS, "destruct_standalone()\n--\n\nFree up temporary functions."},
    {"flush_globals", flushGlobals, METH_NOARGS, "flush_globals()\n--\n\nClear generated globals ready for the next module."},
    {"load_memory_page", loadMemoryPage, METH_VARARGS, "load_memory_page(index, page_no, /)\n--\n\nLoad page_no of the memory specified by index."},
    {"write_function_page", writeFunctionPage, METH_NOARGS, "write_function_page()\n--\n\nCommit generated functions to memory."},
    {"write_globals", writeGlobals, METH_NOARGS, "write_globals()\n--\n\nCommit generated globals to memory."},
    {NULL, NULL, 0, NULL}};

static PyModuleDef module = {
    PyModuleDef_HEAD_INIT,
    "nativelib",
    NULL,
    -1,
    methods};

PyMODINIT_FUNC PyInit_nativelib()
{
#ifdef PLATFORM_LINUX
    page_size = sysconf(_SC_PAGE_SIZE);
#elif PLATFORM_WINDOWS
    SYSTEM_INFO sysinfo;
    GetSystemInfo(&sysinfo);
    page_size = sysinfo.dwPageSize;
#endif

    errorPageAddr = (uint64_t)writePage(errorPage);
    Py_AtExit(&freePages);
    return PyModule_Create(&module);
}
