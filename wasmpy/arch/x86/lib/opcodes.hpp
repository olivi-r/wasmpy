#ifndef OPCODES_H
#define OPCODES_H

#include "nativelib.hpp"

bytes decodeOperation(bytes buf, size_t offset);
bytes ret_v32(uint64_t errorPageAddr);
bytes ret_v64(uint64_t errorPageAddr);
bytes ret_void(uint64_t errorPageAddr);

bytes local_get_32(uint32_t offset);
bytes local_get_64(uint32_t offset);
bytes local_get_32_small(uint8_t offset);
bytes local_get_64_small(uint8_t offset);

extern bytes drop_32;
extern bytes drop_64;
extern bytes select_32;
extern bytes select_64;

extern bytes pushl0;
extern bytes pushq0;
extern bytes param_32;
extern bytes param_64;
extern bytes initStack;

#endif
