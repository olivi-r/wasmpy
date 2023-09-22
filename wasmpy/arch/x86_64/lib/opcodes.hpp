#ifndef OPCODES_H
#define OPCODES_H

#include "nativelib.hpp"

bool decodeOperation(bytes buf, size_t offset, bytes *insts);
bytes ret_v32(uint64_t errorPageAddr);
bytes ret_v64(uint64_t errorPageAddr);
bytes ret_void(uint64_t errorPageAddr);
bytes sub_rsp(uint8_t offset);

bytes local_get_32(uint32_t offset);
bytes local_get_64(uint32_t offset);
bytes local_set_32(uint32_t offset);
bytes local_set_64(uint32_t offset);
bytes local_tee_32(uint32_t offset);
bytes local_tee_64(uint32_t offset);

bytes global_get_32(uint64_t globalTableAddr, uint32_t offset);
bytes global_get_64(uint64_t globalTableAddr, uint32_t offset);
bytes global_set_32(uint64_t globalTableAddr, uint32_t offset);
bytes global_set_64(uint64_t globalTableAddr, uint32_t offset);

bytes memory_size(uint64_t addr);
bytes memory_grow_linux(uint64_t funcAddr, uint64_t memAddr);
bytes memory_grow_win(uint64_t funcAddr, uint64_t memAddr);

bytes reg_param_f32_linux_0(uint8_t offset);
bytes reg_param_f32_linux_1(uint8_t offset);
bytes reg_param_f32_linux_2(uint8_t offset);
bytes reg_param_f32_linux_3(uint8_t offset);
bytes reg_param_f32_linux_4(uint8_t offset);
bytes reg_param_f32_linux_5(uint8_t offset);
bytes reg_param_f32_linux_6(uint8_t offset);
bytes reg_param_f32_linux_7(uint8_t offset);

bytes reg_param_f64_linux_0(uint8_t offset);
bytes reg_param_f64_linux_1(uint8_t offset);
bytes reg_param_f64_linux_2(uint8_t offset);
bytes reg_param_f64_linux_3(uint8_t offset);
bytes reg_param_f64_linux_4(uint8_t offset);
bytes reg_param_f64_linux_5(uint8_t offset);
bytes reg_param_f64_linux_6(uint8_t offset);
bytes reg_param_f64_linux_7(uint8_t offset);

bytes reg_param_i32_linux_0(uint8_t offset);
bytes reg_param_i32_linux_1(uint8_t offset);
bytes reg_param_i32_linux_2(uint8_t offset);
bytes reg_param_i32_linux_3(uint8_t offset);
bytes reg_param_i32_linux_4(uint8_t offset);
bytes reg_param_i32_linux_5(uint8_t offset);

bytes reg_param_i64_linux_0(uint8_t offset);
bytes reg_param_i64_linux_1(uint8_t offset);
bytes reg_param_i64_linux_2(uint8_t offset);
bytes reg_param_i64_linux_3(uint8_t offset);
bytes reg_param_i64_linux_4(uint8_t offset);
bytes reg_param_i64_linux_5(uint8_t offset);

extern bytes reg_param_f32_win_0;
extern bytes reg_param_f32_win_1;
extern bytes reg_param_f32_win_2;
extern bytes reg_param_f32_win_3;

extern bytes reg_param_f64_win_0;
extern bytes reg_param_f64_win_1;
extern bytes reg_param_f64_win_2;
extern bytes reg_param_f64_win_3;

extern bytes reg_param_int_win_0;
extern bytes reg_param_int_win_1;
extern bytes reg_param_int_win_2;
extern bytes reg_param_int_win_3;

extern bytes drop_32;
extern bytes drop_64;
extern bytes select_32;
extern bytes select_64;

extern bytes initStack;
extern bytes pushl0;
extern bytes pushq0;

#endif
