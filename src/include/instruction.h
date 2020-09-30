/* File: instruction.h
 * Author: Youwei Zhang
 * School: Dalhousie University
 * Dept: Electrical and Computer Engineering
 * Course: ECED 3403 Computer Architecture
 *
 * Purpose：    Header file for instructions in Executing phase. 
 *              Including PSW change and instruction function declaration
 * Last Modified: 2020.7.30
 */

/* Program Status Word */

enum PSWBit {V, N, Z, C};

/*
 * Function: PSWZNUpdate
 * ------------------
 * Update Zero and Negative bit in PSW
 *
 * result: result of operation
 * WB: Size indicator for result WORD or BYTE
 * 
 * returns:     void
 */
void PSWZNUpdate(uword result, uword WB);

/*
 * Function: PSWCVUpdate
 * ------------------
 * Update Carry and oVerflow bit in PSW
 *
 * src: source value
 * dst: destination value before operation
 * result: result of operation
 * 
 * returns:     void
 */
void PSWCVUpdate(uword src, uword dst, uword result, uword WB);


/* Instruction Function */


/*
 * Function: BL
 * ------------------
 * Branch with link to subroutine; store return addr in LR
 */
void BL(void);

/*
 * Function: BRA
 * ------------------
 * Branch always to label
 */
void BRA(void);

/*
 * Function: CEX
 * ------------------
 * Conditional execution. Run following instructions based on condition
 */
void CEX(void);

/*
 * Function: ADD
 * ------------------
 * Add SRC to DST
 * 
 * src:     value of source register
 * *dst:    pointer to  destination register
 * 
 * return:  result of operation
 * (The rest TwoOper Instructions have the same argument and return)
 */
uword ADD(uword src, uword*dst);

/*
 * Function: ADDC
 * ------------------
 * Add SRC and carry to DST
 */
uword ADDC(uword src, uword*dst);

/*
 * Function: SUB
 * ------------------
 * Subtract SRC from DST
 */
uword SUB(uword src, uword*dst);

/*
 * Function: SUBC
 * ------------------
 * Subtract SRC from DST plus carry
 */
uword SUBC(uword src, uword*dst);

/*
 * Function: CMP
 * ------------------
 * Compare DST with SRC (subtraction)
 */
uword CMP(uword src, uword*dst);

/*
 * Function: XOR
 * ------------------
 * XOR SRC with DST
 */
uword XOR(uword src, uword*dst);

/*
 * Function: AND
 * ------------------
 * AND SRC with DST
 */
uword AND(uword src, uword*dst);

/*
 * Function: BIT
 * ------------------
 * Test if bits set in SRC are set in DST
 */
uword BIT(uword src, uword*dst);

/*
 * Function: BIC
 * ------------------
 * Clear bits in DST specified by SRC
 */
uword BIC(uword src, uword*dst);

/*
 * Function: BIS
 * ------------------
 * Set bits in DST specified by SRC
 */
uword BIS(uword src, uword*dst);

/*
 * Function: BIS
 * ------------------
 * Swap SRC and DST
 */
void SWAP(void);

/*
 * Function: MOV
 * ------------------
 * Move SRC to DST
 */
void MOV(void);

/*
 * Function: SRA
 * ------------------
 * Arithmetic shift DST right one bit through carry with sign extension
 */
void SRA(uword * dst);

/*
 * Function: RRC
 * ------------------
 * Rotate DST right one bit through carry
 */
void RRC(uword * dst);

/*
 * Function: SWPB
 * ------------------
 * Swap bytes in DST
 * 
 * *dst:    pointer to  destination register
 */
void SWPB(uword * dst);

/*
 * Function: SXT
 * ------------------
 * Sign extend LSB byte to word in DST
 * 
 * *dst:    pointer to  destination register
 */
void SXT(uword * dst);

/*
 * Function: MOVL
 * ------------------
 * Assign buffer value to low byte of DST
 */
void MOVL(void);

/*
 * Function: MOVLZ
 * ------------------
 * Assigne buffer value to low byte of DST, clear MSB of DST
 */
void MOVLZ(void);

/*
 * Function: MOVLS
 * ------------------
 * Assigne buffer value to low byte of DST, set MSB of DST
 */
void MOVLS(void);

/*
 * Function: MOVH
 * ------------------
 * Assign buffer value to high byte of DST
 */
void MOVH(void);

/*
 * Function: STR
 * ------------------
 * Store a register in memory at effective address
 * 
 * effetive_addr:   Effective address in relative addressing, 
 */
void STR(uword effective_addr);

/*
 * Function: LDR
 * ------------------
 * Load a register in memory at effective address
 * 
 * effetive_addr:   Effective address in relative addressing, 
 */
void LDR(uword effective_addr);



