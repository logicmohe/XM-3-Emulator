/* File: preprocessor.c
 * Author: Youwei Zhang
 * School: Dalhousie University
 * Dept: Electrical and Computer Engineering
 * Course: ECED 3403 Computer Architecture
 * 
 * Purpose： This file is the preprocessor that translates legacy XM2 instructions into
 *           XM3 instructions, then output an XM3 and report all error 
 * Last Modified: 2020.5.30
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define RECORDLENGTH 255
#define TOKENLENGTH 15
#define TABLECONVERTS 35
#define TABLECOLUMN 4
#define NOMORE "XXXNOMORE"

enum
{
    NOEXIST = -3,
    COMMTKN = -2,
    NOMATCH = -1,
    ONE_INST = 0,
    TWO_INST = 1,
    INST_OPER = 2,
    INST_LHOPER = 3,
    INST_RHOPER = 4
};

struct Convert_Struct
{
    char _legacy_inst[TOKENLENGTH];
    int _type;
    char _new_inst[TOKENLENGTH];
    char _new_oper[TOKENLENGTH];
};
struct Convert_Struct convert_table[TABLECONVERTS] = {
    {"SPL0", ONE_INST, "SETPRI", "#0"}, {"SPL1", ONE_INST, "SETPRI", "#2"}, {"SPL2", ONE_INST, "SETPRI", "#3"}, {"SPL3", ONE_INST, "SETPRI", "#3"}, {"SPL4", ONE_INST, "SETPRI", "#4"}, {"SPL5", ONE_INST, "SETPRI", "#5"}, {"SPL6", ONE_INST, "SETPRI", "#6"}, {"SPL7", ONE_INST, "SETPRI", "#7"}, {"CLC", ONE_INST, "CLRCC", "C"}, {"CLN", ONE_INST, "CLRCC", "N"}, {"CLZ", ONE_INST, "CLRCC", "Z"}, {"CLV", ONE_INST, "CLRCC", "V"}, {"SEC", ONE_INST, "SETCC", "C"}, {"SEN", ONE_INST, "SETCC", "N"}, {"SEZ", ONE_INST, "SETCC", "Z"}, {"SEV", ONE_INST, "SETCC", "V"}, {"RET", ONE_INST, "MOV", "R5,R7"}, {"BEQ", TWO_INST, "CEX", "EQ,#1,#0"}, {"BZ", TWO_INST, "CEX", "EQ,#1,#0"}, {"BNE", TWO_INST, "CEX", "NE,#1#0"}, {"BNZ", TWO_INST, "CEX", "NE,#1,#0"}, {"BGE", TWO_INST, "CEX", "GE,#1,#0"}, {"BLT", TWO_INST, "CEX", "LT,#1,#0"}, {"BC", TWO_INST, "CEX", "CS,#1,#0"}, {"BNC", TWO_INST, "CEX", "CC,#1,#0"}, {"BN", TWO_INST, "CEX", "MI,#1,#0"}, {"CALL", INST_OPER, "BL", NOMORE}, {"PUSH", INST_LHOPER, "ST", "-R6"}, {"JUMP", INST_LHOPER, "MOV", "R7"}, {"PULL", INST_RHOPER, "LD", "R6+"}, {"CLR.B", INST_RHOPER, "MOVL", "#0"}, {"CLR", INST_RHOPER, "MOVLZ", "#0"}, {"CLR.W", INST_RHOPER, "MOVLZ", "#0"}, {NOMORE, NOMATCH, NOMORE, NOMORE}};

void Translate(FILE *xm3, int legacy_num, char *oper_token);
int XM2Check(char token[]);

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("Invalid Input, please try again\n");
        return 0;
    }

    FILE *xm3 = fopen("solution.asm", "w+");
    FILE *xm2 = fopen(argv[1], "r");
    char record[RECORDLENGTH];

    while (fgets(record, sizeof record, xm2) != NULL)
    { // READ record from XM-2 input file
        char *first_token = strtok(record, "\t\n ");
        int legacy_num = XM2Check(first_token);
        if (legacy_num > NOMATCH)
        { //If first token is INST Token
            char *second_token = strtok(NULL, "\t\n ");
            Translate(xm3, legacy_num, second_token ? second_token : NOMORE);
        }
        else if (legacy_num == NOMATCH)
        {
            fprintf(xm3, "%s ", first_token);
            char *second_token = strtok(NULL, "\t\n ");
            legacy_num = XM2Check(second_token);
            if (legacy_num > NOMATCH)
            { //If second token is INST token
                char *third_token = strtok(NULL, "\t\n ");
                Translate(xm3, legacy_num, third_token ? third_token : NOMORE);
            }
            else if (legacy_num > NOEXIST) //Avoid (NULL) in solution file
                fprintf(xm3, "%s", second_token);
        }
        else if (legacy_num == COMMTKN)
            fprintf(xm3, "%s", first_token);

        char *token = strtok(NULL, "\t\n ");
        while (token != NULL)
        {
            fprintf(xm3, " %s", token);
            token = strtok(NULL, "\t\n ");
        }
        fprintf(xm3, "\n");
    }
}

/*
 * Function: XM2Check
 * ------------------
 * Check whether the input instruction is legacy XM2 instruction
 * 
 * token[]: Input instruction toke
 * 
 * returns: the index of give instruction in convert_table
 *          returns NOEXIST if it's a comment or NULL
 *          returns NOMATCH if it's not match the instruciton in convert_table
 */
int XM2Check(char token[])
{
    if (token == NULL)
        return NOEXIST;
    else if (token[0] == ';')
        return COMMTKN;
    int length = strlen(token);
    int i;
    char temp[length];
    strcpy(temp, token);
    for (i = 0; i < length; i++)
    {
        temp[i] = toupper(temp[i]);
    }
    for (i = 0; i < TABLECONVERTS; i++)
    {
        if (!strcmp(convert_table[i]._legacy_inst, temp))
            return i;
    }
    return NOMATCH;
}

/*
 * Function: Translate
 * -------------------
 * Translate the legacy record to xm3 module record, write new record into output  file and show 
 * error message if oper_token is missing
 * 
 * xm3: Output file
 * legacy_num: the index of insrtuction in convert_table
 * oper_token: the token after instruction, oper token if instruction is not belong to ONE_INST
 * 
 * returns: Void
 */
void Translate(FILE *xm3, int legacy_num, char *oper_token)
{
    switch (convert_table[legacy_num]._type)
    {
    case ONE_INST:
        fprintf(xm3, "%s %s", convert_table[legacy_num]._new_inst,
                convert_table[legacy_num]._new_oper);
        if (strcmp(oper_token, NOMORE) != 0)
            fprintf(xm3, " %s", oper_token);
        break;
    case TWO_INST:
        fprintf(xm3, "%s %s\n", convert_table[legacy_num]._new_inst,
                convert_table[legacy_num]._new_oper);
        if (!strcmp(oper_token, NOMORE) || oper_token[0] == ';')
            fprintf(xm3, "Mssing Legacy Operand");
        else
            fprintf(xm3, "BRA %s", oper_token);
        break;
    case INST_OPER:
        if (!strcmp(oper_token, NOMORE) || oper_token[0] == ';')
            fprintf(xm3, "Mssing Legacy Operand");
        else
            fprintf(xm3, "%s %s", convert_table[legacy_num]._new_inst,
                    oper_token);
        break;
    case INST_LHOPER:
        if (!strcmp(oper_token, NOMORE) || oper_token[0] == ';')
            fprintf(xm3, "Mssing Legacy Operand");
        else
            fprintf(xm3, "%s %s,%s", convert_table[legacy_num]._new_inst,
                    oper_token, convert_table[legacy_num]._new_oper);
        break;
    case INST_RHOPER:
        if (!strcmp(oper_token, NOMORE) || oper_token[0] == ';')
            fprintf(xm3, "Mssing Legacy Operand");
        else
            fprintf(xm3, "%s %s,%s", convert_table[legacy_num]._new_inst,
                    convert_table[legacy_num]._new_oper, oper_token);
        break;
    default:
        break;
    }
}
