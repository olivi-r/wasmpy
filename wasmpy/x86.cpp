#include "x86.hpp"

std::vector<void (*)()> registeredFuncs = {};
bytes globalTable = {};
uint64_t globalTableAddr = NULL;

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
#ifdef linux
    for (int i = 0; i < std::min((int)arglen, 6); i++)
    {
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
                code = concat(code, {param_32});

            else if (argbuf[i + argoff] == 0x7E || argbuf[i + argoff] == 0x7C)
                code = concat(code, {param_64});
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

        bytes loadLocals;
        if (plat == 4)
            loadLocals = regParam32(argbuf, arglen);

        else if (plat == 8)
            loadLocals = regParam64(argbuf, arglen);

        else
            return NULL;

        for (Py_ssize_t i = 0; i < locallen; i++)
        {
            if (localbuf[i] == 0x7F || localbuf[i] == 0x7D)
                loadLocals = concat(loadLocals, {local32});

            if (localbuf[i] == 0x7E || localbuf[i] == 0x7C)
                loadLocals = concat(loadLocals, {local64});
        }

        bytes code(codebuf, codelen + codebuf);

        bytes returnCode = {};

        if (ret == 0x7F || ret == 0x7D)
            returnCode = ret_v32;

        else if (ret == 0x7E || ret == 0x7C)
            returnCode = ret_v64;

        auto func = writeFunction(concat(initStack, {loadLocals, decodeFunc(code, plat, globalTableAddr), returnCode, cleanupStack, {0xC3}}));
        registeredFuncs.push_back(func);

        return Py_BuildValue("O", PyLong_FromSize_t((size_t)func));
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
        globalTableAddr = (uint64_t)buf;
        // flush table
        globalTable = {};
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
