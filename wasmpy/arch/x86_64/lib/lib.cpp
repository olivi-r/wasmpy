#include "lib.hpp"

bytes regParam(const char *argbuf, Py_ssize_t arglen)
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
#elif _WIN32
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
#elif _WIN32
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
