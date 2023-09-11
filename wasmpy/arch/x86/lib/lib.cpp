#include "lib.hpp"

int64_t func_div64_s(int64_t j2, int64_t j1)
{
    return j1 / j2;
}

uint64_t func_div64_u(uint64_t i2, uint64_t i1)
{
    return i1 / i2;
}

int64_t func_rem64_s(int64_t j2, int64_t j1)
{
    return j1 % j2;
}

uint64_t func_rem64_u(uint64_t i2, uint64_t i1)
{
    return i1 % i2;
}

bytes regParam(const char *argbuf, Py_ssize_t arglen)
{
    uint32_t offset = 4;
    for (Py_ssize_t i = 0; i < arglen; i++)
    {
        localTypes.push_back(argbuf[i]);
        if (argbuf[i] == 0x7F || argbuf[i] == 0x7D)
            offset += 4;

        else
            offset += 8;

        localOffsets.push_back(offset);
    }

    return {};
}
