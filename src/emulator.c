/* File: emulator.c
 * Author: Youwei Zhang
 * School: Dalhousie University
 * Dept: Electrical and Computer Engineering
 * Course: ECED 3403 Computer Architecture
 *
 * Purpose：    Emulator: Fetch --> Decode --> Execute
 * Last Modified: 2020.7.22
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "global.h"
#include "function.h"
#include "emulator.h"
#include "instruction.h"

const int Constant[REGNUM] = {0,1,2,4,8,16,32,-1};
int inst_type; //Type of current instruction
union UTraConInst TraCon;
union UConExeInst ConExe;
union UTwoOperInst TwoOper;
union URegExcInst RegExc;
union UIniRegInst SinReg;
union URegIniInst RegIni;
union UMemAccInst MemAcc;


void Fetch(){
    Bus(Register[PC], &inst_reg, READ, WORD);
    Register[PC] += 2;
    cpu_cycle[FETCH_C]++;
}



void Decode(){
    if (inst_reg & BIT15MASK){      /* MEMACC_INST */
        inst_type = MEMACC_INST;
    }else if (!(inst_reg & BIT14MASK)){
        if((inst_reg & BIT13MASK)&&(inst_reg & BIT10MASK))      /* CONEXE_INST */
            inst_type = CONEXE_INST;
        else    /* TRACON_INST */
            inst_type = TRACON_INST;
    }else if (inst_reg & BIT13MASK){/* REGINI_INST */
        inst_type = REGINI_INST;
    }else if ((inst_reg & BIT11MASK)&&(inst_reg & BIT10MASK)){
        if((!(inst_reg & BIT11MASK))&&(!(inst_reg & BIT10MASK)))/* REGEXC_INST */
            inst_type= REGEXC_INST;
        else    /* SINREG_INST */
            inst_type = SINREG_INST;
    }else       /* TWOOPER_INST */
        inst_type= TWOOPER_INST;
    cpu_cycle[DECODE_C]++;
}



void Execute(){
    switch(inst_type){
        case TRACON_INST:
            TraCon._inst_reg=inst_reg;
            if(inst_reg & BIT13MASK){   /* BRA */
                BRA();
            }else   /* BL */
                BL();
            break;

        case CONEXE_INST:
            ConExe._inst_reg=inst_reg;
            CEX();  /* CEX */
            break;

        case TWOOPER_INST:{
            TwoOper._inst_reg=inst_reg;
            uword result;
            uword src = (TwoOper._struct.RC == REG) ? Register[TwoOper._struct.SrcCon] : Constant[TwoOper._struct.SrcCon];
            uword *dst = &Register[TwoOper._struct.DST]; //Pointer to destination register
            uword temp= *dst;   // Temp: store original DST value before operation

            switch(TwoOper._struct.Opcode){
                case ADDOPCODE:     /* ADD */
                    result = ADD(src, dst);
                    break;
                case ADDCOPCODE:    /* ADDC */
                    result = ADDC(src, dst);
                    break;
                case SUBOPCODE:     /* SUB */
                    result=SUB(src, dst);
                    break;
                case SUBCOPCODE:    /* SUBC */
                    result = SUBC(src, dst);
                    break;
                case CMPOPCODE:     /* CMP */
                    result = CMP(src, dst);
                    break;
                case XOROPCODE:     /* XOR */
                    result = XOR(src, dst);
                    break;
                case ANDOPCODE:     /* AND */
                    result = AND(src, dst);
                    break;   
                case BITOPCODE:     /* BIT */
                    result = BIT(src, dst);
                    break;
                case BICOPCODE:     /* BIC */
                    result = BIC(src, dst);
                    break;
                case BISOPCODE:     /* BIS */
                    result = BIS(src, dst);
                    break;
                default:
                    printf("Warning: Instruction not supported\n");
                    return;
                    break;
            }
            PSWZNUpdate(result, TwoOper._struct.WB);
            if(TwoOper._struct.Opcode <= SUBCOPCODE)
                PSWCVUpdate(src, temp, result, TwoOper._struct.WB);}
            break;
            

        case REGEXC_INST:{
            RegExc._inst_reg=inst_reg;
            if (inst_reg & BIT7MASK){   /* SWAP */
                SWAP();
            }else                      /* MOV */
                MOV();
            }
            break;
            

        case SINREG_INST:{
            SinReg._inst_reg=inst_reg;
            uword *dst = &Register[SinReg._struct.DST]; //Pointer to destination Register
            switch(SinReg._struct.Opcode){
                case SRAOPCODE:     /* SRA */
                    SRA(dst);
                    break;
                case  RRCOPCODE:    /* RRC */
                    RRC(dst);
                    break;
                case SWSXOPCODE: 
                    if(SinReg._struct.Opcodes == SWPBOPCODE) /* SWPB */
                        SWPB(dst);
                    else                                     /* SXT */
                        SXT(dst);
                    break;
                default:
                    printf("Warining: Instruction not supported\n");
                    return;
                    break;
                }
            }
            break;

        case REGINI_INST:
            RegIni._inst_reg=inst_reg;
            switch(RegIni._struct.Opcode){
                case MOVLOPCODE:    /* MOVL */
                    MOVL();
                    break;
                case MOVLZOPCODE:   /* MOVLZ */
                    MOVLZ();
                    break;
                case MOVLSOPCODE:   /* MOVLS */
                    MOVLS();
                    break;
                case MOVHOPCODE:   /* MOVH */
                    MOVH();
                    break;
                default:
                    printf("Warning: Instruction not supported\n");
                    return;
                    break;
            }
            break;

        case MEMACC_INST:{
            MemAcc._inst_reg=inst_reg;
            uword effective_addr= Register[MemAcc._struct.SRC]+MemAcc._struct.Offset;
            if(inst_reg & BIT14MASK) /* STR */
                STR(effective_addr);
            else                     /* LDR */
                LDR(effective_addr);
            }
            break;
        
    }
    cpu_cycle[EXECUTE_C]++;
}

