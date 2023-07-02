#include "x86.hpp"
#include "opcodes.hpp"

std::vector<void (*)()> registeredFuncs = {};
bytes globalTable = {};

bytes concat(bytes v0, std::vector<bytes> vn)
{
    for (size_t i = 0; i < vn.size(); i++)
    {
        v0.insert(v0.end(), vn.at(i).begin(), vn.at(i).end());
    }

    return v0;
}

void freeFuncs()
{
    for (size_t i = 0; i < registeredFuncs.size(); i++)
    {
#ifdef linux
        munmap((void *)registeredFuncs.at(i), sysconf(_SC_PAGE_SIZE));
#endif
#ifdef _WIN32
        VirtualFree((void *)registeredFuncs.at(i), 0, MEM_RELEASE);
#endif
    }
}

auto writeFunction(bytes code)
{
#ifdef linux
    size_t size = sysconf(_SC_PAGE_SIZE) * (1 + code.size() / sysconf(_SC_PAGE_SIZE));
    void *buf = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    memcpy(buf, code.data(), code.size());
    mprotect(buf, code.size(), PROT_READ | PROT_EXEC);
#endif
#ifdef _WIN32
    SYSTEM_INFO sysinf;
    GetSystemInfo(&sysinf);
    size_t size = sysinf.dwPageSize * (1 + code.size() / sysinf.dwPageSize);
    void *buf = VirtualAlloc(nullptr, size, MEM_COMMIT, PAGE_READWRITE);
    memcpy(buf, code.data(), code.size());
    DWORD dummy;
    VirtualProtect(buf, code.size(), PAGE_EXECUTE_READ, &dummy);
#endif
    return reinterpret_cast<void (*)()>(buf);
}

bytes regParam32(const char *argbuf, Py_ssize_t arglen)
{
    bytes code = {};
    int offset = 8;
    for (Py_ssize_t i = 0; i < arglen; i++)
    {
        // movl (%ebp + offset), %eax
        code = concat(code, {{0x8B, 0x85, (uint8_t)offset, (uint8_t)(offset >> 8), (uint8_t)(offset >> 16), (uint8_t)(offset >> 24)}});

        if (argbuf[i] == 0x7F || argbuf[i] == 0x7D)
        {
            // push %ax
            // shr $16, %eax
            // push %ax
            // pushw $2
            // pushw $0
            // pushw $0
            code = concat(code, {{PUSH_V32, PUSH(0), PUSH(0)}});
        }
        else if (argbuf[i] == 0x7E || argbuf[i] == 0x7C)
        {
            offset += 4;
            // movl (%ebp + offset), %ecx
            code = concat(code, {{0x8B, 0x8D, (uint8_t)offset, (uint8_t)(offset >> 8), (uint8_t)(offset >> 16), (uint8_t)(offset >> 24)}});

            // push %ax
            // shr $16, %eax
            // push %ax
            // push %cx
            // shr $16, %ecx
            // push %cx
            // pushw $4
            code = concat(code, {{PUSH_AX, SHR_EAX, PUSH_AX, PUSH_CX, SHR_ECX, PUSH_CX, V64}});
        }

        offset += 4;
    }

    return code;
}

bytes regParam64(const char *argbuf, Py_ssize_t arglen)
{
    bytes code = {};
#ifdef linux
    for (int i = 0; i < std::min((int)arglen, 6); i++)
    {
        if (argbuf[i] == 0x7F || argbuf[i] == 0x7D)
        {
            if (i / 2 == 0)
                // push %[ds]i
                // shr $16, %e[ds]i
                // push %[ds]i
                // pushw $2
                // pushw $0
                // pushw $0
                code = concat(code, {{0x66, (uint8_t)(0x57 - i), 0xC1, (uint8_t)(0xEF - i), 16, 0x66, (uint8_t)(0x57 - i), V32, PUSH(0), PUSH(0)}});

            else if (i / 2 == 1)
                // push %[dc]x
                // shr $16, %e[dc]x
                // push %[dc]x
                // pushw $2
                // pushw $0
                // pushw $0
                code = concat(code, {{0x66, (uint8_t)(0x54 - i), 0xC1, (uint8_t)(0xEC - i), 16, 0x66, (uint8_t)(0x54 - i), V32, PUSH(0), PUSH(0)}});

            else
                // push %r[89]w
                // shr $16, %r[89]d
                // push %r[89]w
                // pushw $2
                // pushw $0
                // pushw $0
                code = concat(code, {{0x66, 0x41, (uint8_t)(0x4C + i), 0x41, 0xC1, (uint8_t)(0xE4 + i), 16, 0x66, 0x41, (uint8_t)(0x4C + i), V32, PUSH(0), PUSH(0)}});
        }
    }
#endif
#ifdef _WIN32
    for (int i = 0; i < min(arglen, 4); i++)
    {
        if (argbuf[i] == 0x7F || argbuf[i] == 0x7D)
        {
            if (i / 2 == 0)
                // push %[cd]x
                // shr $16, %e[cd]x
                // push %[cd]x
                // pushw $2
                // pushw $0
                // pushw $0
                code = concat(code, {{0x66, (uint8_t)(0x51 + i), 0xC1, (uint8_t)(0xE9 + i), 0x10, 0x66, (uint8_t)(0x51 + i), V32, PUSH(0), PUSH(0)}});

            else
                // push %r[89]w
                // shr $16, %r[89]d
                // push %r[89]w
                // pushw $2
                // pushw $0
                // pushw $0
                code = concat(code, {{0x66, 0x41, (uint8_t)(0x4E + i), 0x41, 0xC1, (uint8_t)(0xE6 + i), 16, 0x66, 0x41, (uint8_t)(0x4E + i), V32, PUSH(0), PUSH(0)}});
        }
        else if (argbuf[i] == 0x7E || argbuf[i] == 0x7C)
        {
            if (i / 2 == 0)
                // push %[cd]x
                // shr $16, %r[cd]x
                // push %[cd]x
                // shr $16, %r[cd]x
                // push %[cd]x
                // shr $16, %e[cd]x
                // push %[cd]x
                // pushw $4
                code = concat(code, {{0x66, (uint8_t)(0x51 + i), 0x48, 0xC1, (uint8_t)(0xE9 + i), 16, 0x66, (uint8_t)(0x51 + i), 0x48, 0xC1, (uint8_t)(0xE9 + i), 16, 0x66, (uint8_t)(0x51 + i), 0xC1, (uint8_t)(0xE9 + i), 16, 0x66, (uint8_t)(0x51 + i), V64}});

            else
                // push %r[89]w
                // shr $16, %r[89
                // push %r[89]w
                // shr $16, %r[89
                // push %r[89]w
                // shr $16, %r[89]d
                // push %r[89]w
                // pushw $4
                code = concat(code, {{0x66, 0x41, (uint8_t)(0x4E + i), 0x49, 0xC1, (uint8_t)(0xE6 + i), 16, 0x66, 0x41, (uint8_t)(0x4E + i), 0x49, 0xC1, (uint8_t)(0xE6 + i), 16, 0x66, 0x41, (uint8_t)(0x4E + i), 0x41, 0xC1, (uint8_t)(0xE6 + i), 16, 0x66, 0x41, (uint8_t)(0x4E + i), V64}});
        }
    }
#endif

#ifdef linux
    for (Py_ssize_t i = 0; i < arglen - 6; i++)
    {
        Py_ssize_t offset = i * 8 + 16;
        int argoff = 6;
#endif
#ifdef _WIN32
        for (Py_ssize_t i = 0; i < arglen - 4; i++)
        {
            Py_ssize_t offset = i * 8 + 48;
            int argoff = 4;
#endif
            // movq (%rbp + offset), %rax
            code = concat(code, {{0x48, 0x8B, 0x85, (uint8_t)offset, (uint8_t)(offset >> 8), (uint8_t)(offset >> 16), (uint8_t)(offset >> 24)}});

            if (argbuf[i + argoff] == 0x7F || argbuf[i + argoff] == 0x7D)
            {
                // push %ax
                // shr $16, %eax
                // push %ax
                // pushw $2
                // pushw $0
                // pushw $0
                code = concat(code, {{PUSH_V32, PUSH(0), PUSH(0)}});
            }
            else if (argbuf[i + argoff] == 0x7E || argbuf[i + argoff] == 0x7C)
            {
                // push %ax
                // shr $16, %rax
                // push %ax
                // shr $16, %rax
                // push %ax
                // shr $16, %eax
                // push %ax
                // pushw $4
                code = concat(code, {{PUSH_AX, 0x48, 0xC1, 0xE8, 16, PUSH_AX, 0x48, 0xC1, 0xE8, 16, PUSH_AX, SHR_EAX, PUSH_AX, V64}});
            }
        }

        return code;
    }

    static PyObject *createFunction(PyObject * self, PyObject * args)
    {
        char plat, ret;
        const char *codebuf, *argbuf, *localbuf;
        Py_ssize_t codelen, arglen, locallen;
        if (!PyArg_ParseTuple(args, "bby#y#y#", &plat, &ret, &codebuf, &codelen, &argbuf, &arglen, &localbuf, &locallen))
            return NULL;

        bytes ret_v64, initStack, loadLocals, cleanupStack;
        if (plat == 4)
        {
            // pop %ax
            // pop %dx
            // shl $16, %edx
            // pop %dx
            // pop %ax
            // shl $16, %eax
            // pop %ax
            ret_v64 = {POP_AX, POP_EDX, POP_EAX};

            // push %ebp
            // movl %esp, %ebp
            initStack = {0x55, 0x89, 0xE5};

            loadLocals = regParam32(argbuf, arglen);

            // movl %ebp, %esp
            // pop %ebp
            cleanupStack = {0x89, 0xEC, 0x5D};
        }
        else if (plat == 8)
        {
            // pop %ax
            // pop %ax
            // shl $16, %eax
            // pop %ax
            // shl $16, %rax
            // pop %ax
            // shl $16, %rax
            // pop %ax
            ret_v64 = {POP_V32A, 0x48, 0xC1, 0xE0, 16, POP_AX, 0x48, 0xC1, 0xE0, 16, POP_AX};

            // push %rbp
            // movq %rsp, %rbp
            initStack = {0x55, 0x48, 0x89, 0xE5};

            loadLocals = regParam64(argbuf, arglen);

            // movq %rbp, %rsp
            // pop %rbp
            cleanupStack = {0x48, 0x89, 0xEC, 0x5D};
        }
        else
            return NULL;

        for (Py_ssize_t i = 0; i < locallen; i++)
        {
            if (localbuf[i] == 0x7F || localbuf[i] == 0x7D)
                loadLocals = concat(loadLocals, {{PUSH(0), PUSH(0), PUSH(2), PUSH(0), PUSH(0)}});

            if (localbuf[i] == 0x7E || localbuf[i] == 0x7C)
                loadLocals = concat(loadLocals, {{PUSH(0), PUSH(0), PUSH(0), PUSH(0), PUSH(4)}});
        }

        bytes code(codebuf, codelen + codebuf);

        bytes cleanupCode;
        const char *returnType;

        switch (ret)
        {
        case 0x7F:
            cleanupCode = concat({POP_V32A}, {cleanupStack, {0xC3}});
            returnType = "i32";
            break;

        case 0x7E:
            cleanupCode = concat(ret_v64, {cleanupStack, {0xC3}});
            returnType = "i64";
            break;

        case 0x7D:
            cleanupCode = concat({POP_V32A}, {cleanupStack, {0xC3}});
            returnType = "f32";
            break;

        case 0x7C:
            cleanupCode = concat(ret_v64, {cleanupStack, {0xC3}});
            returnType = "f64";
            break;

        default:
            cleanupCode = concat(cleanupStack, {{0xC3}});
            returnType = "void";
            break;
        }

        auto func = writeFunction(concat(initStack, {loadLocals, decodeFunc(code, plat), cleanupCode}));
        registeredFuncs.push_back(func);

        if (!strcmp(returnType, "void"))
            return Py_BuildValue("OO", PyLong_FromSize_t((size_t)func), Py_None);

        return Py_BuildValue("OU#", PyLong_FromSize_t((size_t)func), returnType, 3);
    }

    PyObject *appendGlobal(PyObject * self, PyObject * args)
    {
        unsigned long long global;
        char type;
        if (!PyArg_ParseTuple(args, "Kb", &global, &type))
            return NULL;

        size_t offset = globalTable.size();

        if (type == 0x7F || type == 0x7D)
            globalTable = concat(globalTable, {{(uint8_t)global, (uint8_t)(global >> 8), (uint8_t)(global >> 16), (uint8_t)(global >> 24), 2, 0, 0, 0, 0}});

        else if (type == 0x7E || type == 0x7C)
            globalTable = concat(globalTable, {{(uint8_t)global, (uint8_t)(global >> 8), (uint8_t)(global >> 16), (uint8_t)(global >> 24), (uint8_t)(global >> 32), (uint8_t)(global >> 40), (uint8_t)(global >> 48), (uint8_t)(global >> 56), 4}});

        return Py_BuildValue("O", PyLong_FromSize_t(offset));
    }

    static PyObject *writeGlobalTable(PyObject * self, PyObject * args)
    {
#ifdef linux
        size_t size = sysconf(_SC_PAGE_SIZE) * (1 + globalTable.size() / sysconf(_SC_PAGE_SIZE));
        void *buf = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
        memcpy(buf, globalTable.data(), globalTable.size());
#endif
#ifdef _WIN32
        SYSTEM_INFO sysinf;
        GetSystemInfo(&sysinf);
        size_t size = sysinf.dwPageSize * (1 + globalTable.size() / sysinf.dwPageSize);
        void *buf = VirtualAlloc(nullptr, size, MEM_COMMIT, PAGE_READWRITE);
        memcpy(buf, globalTable.data(), globalTable.size());
#endif
        registeredFuncs.push_back((void (*)())buf);
        return Py_BuildValue("O", PyLong_FromSize_t((size_t)buf));
    }

    static PyMethodDef methods[] = {
        {"append_global", appendGlobal, METH_VARARGS, NULL},
        {"create_function", createFunction, METH_VARARGS, NULL},
        {"write_global_table", writeGlobalTable, METH_VARARGS, NULL},
        {NULL, NULL, 0, NULL}};

    static struct PyModuleDef module = {
        PyModuleDef_HEAD_INIT,
        "x86",
        NULL,
        -1,
        methods};

    PyMODINIT_FUNC PyInit_x86()
    {
        Py_AtExit(&freeFuncs);
        return PyModule_Create(&module);
    }
