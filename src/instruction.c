/* File: instruction.c
 * Author: Youwei Zhang
 * School: Dalhousie University
 * Dept: Electrical and Computer Engineering
 * Course: ECED 3403 Computer Architecture
 *
 * Purpose：    ISA instructions in Execution phase while running the program
 * Last Modified: 2020.7.30
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "global.h"
#include "emulator.h"
#include "function.h"
#include "instruction.h"

/* Program Status Word - Table & Initialization */

bool PSW [4];

const unsigned int carry_table[2][2][2]={
    {{0,0},{1,0}},
    {{1,0},{1,1}}
};

const unsigned int overflow_table[2][2][2]={
    {{0,1},{0,0}},
    {{0,0},{1,0}}
};

/* TRACON_INST */

void BL(){
    Register[LR]=Register[PC];
    uword offset;
    if(TraCon._struct.Offset & BIT9MASK){ //Negative
        offset = (TraCon._struct.Offset << 1) | 0xE000; //0b111x xxxx xxxx xxx0
        Register[PC] = Register[PC] + ((~offset) + 1);  //2's complement
    }else{
        offset = (TraCon._struct.Offset << 1) & 0x1FFE; //0b000x xxxx xxxx xxx0
        Register[PC] += offset;
    }
}

void BRA(){
    Register[LR]=Register[PC];
    uword offset;
    if(TraCon._struct.Offset & BIT9MASK){ //Negative
        offset = (TraCon._struct.Offset << 1) | 0xFC00; //0b1111 11xx xxxx xxx0
        Register[PC] = Register[PC] - ((~offset) + 1);  //2's complement
    }else{
        offset = (TraCon._struct.Offset << 1) & 0x03FE; //0b0000 00xx xxxx xxx0
        Register[PC] += offset;
    }
}


/* CONEXE_INST */

void CEX(){
    bool condition= false;
    switch(ConExe._struct.Condition){
        case EQ:
            if (PSW[Z])
                condition = true;
            break;
        case NE:
            if (!PSW[Z])
                condition = true;
            break;
        case CS:
            if (PSW[C])
                condition = true;
            break;
        case CC:
            if (!PSW[C])
                condition = true;
            break;
        case MI:
            if (PSW[N])
                condition = true;
            break;
        case PL:
            if (!PSW[N])
                condition = true;        
            break;
        case VS:
            if (PSW[V])
                condition = true;
            break;
        case VC:
            if (!PSW[V])
                condition = true;
            break;
        case HI:
            if (PSW[C] && !PSW[Z])
                condition = true;
            break;
        case LS:
            if(!PSW[C] || PSW[Z])
                condition = true;
            break;
        case GE:
            if(PSW[N]==PSW[V])
                condition = true;
            break;
        case LT:
            if (PSW[N]!=PSW[V])
                condition = true;
            break;
        case GT:
            if(!PSW[Z]&& (PSW[N]==PSW[V]))
                condition = true;
            break;
        case LE:
            if(PSW[Z] || (PSW[N]!=PSW[V]))
                condition = true;
            break;
        case AL:
            condition = true;
            break;
    }
    if (condition){
        int i;
        for(i=0; i<ConExe._struct.TrueCount; i++){
            Fetch();
            Decode();
            Execute();
        }
        for(i=0; i<ConExe._struct.FalseCount; i++){
            Fetch();
        }
    }else{
        int i;
        for(i=0; i<ConExe._struct.TrueCount; i++){
            Fetch();
        }
        for(i=0; i<ConExe._struct.FalseCount; i++){
            Fetch();
            Decode();
            Execute();
        }     
    }
};


/* TWOOPER_INST */

uword ADD(uword src, uword*dst){
    if (TwoOper._struct.WB==WORD){
        *dst = *dst + src;
        return *dst;
    }else{
        uword result = (LSB(*dst) + LSB(src));
        *dst = (*dst & HIGHBYTE) + (result & LOWBYTE);
        return result;
    }
}

uword ADDC(uword src, uword*dst){
    if (TwoOper._struct.WB==WORD){
        *dst = *dst + src+ PSW[C];
        return *dst;
    }else{
        uword result = (LSB(*dst) + LSB(src))+PSW[C];
        *dst = (*dst & HIGHBYTE) + (result & LOWBYTE);
        return result;
    }
}

uword SUB(uword src, uword*dst){
    if (TwoOper._struct.WB==WORD){
        *dst = *dst + ~src+ 1;
        return *dst;
    }else{
        uword result = LSB(*dst) + (~LSB(src))+1;
        *dst = (*dst & HIGHBYTE) + (result & LOWBYTE);
        return result;
    }
}

uword SUBC(uword src, uword*dst){
    if (TwoOper._struct.WB==WORD){
        *dst = *dst + ~src+ PSW[C];
        return *dst;
    }else{
        uword result = LSB(*dst) + (~LSB(src))+PSW[C];
        *dst = (*dst & HIGHBYTE) + (result & LOWBYTE);
        return result;
    }
}

uword CMP(uword src, uword*dst){
    uword result;
    if (TwoOper._struct.WB==WORD){
        result = *dst + ~src+ 1;
    }else{
        result = LSB(*dst) + (~LSB(src))+1;
    }
    return result;
}

uword XOR(uword src, uword*dst){
    if (TwoOper._struct.WB==WORD){
        *dst = *dst ^ src;
        return *dst;
    }else{
        uword result = LSB(LSB(*dst) ^ LSB(src));
        *dst=result+MSB(*dst);
        return result;
    }
}

uword AND(uword src, uword*dst){
    if (TwoOper._struct.WB==WORD){
        *dst = *dst & src;
        return *dst;
    }else{
        uword result = LSB(LSB(*dst) & LSB(src));
        *dst= MSB(*dst) + result;
        return result;
    }
}

uword BIT(uword src, uword*dst){
    uword result;
    if (TwoOper._struct.WB==WORD){
        result = *dst & src;
    }else{
        result = LSB(LSB(*dst) & LSB(src));
    }
    return result;
}

uword BIC(uword src, uword*dst){
    if (TwoOper._struct.WB==WORD){
        *dst = *dst & (~src);
        return *dst;
    }else{
        uword result = LSB(LSB(*dst) & (~LSB(src)));
        *dst= MSB(*dst) + result;
        return result;
    }
}

uword BIS(uword src, uword*dst){
    if (TwoOper._struct.WB==WORD){
        *dst = *dst | src;
        return *dst;
    }else{
        uword result = LSB(LSB(*dst) | LSB(src));
        *dst= MSB(*dst) + result;
        return result;
    }
}


/* REGEXC_INST */

void SWAP(){
    if (RegExc._struct.WB==WORD){
        uword temp = Register[RegExc._struct.DST];
        Register[RegExc._struct.DST] = Register[RegExc._struct.SRC];
        Register[RegExc._struct.SRC] = temp;
    }else{
        ubyte temp = LSB(Register[RegExc._struct.DST]);
        Register[RegExc._struct.DST] = LSB(Register[RegExc._struct.SRC])+MSB(Register[RegExc._struct.DST]);
        Register[RegExc._struct.SRC] = temp + MSB(Register[RegExc._struct.SRC]);
    }
}

void MOV(){
    Register[RegExc._struct.DST] = Register[RegExc._struct.SRC];
}


/* SINREG_INST */

void SRA(uword * dst){
    PSW[C]=*dst & 0b1;
    uword temp = (SinReg._struct.WB==BYTE) ? (*dst & BIT7MASK) : (*dst & BIT15MASK);
    *dst=(*dst >> 1) + temp;
}

void RRC(uword * dst){
    PSW[C]=*dst & 0b1;
    uword temp = (SinReg._struct.WB==BYTE) ? (PSW[C]<<7) : (PSW[C]<<15);
    *dst = (*dst >>1) + temp;
}

void SWPB(uword * dst){
    *dst = (LSB(*dst)<<8) + MSB(*dst);
}

void SXT(uword * dst){
    *dst = (*dst & BIT7MASK)? (*dst | HIGHBYTE) : (*dst & LOWBYTE);
}


/* REGINI_INST */

void MOVL(){
    Register[RegIni._struct.DST]= (Register[RegIni._struct.DST] & HIGHBYTE) + RegIni._struct.Buffer;
}

void MOVLZ(){
    Register[RegIni._struct.DST]=RegIni._struct.Buffer;
}

void MOVLS(){
    Register[RegIni._struct.DST]= HIGHBYTE + RegIni._struct.Buffer;
}

void MOVH(){
    Register[RegIni._struct.DST]= LSB(Register[RegIni._struct.DST]) + ((RegIni._struct.Buffer<<8)&HIGHBYTE);
}


/* MEMACC_INST */

void STR(uword effective_addr){
    Bus(effective_addr,&Register[MemAcc._struct.DST],WRITE,MemAcc._struct.WB);
}

void LDR(uword effective_addr){
    Bus(effective_addr,&Register[MemAcc._struct.DST],READ,MemAcc._struct.WB);
}


/* Program Status Word */

void PSWZNUpdate(uword result, uword WB){
    /* Change Zero bit in PSW */
    PSW[Z] = (result ==0) ? 1: 0;

    /* Change Negative bit in PSW */
    result = (WB==BYTE) ? result<<7 : result <<15;//Get MSB
    PSW[N] = (result & ((WB==BYTE)?BIT7MASK:BIT15MASK)) ? 1 : 0;
};


void PSWCVUpdate(uword src, uword dst, uword result, uword WB){
    bool srcbit=src & ((WB==WORD)?BIT15MASK:BIT7MASK);
    bool dstbit=dst & ((WB==WORD)?BIT15MASK:BIT7MASK);
    bool resultbit= result & ((WB==WORD)?BIT15MASK:BIT7MASK);
    /* Change Carry bit in PSW */
    PSW[C]=carry_table[srcbit][dstbit][resultbit];

    /* Change oVerflow bit in PSW */
    PSW[V]=overflow_table[srcbit][dstbit][resultbit];
};
