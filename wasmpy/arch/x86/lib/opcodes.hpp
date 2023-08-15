#ifndef OPCODES_H
#define OPCODES_H

#include "nativelib.hpp"

bool decodeOperation(bytes buf, size_t offset, bytes *insts);
bytes ret_v32(uint64_t errorPageAddr);
bytes ret_v64(uint64_t errorPageAddr);
bytes ret_void(uint64_t errorPageAddr);

bytes local_get_32(uint32_t offset);
bytes local_get_64(uint32_t offset);
bytes local_set_32(uint32_t offset);
bytes local_set_64(uint32_t offset);
bytes local_tee_32(uint32_t offset);
bytes local_tee_64(uint32_t offset);

bytes global_get_32(uint64_t globalTableAddr, uint32_t offset);
bytes global_get_64(uint64_t globalTableAddr, uint32_t offset);

extern bytes drop_32;
extern bytes drop_64;
extern bytes select_32;
extern bytes select_64;

extern bytes pushl0;
extern bytes pushq0;
extern bytes initStack;

#endif
