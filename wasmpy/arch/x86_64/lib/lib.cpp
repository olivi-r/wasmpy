#include "lib.hpp"

bytes paramFloat32(Py_ssize_t i, uint8_t offset = 0)
{
#ifdef PLATFORM_LINUX
    if (i == 0)
        return reg_param_f32_linux_0(offset);
    if (i == 1)
        return reg_param_f32_linux_1(offset);
    if (i == 2)
        return reg_param_f32_linux_2(offset);
    if (i == 3)
        return reg_param_f32_linux_3(offset);
    if (i == 4)
        return reg_param_f32_linux_4(offset);
    if (i == 5)
        return reg_param_f32_linux_5(offset);
    if (i == 6)
        return reg_param_f32_linux_6(offset);
    if (i == 7)
        return reg_param_f32_linux_7(offset);
#elif PLATFORM_WINDOWS
    if (i == 0)
        return reg_param_f32_win_0;
    if (i == 1)
        return reg_param_f32_win_1;
    if (i == 2)
        return reg_param_f32_win_2;
    if (i == 3)
        return reg_param_f32_win_3;
#endif
    return {};
}

bytes paramFloat64(Py_ssize_t i, uint8_t offset = 0)
{
#ifdef PLATFORM_LINUX
    if (i == 0)
        return reg_param_f64_linux_0(offset);
    if (i == 1)
        return reg_param_f64_linux_1(offset);
    if (i == 2)
        return reg_param_f64_linux_2(offset);
    if (i == 3)
        return reg_param_f64_linux_3(offset);
    if (i == 4)
        return reg_param_f64_linux_4(offset);
    if (i == 5)
        return reg_param_f64_linux_5(offset);
    if (i == 6)
        return reg_param_f64_linux_6(offset);
    if (i == 7)
        return reg_param_f64_linux_7(offset);
#elif PLATFORM_WINDOWS
    if (i == 0)
        return reg_param_f64_win_0;
    if (i == 1)
        return reg_param_f64_win_1;
    if (i == 2)
        return reg_param_f64_win_2;
    if (i == 3)
        return reg_param_f64_win_3;
#endif
    return {};
}

bytes paramInt(Py_ssize_t i, uint8_t offset = 0, int bits = 64)
{
#ifdef PLATFORM_LINUX
    if (bits == 32)
    {
        if (i == 0)
            return reg_param_i32_linux_0(offset);
        if (i == 1)
            return reg_param_i32_linux_1(offset);
        if (i == 2)
            return reg_param_i32_linux_2(offset);
        if (i == 3)
            return reg_param_i32_linux_3(offset);
        if (i == 4)
            return reg_param_i32_linux_4(offset);
        if (i == 5)
            return reg_param_i32_linux_5(offset);
    }
    else
    {
        if (i == 0)
            return reg_param_i64_linux_0(offset);
        if (i == 1)
            return reg_param_i64_linux_1(offset);
        if (i == 2)
            return reg_param_i64_linux_2(offset);
        if (i == 3)
            return reg_param_i64_linux_3(offset);
        if (i == 4)
            return reg_param_i64_linux_4(offset);
        if (i == 5)
            return reg_param_i64_linux_5(offset);
    }
#elif PLATFORM_WINDOWS
    if (i == 0)
        return reg_param_int_win_0;
    if (i == 1)
        return reg_param_int_win_1;
    if (i == 2)
        return reg_param_int_win_2;
    if (i == 3)
        return reg_param_int_win_3;
#endif
    return {};
}

bytes regParam(const char *argbuf, Py_ssize_t arglen)
{
    bytes code = {};
#ifdef PLATFORM_LINUX
    int32_t offset = 0;
    uint32_t rtl_offset = 8;
    int param_int = 0;
    int param_float = 0;
    for (Py_ssize_t i = 0; i < arglen; i++)
    {
        localTypes.push_back(argbuf[i]);
        if (argbuf[i] == 0x7F || argbuf[i] == 0x7D)
        {
            offset -= 4;
            if (param_int < 6 && argbuf[i] == 0x7F)
            {
                code = concat(code, {paramInt(param_int, (uint8_t)offset, 32)});
                param_int += 1;
                localOffsets.push_back(offset);
            }
            else if (param_float < 8 && argbuf[i] == 0x7D)
            {
                code = concat(code, {paramFloat32(param_float, (uint8_t)offset)});
                param_float += 1;
                localOffsets.push_back(offset);
            }
            else
            {
                localOffsets.push_back(rtl_offset);
                rtl_offset += 4;
            }
        }
        else
        {
            offset -= 8;
            if (param_int < 6 && argbuf[i] == 0x7E)
            {
                code = concat(code, {paramInt(param_int, (uint8_t)offset)});
                param_int += 1;
                localOffsets.push_back(offset);
            }
            else if (param_float < 8 && argbuf[i] == 0x7C)
            {
                code = concat(code, {paramFloat64(param_float, (uint8_t)offset)});
                param_float += 1;
                localOffsets.push_back(offset);
            }
            else
            {
                localOffsets.push_back(rtl_offset);
                rtl_offset += 8;
            }
        }
    }
    code = concat(code, {sub_rsp((uint8_t)(-offset))});
#elif PLATFORM_WINDOWS
    uint32_t offset = 8;
    for (Py_ssize_t i = 0; i < arglen; i++)
    {
        localTypes.push_back(argbuf[i]);
        if (i < 4)
        {
            offset += 8;
            if (argbuf[i] == 0x7F || argbuf[i] == 0x7E)
                code = concat(code, {paramInt(i)});

            else if (argbuf[i] == 0x7D)
                code = concat(code, {paramFloat32(i)});

            else if (argbuf[i] == 0x7C)
                code = concat(code, {paramFloat64(i)});
        }
        else if (argbuf[i] == 0x7F || argbuf[i] == 0x7D)
            offset += 4;

        else
            offset += 8;

        localOffsets.push_back(offset);
    }
#endif
    return code;
}
