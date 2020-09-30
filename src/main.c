/* File: main.c
 * Author: Youwei Zhang
 * School: Dalhousie University
 * Dept: Electrical and Computer Engineering
 * Course: ECED 3403 Computer Architecture
 *
 * Purpose：    Main program, get command from console or debugging file
 * 
 * Last Modified: 2020.7.29
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "global.h"
#include "function.h"

/* Initialize Register */
uword break_point = 0xFFFF;
uword inst_reg;
uword starting_addr;        //Address that program starts execution
uword Register[REGNUM]={0};

/* Initialize Ctrl C catcher */
volatile sig_atomic_t ctrl_c_catcher;

/* Initialize Memory Stack */
ubyte memory_stack[MEMSIZE]={0};

/* Initialize CPU cycle */
int cpu_cycle[CYCLENUM]={0};

int main(int argc, char **argv) {
    /* Check Input parameter */
    FILE *DBG = NULL;
    bool debug_on = false;
    
    if ((argc == 3) && (strcmp(argv[1], "-d") == 0)) {
        DBG = fopen(argv[2], "r");
        debug_on = true;
    } else if (argc != 1) {
        printf("Invalid Input, please try again\n");
        return 0;
    }

    /* Start Debugger */
    while (1) {
        /* Initialize Ctrl C catcher */
        ctrl_c_catcher = false;
        signal(SIGINT, SigintHandler);

        /* Get Input Command */
        printf("Option: ");
        char command;
        if (debug_on) {
            command = fgetc(DBG);
            if(command=='\n') command=fgetc(DBG);
            printf("%c\n", command);
        } else
            scanf(" %c", &command);

        switch (command) {
            /* Load XME File */
            case 'l':
            case 'L':
                printf("Enter .XME file to load\n");
                char file_name[NAMESIZE]; //Input xme file
                if (debug_on) {
                    fscanf(DBG, "%s", file_name);
                    printf("%s\n", file_name);
                } else
                    scanf("%s", file_name);
                if (Loader(file_name) == false) continue; //Validate input file
                break;

            /* Dump Memory */
            case 'd':
            case 'D': 
                printf("Enter lower and upper bound\n");
                uword low_bound, high_bound;
                if (debug_on) {
                    fscanf(DBG, "%hx %hx", &low_bound, &high_bound);
                    printf("%hx %hx\n", low_bound, high_bound);
                } else
                    scanf("%hx %hx", &low_bound, &high_bound);
                MemDump(low_bound,high_bound);
                break;
            
            /* Add Break Point */
            case 'b':
            case 'B':
                printf("Enter the break point\n");
                if (debug_on) {
                    fscanf(DBG, "%hx", &break_point);
                    printf("%hx\n", break_point);
                } else
                    scanf("%hx",&break_point);
                break;

            /* Run Program */
            case 'g':
            case 'G':
                printf("Starting running program\n");
                Register[PC] = starting_addr;
                inst_reg=0xFFFF;
                printf("Start: PC: 0x%4hx; Break Point: 0x%4hx\n", Register[PC], break_point);
                while((!ctrl_c_catcher) && (Register[PC] != break_point) && (inst_reg!=0)){
                    Fetch();
                    Decode();
                    Execute();
                }
                printf("End: PC: 0x%4hx\n", Register[PC]);
                break;

            /* Print Register */
            case 'r':
            case 'R':{
                int i;
                for(i=0;i<REGNUM;i++){
                    printf("R%d:  0x%04hx\n",i,Register[i]);
                }
                }
                break;

            /* End Program */
            case 'x':
            case 'X':
                if (DBG) {fclose(DBG);}
                return 0;
                break;

            default:
                printf("Warning: Wrong Command\n");
        }
    }
    return 0;
}
