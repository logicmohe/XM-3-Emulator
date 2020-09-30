/* File: emulator.h
 * Author: Youwei Zhang
 * School: Dalhousie University
 * Dept: Electrical and Computer Engineering
 * Course: ECED 3403 Computer Architecture
 *
 * Purpose：    header file for emulator, including necessary variable and terms
 * Last Modified: 2020.7.30
 */

#pragma once

enum BitMasking {
    BIT0MASK = 0x0001,  BIT1MASK = 0x0002,  BIT2MASK = 0x0004,  BIT3MASK = 0x0008, 
    BIT4MASK = 0x0010,  BIT5MASK = 0x0020,  BIT6MASK = 0x0040,  BIT7MASK = 0x0080, 
    BIT8MASK = 0x0100,  BIT9MASK = 0x0200,  BIT10MASK = 0x0400, BIT11MASK = 0x0800, 
    BIT12MASK = 0x1000, BIT13MASK = 0x2000, BIT14MASK = 0x4000, BIT15MASK = 0x8000
};

/* OPCODE for instructions */
enum TWOOPEROPCODE {
    ADDOPCODE = 0b01000000, ADDCOPCODE = 0b01000001, SUBOPCODE = 0b01000010, 
    SUBCOPCODE = 0b01000011, CMPOPCODE = 0b01000101, XOROPCODE = 0b01000110, 
    ANDOPCODE = 0b01000111, BITOPCODE = 0b01001000, BICOPCODE = 0b01001001,
    BISOPCODE = 0b01001010
};
enum SINREGOPCODE {
    SRAOPCODE = 0b010011010, RRCOPCODE = 0b010011011 , 
    SWSXOPCODE = 0b010011100, SWPBOPCODE = 0b000, SXTOPCODE=0b001
};
enum REGINIOPCODE {
    MOVLOPCODE = 0b01100, MOVLZOPCODE = 0b01101, 
    MOVLSOPCODE = 0b01110, MOVHOPCODE = 0b01111
};

enum CondExeCode {
    EQ=0b0000, NE=0b0001, CS=0b0010, HS=0b0010, 
    CC=0b0011, LO=0b0011, MI=0b0100, PL=0b0101, 
    VS=0b0110, VC=0b0111, HI=0b1000, LS=0b1001, 
    GE=0b1010, LT=0b1011, GT=0b1100, LE=0b1101, 
    AL=0b1110
};

extern int inst_type; //Type of current instruction

enum InstType {
    REGINI_INST, MEMACC_INST, TWOOPER_INST,
    REGEXC_INST, SINREG_INST, TRACON_INST,
    CONEXE_INST
};



typedef struct {
    uword Offset : 13;    
    uword Opcode : 3;
}STraConInst;
extern union UTraConInst{STraConInst _struct;     uword _inst_reg;}TraCon;

typedef struct {
    uword FalseCount : 3;
    uword TrueCount : 3;
    uword Condition : 4;
    uword Opcode : 6;
}SConExeInst;
extern union UConExeInst{SConExeInst _struct;     uword _inst_reg;}ConExe;

typedef struct {
    uword DST : 3;
    uword SrcCon : 3;
    uword WB : 1;
    uword RC : 1;
    uword Opcode : 8;
}STwoOperInst;
extern union UTwoOperInst{STwoOperInst _struct;     uword _inst_reg;}TwoOper;

typedef struct {
    uword DST : 3;
    uword SRC : 3;
    uword WB : 1;
    uword Opcode : 9;
}SRegExcInst;
extern union URegExcInst{SRegExcInst _struct;     uword _inst_reg;}RegExc;

typedef struct {
    uword DST : 3;
    uword Opcodes : 3;
    uword WB : 1;
    uword Opcode : 9;
}SIniRegInst;
extern union UIniRegInst{SIniRegInst _struct;     uword _inst_reg;}SinReg;

typedef struct {
    uword DST : 3;
    uword Buffer : 8;
    uword Opcode : 5;
}SRegIniInst;
extern union URegIniInst{SRegIniInst _struct;     uword _inst_reg;}RegIni;

typedef struct {
    uword DST : 3;
    uword SRC : 3;
    uword WB  : 1;
    uword Offset : 7;
    uword Opcode : 2;
}SMemAccInst;
extern union UMemAccInst{SMemAccInst _struct;     uword _inst_reg;}MemAcc;
