#include "lib.hpp"

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
