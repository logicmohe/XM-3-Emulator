#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> //Can we use this? for toUpper

#define RECORDLENGTH 255
#define TOKENLENGTH 15
#define TABLECONVERTS 35
#define TABLECOLUMN 4
#define NOMORE "XXXNOMORE"

enum {
    NOEXIST = -2,
    NOMATCH = -1,
    ONE_INST = 0,
    TWO_INST = 1,
    INST_OPER = 2,
    INST_LHOPER = 3,
    INST_RHOPER = 4
};

struct Convert_Struct {
    char _legacy_inst[TOKENLENGTH];
    int _type;
    char _new_inst[TOKENLENGTH];
    char _new_oper[TOKENLENGTH];
};
struct Convert_Struct convert_table[TABLECONVERTS] = {
    {"SPL0", ONE_INST, "SETPRI", "#0"},    {"SPL1", ONE_INST, "SETPRI", "#2"},
    {"SPL2", ONE_INST, "SETPRI", "#3"},    {"SPL3", ONE_INST, "SETPRI", "#3"},
    {"SPL4", ONE_INST, "SETPRI", "#4"},    {"SPL5", ONE_INST, "SETPRI", "#5"},
    {"SPL6", ONE_INST, "SETPRI", "#6"},    {"SPL7", ONE_INST, "SETPRI", "#7"},
    {"CLC", ONE_INST, "CLRCC", "C"},       {"CLN", ONE_INST, "CLRCC", "N"},
    {"CLZ", ONE_INST, "CLRCC", "Z"},       {"CLV", ONE_INST, "CLRCC", "V"},
    {"SEC", ONE_INST, "SETCC", "C"},       {"SEN", ONE_INST, "SETCC", "N"},
    {"SEZ", ONE_INST, "SETCC", "Z"},       {"SEV", ONE_INST, "SETCC", "V"},
    {"RET", ONE_INST, "MOV", "R5,R7"},     {"BEQ", TWO_INST, "CEX", "EQ,#1,#0"},
    {"BZ", TWO_INST, "CEX", "EQ,#1,#0"},   {"BNE", TWO_INST, "CEX", "NE,#1#0"},
    {"BNZ", TWO_INST, "CEX", "NE,#1,#0"},  {"BGE", TWO_INST, "CEX", "GE,#1,#0"},
    {"BLT", TWO_INST, "CEX", "LT,#1,#0"},  {"BC", TWO_INST, "CEX", "CS,#1,#0"},
    {"BNC", TWO_INST, "CEX", "CC,#1,#0"},  {"BN", TWO_INST, "CEX", "MI,#1,#0"},
    {"CALL", INST_OPER, "BL", NOMORE},     {"PUSH", INST_LHOPER, "ST", "-R6"},
    {"JUMP", INST_LHOPER, "MOV", "R7"},    {"PULL", INST_RHOPER, "LD", "R6+"},
    {"CLR.B", INST_RHOPER, "MOVL", "#0"},  {"CLR", INST_RHOPER, "MOVLZ", "#0"},
    {"CLR.W", INST_RHOPER, "MOVLZ", "#0"}, {NOMORE, NOMATCH, NOMORE, NOMORE}};

void Translate(FILE *xm3, int legacy_num, char *oper_token);
int XM2Check(char token[]);

// In Linux Environment
// int main(int argc, char **argv){

int main() {
    FILE *xm3 = fopen("A1_solution.asm", "w+");
    FILE *xm2 = fopen("A1_tests.asm", "r");
    // *Convert XM-2 instructions to XM-3 instructions*
    char record[RECORDLENGTH];
    // READ first record from XM-2 input file
    while (fgets(record, sizeof record, xm2) != NULL) {
        char *first_token = strtok(record, "\t ");
        int legacy_num = XM2Check(first_token); 
        if (legacy_num > NOMATCH) {
            char *second_token = strtok(NULL, "\t ");
            Translate(xm3, legacy_num, second_token ? second_token : NOMORE);
        }
        else if (legacy_num == NOMATCH) {
            fprintf(xm3, "%s ", first_token);
            char *second_token = strtok(NULL, "\t ");
            legacy_num = XM2Check(second_token);
            if (legacy_num == NOEXIST) {
                char *third_token = strtok(NULL, "\t ");
                Translate(xm3, legacy_num, third_token ? third_token : NOMORE);
            } else
                fprintf(xm3, "%s ", second_token);
        }
        else fprintf(xm3, "%s", first_token);

        char *token = strtok(NULL, "\t ");
        while(token!=NULL){
            fprintf(xm3, "%s", token);
            token=strtok(NULL, "\t ");
        }
    }
}

int XM2Check(char token[]) {
    if (token == NULL || token[0]==';')  // NOTE: Is it necessary to check the
                                     // comment? In case ;abc clc abc
        return NOEXIST;
    int length = strlen(token);
    int i;
    for (i = 0; i < length; i++) {
        token[i] = toupper(token[i]);
    }
    for (i = 0; i < TABLECONVERTS; i++) {
        if (!strcmp(convert_table[i]._legacy_inst, token)) 
            return i;
    }
    return NOMATCH;
}

void Translate(FILE *xm3, int legacy_num, char *oper_token) {
    switch (convert_table[legacy_num]._type) {
        case ONE_INST:  // NOTE: HOw to get an Extract Token function
            fprintf(xm3, "%s %s ", convert_table[legacy_num]._new_inst,
                    convert_table[legacy_num]._new_oper);
            break;
        case TWO_INST:
            fprintf(xm3, "%s %s\n", convert_table[legacy_num]._new_inst,
                    convert_table[legacy_num]._new_oper);
            if (!strcmp(oper_token, NOMORE))
                fprintf(xm3, "Mssing Legacy Operand");
            else
                fprintf(xm3, "BRA %s", oper_token);
            break;
        case INST_OPER:
            if (!strcmp(oper_token, NOMORE))
                fprintf(xm3, "Mssing Legacy Operand");
            else
                fprintf(xm3, "%s %s", convert_table[legacy_num]._new_inst,
                        oper_token);
            break;
        case INST_LHOPER:
            if (!strcmp(oper_token, NOMORE))
                fprintf(xm3, "Mssing Legacy Operand");
            else
                fprintf(xm3, "%s %s,%s", convert_table[legacy_num]._new_inst,
                        oper_token, convert_table[legacy_num]._new_oper);
            break;
        case INST_RHOPER:
            if (!strcmp(oper_token, NOMORE))
                fprintf(xm3, "Mssing Legacy Operand");
            else
                fprintf(xm3, "%s %s,%s", convert_table[legacy_num]._new_inst,
                        convert_table[legacy_num]._new_oper, oper_token);
            break;
        default:
            break;
    }
}
