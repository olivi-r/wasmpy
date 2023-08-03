#ifndef OPCODES_H
#define OPCODES_H

#include "lib.hpp"

bytes decodeOperation(bytes buf, size_t offset, char plat);
bytes ret_v32(uint64_t errorPageAddr);
bytes ret_v64(uint64_t errorPageAddr);
bytes ret_void(uint64_t errorPageAddr);

extern bytes local32;
extern bytes local64;
extern bytes param_32;
extern bytes param_64;
extern bytes initStack;

#endif
