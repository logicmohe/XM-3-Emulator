/* File: debugger.h
 * Author: Youwei Zhang
 * School: Dalhousie University
 * Dept: Electrical and Computer Engineering
 * Course: ECED 3403 Computer Architecture
 *
 * Purpose：Header file for debugger and emulator funcitons
 * 
 * Last Modified: 2020.7.29
 */

#pragma once

#include <stdbool.h>


/*
 * Function: MemDump
 * ------------------
 * Dump memory from device memory stack
 *
 * low_bound:   Starting address to dump memory
 * high_bound:  Stoping address to dump memory
 *
 * returns:     void
 */
void MemDump(int low_bound, int high_bound);


/*
 * Function: Loader
 * ------------------
 * Load the s-record file into memory stack
 *
 * file_name:   xme file name that contains the s-records
 *
 * returns:     bool true if no errors while loading the records
 *              bool false if there is a warning while loading the records
 */
bool Loader(char *file_name);


/*
 * Function: SigintHandler
 * ------------------
 * Execute when detected "Ctrl C" pressed
 *
 * returns:     void
 */
void SigintHandler(int p);


/*
 * Function: Bus
 * ------------------
 * Access the memory stack, either read data from or write data into it.
 *
 * mar:     Memory address being accessed
 * mbr:     The bidirectional memory buffer. Pointer and refers to the address
 *          of data being written or assigned.
 * rw:      read-write indicator defined in enum ACTION { READ, WRITE };
 * bw:      byte-word indicator defined in enum SIZE { BYTE, WORD }
 *
 * returns: void
 */
void Bus(unsigned short mar, unsigned short *mbr, enum ACTION rw, enum SIZE bw);


/*
 * Function: Fetch
 * ------------------
 * Get Instruction Register from memory stack
 *
 * returns:     void
 */
void Fetch(void);


/*
 * Function: Decode
 * ------------------
 * Decode Instruction Register to corresponding instructions types
 *
 * returns:     void
 */
void Decode(void);


/*
 * Function: Execute
 * ------------------
 * Execute instruction
 *
 * returns:     void
 */
void Execute(void);

