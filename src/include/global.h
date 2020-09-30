/* File: global.h
 * Author: Youwei Zhang
 * School: Dalhousie University
 * Dept: Electrical and Computer Engineering
 * Course: ECED 3403 Computer Architecture
 *
 * Purpose：    Header file containing the global variable
 * 
 * Last Modified: 2020.7.29
 */

#pragma once

#include <signal.h> 

#define MEMSIZE 65536
#define COMMANDSIZE 73      // 2(header)+4(length)+64(content)+2(checksum)
#define NAMESIZE 33         // 32+1
#define BYTESHIFT 0x100     // byte: 8 bits
#define NIBBLESHIFT 0x10    // nibble: 4 bits
#define LOWBYTE 0x00FF      // 0b0000 0000 1111 1111
#define HIGHBYTE 0xFF00     // 0b1111 1111 0000 0000 
#define REGNUM 8            // 8 registers
#define CYCLENUM 4          // 4 types of CPU cycles

#define MSB(word) (((word)>>8) & LOWBYTE) // Macro: Get Most Significant Byte from word
#define LSB(word) ((word) & LOWBYTE)    // Macro: Get Least Significant Byte from word

enum ACTION     { READ, WRITE };
enum SIZE       { WORD, BYTE  };
enum SRCCON     { REG , CONST };
enum REGISTER   { R0, R1, R2, R3, R4, LR, SP, PC};
enum CPUCYCLE   { FETCH_C, DECODE_C, EXECUTE_C, MEMACC_C};


typedef unsigned char  ubyte;   // 8 bit    0x00    (usually used for char or byte value in s-record)
typedef unsigned short uword;   // 16 bit   0x0000  (usually used for address)
typedef   signed short sword;   // 16 bit   0x0000  (usually used for singed value)

extern ubyte memory_stack[MEMSIZE];    //64 KiB Memroy Stack

extern volatile sig_atomic_t ctrl_c_catcher; //T|F indicator to show if the ^C signal happens (Reference to Assign 3 Description)

extern uword break_point;
extern uword starting_addr;        //Address that program starts execution
extern uword inst_reg;

extern int cpu_cycle[CYCLENUM]; //CPU cycles

/* Register File */
extern uword Register[REGNUM]; // {r0, r1, r2, r3, r4, r5/link_register, r6/stack_pointer, r7/program_counter}
