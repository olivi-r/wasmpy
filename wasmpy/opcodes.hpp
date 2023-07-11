#ifndef OPCODES_H
#define OPCODES_H

#include "x86.hpp"

std::vector<bytes> decodeFunc(bytes buf, char plat, void *globalTableAddr, uint64_t errorPageAddr);

extern bytes local32;
extern bytes local64;
extern bytes ret_v32;
extern bytes ret_v64;
extern bytes param_32;
extern bytes param_64;
extern bytes initStack;
extern bytes cleanupStack;
extern bytes param_32_linux_0;
extern bytes param_32_linux_1;
extern bytes param_32_linux_2_win_1;
extern bytes param_32_linux_3_win_0;
extern bytes param_32_linux_4_win_2;
extern bytes param_32_linux_5_win_3;
extern bytes param_64_linux_0;
extern bytes param_64_linux_1;
extern bytes param_64_linux_2_win_1;
extern bytes param_64_linux_3_win_0;
extern bytes param_64_linux_4_win_2;
extern bytes param_64_linux_5_win_3;

#endif
