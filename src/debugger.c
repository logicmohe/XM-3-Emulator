/* File: debugger.c
 * Author: Youwei Zhang
 * School: Dalhousie University
 * Dept: Electrical and Computer Engineering
 * Course: ECED 3403 Computer Architecture
 *
 * Purpose：    Debugger file containing the memory dump, file loader, signal handler and BUS
 * Last Modified: 2020.7.22
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "global.h"
#include "function.h"


void MemDump(int low_bound, int high_bound){
    uword addr, val; 
    ubyte ascii[NIBBLESHIFT];
    for (addr = (low_bound / NIBBLESHIFT) * NIBBLESHIFT;// addr should be n*0x10 | Alter way: low_bound & 0xFFFE
            addr <= high_bound; addr += NIBBLESHIFT) {
        printf("%hx: ", addr);
        uword tempaddr;
        for (tempaddr = 0; tempaddr < NIBBLESHIFT; tempaddr++) {
            Bus(tempaddr + addr, &val, READ, BYTE);
            printf("%02hx ", val);
            ascii[tempaddr] =
                (val > 0x1F) && (val < 0x7F) ? val : '.';
        }
        for (tempaddr = 0; tempaddr < NIBBLESHIFT; tempaddr++) {
            printf("%c", ascii[tempaddr]);
        }
        printf("\n");
    }
}


bool Loader(char *file_name) {
    FILE *XME = fopen(file_name, "r");
    if (XME == NULL) {
        printf("Warning: Mssing file\n");
        return false;
    }

    char source_name[NAMESIZE];

    char record[COMMANDSIZE];
    while (fgets(record, sizeof(record), XME) != NULL) {
        char r_name[3];
        ubyte r_length, r_checksum, checksum;
        uword r_addr;
        sscanf(record, "%2s%2hhx%4hx", r_name, &r_length, &r_addr);
        checksum = r_length + (r_addr>>8 & LOWBYTE) + (r_addr & LOWBYTE); //Alter way: r_addr / BYTESHIFT + r_addr % BYTESHIFT
        r_length -= 3;  // Ignore the length of addr & checksum bytes
        int pos = 8;
        /* S- record */
        if (strcasecmp(r_name, "S0") == 0) {
            int i;
            ubyte tempbyte;
            for (i = 0; i < r_length; i++) {
                sscanf(&record[pos], "%2hhx", &tempbyte);
                source_name[i] = (char)tempbyte;
                checksum += tempbyte & 0xFF;
                pos += 2;
            }
            source_name[i]=0;
        } else if (strcasecmp(r_name, "S1") == 0) {
            int i;
            uword tempbyte;
            for (i = 0; i < r_length; i++) {
                sscanf(&record[pos], "%2hx", &tempbyte);
                Bus(r_addr, &tempbyte, WRITE, BYTE);
                checksum += tempbyte & 0xFF;
                r_addr += 1;
                pos += 2;
            }
        } else if (strcasecmp(r_name, "S9") == 0) {
            starting_addr = r_addr;
        } else {
            printf("Warning: File does not contain S-record\n");
            return false;
        }
        sscanf(&record[pos], "%2hhx", &r_checksum);
        checksum += r_checksum;
        if (checksum != LOWBYTE) {
            printf("Warning: Incorrect checksum %s\n", record);
            return false;
        }
    }
    fclose(XME);
    printf("Source filename: %s\n", source_name);
    printf("File read - no errors deteted. Starting address %hx\n",
           starting_addr);
    return true;
}


void SigintHandler(int p){
    ctrl_c_catcher = true;
    signal(SIGINT, SigintHandler); //(_crt_signal_t) cause me problem
}


void Bus(unsigned short mar, unsigned short *mbr, enum ACTION rw,
         enum SIZE bw) {
    if (rw == READ) {
        if (bw == WORD) {
            *mbr = memory_stack[mar] + memory_stack[mar + 1] * BYTESHIFT;
        } else
            *mbr = memory_stack[mar];

    } else {
        if (bw == WORD) {
            memory_stack[mar] = *mbr % BYTESHIFT;
            memory_stack[mar + 1] = *mbr / BYTESHIFT;
        } else
            memory_stack[mar] = *mbr;
    }
    cpu_cycle[MEMACC_C]+=3;
}
