#include "lib.hpp"

std::vector<void *> registeredPages = {};
bytes globalTable = {};
bytes globalTypes = {};
std::vector<int> globalMut = {};
bytes localTypes = {};
uint64_t globalTableAddr;
uint64_t errorPageAddr;

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
#ifdef __linux__
    size_t size = sysconf(_SC_PAGE_SIZE) * (1 + data.size() / sysconf(_SC_PAGE_SIZE));
    buf = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
#endif
#ifdef _WIN32
    SYSTEM_INFO sysinfo;
    GetSystemInfo(&sysinfo);
    size_t size = sysinfo.dwPageSize * (1 + data.size() / sysinfo.dwPageSize);
    buf = VirtualAlloc(nullptr, size, MEM_COMMIT, PAGE_READWRITE);
#endif
    memcpy(buf, data.data(), data.size());
    registeredPages.push_back(buf);
    return buf;
}

void freePages()
{
    for (size_t i = 0; i < registeredPages.size(); i++)
    {
#ifdef __linux__
        munmap(registeredPages.at(i), sysconf(_SC_PAGE_SIZE));
#endif
#ifdef _WIN32
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
#ifdef __linux__
    mprotect(buf, text.size(), PROT_READ | PROT_EXEC);
#endif
#ifdef _WIN32
    DWORD dummy;
    VirtualProtect(buf, text.size(), PAGE_EXECUTE_READ, &dummy);
#endif
    return PyLong_FromVoidPtr(buf);
}

bytes regParam32(const char *argbuf, Py_ssize_t arglen)
{
    bytes code = {};
    int offset = 8;
    for (Py_ssize_t i = 0; i < arglen; i++)
    {
        localTypes.push_back(argbuf[i]);

        // movl (%ebp + offset), %eax
        code = concat(code, {{0x8B, 0x85, (uint8_t)offset, (uint8_t)(offset >> 8), (uint8_t)(offset >> 16), (uint8_t)(offset >> 24)}});

        if (argbuf[i] == 0x7F || argbuf[i] == 0x7D)
            code = concat(code, {param_32});

        else if (argbuf[i] == 0x7E || argbuf[i] == 0x7C)
        {
            offset += 4;
            // movl (%ebp + offset), %ecx
            code = concat(code, {{0x8B, 0x8D, (uint8_t)offset, (uint8_t)(offset >> 8), (uint8_t)(offset >> 16), (uint8_t)(offset >> 24)}});
            code = concat(code, {param_64});
        }

        offset += 4;
    }

    return code;
}

bytes regParam64(const char *argbuf, Py_ssize_t arglen)
{
    bytes code = {};
#ifdef __linux__
    for (int i = 0; i < std::min((int)arglen, 6); i++)
    {
        localTypes.push_back(argbuf[i]);

        if (argbuf[i] == 0x7F || argbuf[i] == 0x7D)
        {
            if (i == 0)
                code = concat(code, {param_32_linux_0});

            else if (i == 1)
                code = concat(code, {param_32_linux_1});

            else if (i == 2)
                code = concat(code, {param_32_linux_2_win_1});

            else if (i == 3)
                code = concat(code, {param_32_linux_3_win_0});

            else if (i == 4)
                code = concat(code, {param_32_linux_4_win_2});

            else if (i == 5)
                code = concat(code, {param_32_linux_5_win_3});
        }
        else if (argbuf[i] == 0x7E || argbuf[i] == 0x7C)
        {
            if (i == 0)
                code = concat(code, {param_64_linux_0});

            else if (i == 1)
                code = concat(code, {param_64_linux_1});

            else if (i == 2)
                code = concat(code, {param_64_linux_2_win_1});

            else if (i == 3)
                code = concat(code, {param_64_linux_3_win_0});

            else if (i == 4)
                code = concat(code, {param_64_linux_4_win_2});

            else if (i == 5)
                code = concat(code, {param_64_linux_5_win_3});
        }
    }
#endif
#ifdef _WIN32
    for (int i = 0; i < min(arglen, 4); i++)
    {
        localTypes.push_back(argbuf[i]);

        if (argbuf[i] == 0x7F || argbuf[i] == 0x7D)
        {
            if (i == 0)
                code = concat(code, {param_32_linux_3_win_0});

            else if (i == 1)
                code = concat(code, {param_32_linux_2_win_1});

            else if (i == 2)
                code = concat(code, {param_32_linux_4_win_2});

            else if (i == 3)
                code = concat(code, {param_32_linux_5_win_3});
        }
        else if (argbuf[i] == 0x7E || argbuf[i] == 0x7C)
        {
            if (i == 0)
                code = concat(code, {param_64_linux_3_win_0});

            else if (i == 1)
                code = concat(code, {param_64_linux_2_win_1});

            else if (i == 2)
                code = concat(code, {param_64_linux_4_win_2});

            else if (i == 3)
                code = concat(code, {param_64_linux_5_win_3});
        }
    }
#endif

#ifdef __linux__
    int argoff = 6;
    Py_ssize_t count = arglen - argoff;
    int baseOffset = 16;
#endif
#ifdef _WIN32
    int argoff = 4;
    Py_ssize_t count = arglen - argoff;
    int baseOffset = 48;
#endif
    for (Py_ssize_t i = 0; i < count; i++)
    {
        localTypes.push_back(argbuf[i + argoff]);

        Py_ssize_t offset = i * 8 + baseOffset;
        // movq (%rbp + offset), %rax
        code = concat(code, {{0x48, 0x8B, 0x85, (uint8_t)offset, (uint8_t)(offset >> 8), (uint8_t)(offset >> 16), (uint8_t)(offset >> 24)}});

        if (argbuf[i + argoff] == 0x7F || argbuf[i + argoff] == 0x7D)
            code = concat(code, {param_32});

        else if (argbuf[i + argoff] == 0x7E || argbuf[i + argoff] == 0x7C)
            code = concat(code, {param_64});
    }

    return code;
}

struct operation
{
    uint8_t opcode;
    std::vector<wchar_t *> arguments;
    std::vector<wchar_t *> results;
    long consumes;
    bytes code;
    operation(uint8_t opcode, std::vector<wchar_t *> arguments, std::vector<wchar_t *> results) : opcode(opcode), arguments(arguments), results(results), consumes(0){};
};

static PyObject *createFunction(PyObject *self, PyObject *args)
{
    char plat, ret;
    const char *codebuf, *argbuf, *localbuf;
    Py_ssize_t codelen, arglen, locallen;
    PyObject *consumes, *signatures;
    if (!PyArg_ParseTuple(args, "bby#y#y#OO", &plat, &ret, &codebuf, &codelen, &argbuf, &arglen, &localbuf, &locallen, &consumes, &signatures))
        return NULL;

    bytes loadLocals;
    if (plat == 4)
        loadLocals = regParam32(argbuf, arglen);

    else if (plat == 8)
        loadLocals = regParam64(argbuf, arglen);

    else
    {
        PyErr_SetString(PyExc_EnvironmentError, "unsupported architecture bits");
        return NULL;
    }
    for (Py_ssize_t i = 0; i < locallen; i++)
    {
        localTypes.push_back(localbuf[i]);

        if (localbuf[i] == 0x7F || localbuf[i] == 0x7D)
            loadLocals = concat(loadLocals, {local32});

        if (localbuf[i] == 0x7E || localbuf[i] == 0x7C)
            loadLocals = concat(loadLocals, {local64});
    }

    bytes code(codebuf, codelen + codebuf);

    std::vector<operation *> stack = {};

    std::vector<wchar_t *> validateStack = {};
    for (size_t i = 0; i < code.size(); i++)
    {
        uint8_t op = code.at(i);

        std::vector<wchar_t *> operandVector = {};
        std::vector<wchar_t *> resultVector = {};

        // local instruction validation
        if (op == 0x20 || op == 0x21 || op == 0x22)
        {
            uint32_t localidx = code.at(i + 4) << 24 | code.at(i + 3) << 16 | code.at(i + 2) << 8 | code.at(i + 1);

            if (localidx >= localTypes.size())
            {
                PyErr_SetString(PyExc_ValueError, "attempted access of undefined local");
                return NULL;
            }

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

        stack.push_back(new operation(op, operandVector, resultVector));
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

    size_t offset = 0;
    bytes funcBody = {};
    for (size_t i = 0; i < stack.size(); i++)
    {
        operation *current = stack.at(i);
        current->code = decodeOperation(code, offset, plat);
        funcBody = concat(funcBody, {current->code});
        offset += current->consumes;
        offset += 1;
    }

    // flush locals ready for next function
    localTypes = {};

    bytes returnCode = {};

    if (plat == 4)
    {
        if (ret == 0x7F || ret == 0x7D)
            returnCode = ret_v32(errorPageAddr);

        else if (ret == 0x7E || ret == 0x7C)
            returnCode = ret_v64(errorPageAddr);

        else
            returnCode = ret_void(errorPageAddr);
    }
    else
    {
        if (ret == 0x7F || ret == 0x7D)
            returnCode = ret_v32(errorPageAddr);

        else if (ret == 0x7E || ret == 0x7C)
            returnCode = ret_v64(errorPageAddr);

        else
            returnCode = ret_void(errorPageAddr);
    }
    return Py_BuildValue("O", PyLong_FromSize_t(writeFunction(concat(initStack, {loadLocals, funcBody, returnCode}))));
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
        globalTable = concat(globalTable, {{(uint8_t)global, (uint8_t)(global >> 8), (uint8_t)(global >> 16), (uint8_t)(global >> 24), 2, 0, 0, 0, 0}});

    else if (type == 0x7E || type == 0x7C)
        globalTable = concat(globalTable, {{(uint8_t)global, (uint8_t)(global >> 8), (uint8_t)(global >> 16), (uint8_t)(global >> 24), (uint8_t)(global >> 32), (uint8_t)(global >> 40), (uint8_t)(global >> 48), (uint8_t)(global >> 56), 4}});

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

static PyMethodDef methods[] = {
    {"append_global", appendGlobal, METH_VARARGS, NULL},
    {"create_function", createFunction, METH_VARARGS, NULL},
    {"flush_globals", flushGlobals, METH_NOARGS, NULL},
    {"write_function_page", writeFunctionPage, METH_NOARGS, NULL},
    {"write_globals", writeGlobals, METH_NOARGS, NULL},
    {NULL, NULL, 0, NULL}};

static PyModuleDef module = {
    PyModuleDef_HEAD_INIT,
    "nativelib",
    NULL,
    -1,
    methods};

PyMODINIT_FUNC PyInit_nativelib()
{
    errorPageAddr = (uint64_t)writePage(errorPage);
    Py_AtExit(&freePages);
    return PyModule_Create(&module);
}
