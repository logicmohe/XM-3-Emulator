#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define RECORDLENGTH 255
#define INSTLENGTH 15
#define NOINST "XXXNOINST"
#define NOEXIST "XXXNOEXIST"

enum {NOT_MATCH = 0, ONE_INST=1, TWO_INST=2, INST_OPER=3, INST_LHOPER=4,INST_RHOPER=5};

char xm2_one_inst[][INSTLENGTH]= {"SPL0", "SPL1", "SPL2", "SPL3", "SPL4", "SPL5", "SPL6", "SPL7", "CLC", "CLN", "CLZ", "CLV", "SEC", "SEN", "SEZ", "SEV", "RET", NOINST};
char xm3_one_inst[][INSTLENGTH]= {"SETPRI #1", "SETPRI #1", "SETPRI #1", "SETPRI #1", "SETPRI #1", "SETPRI #1", "SETPRI #1", "SETPRI #1", "CLRCC C", "CLRCC N", "CLRCC Z", "CLRCC V", "SETCC C", "SETCC N", "SETCC Z", "SETCC V", "MOV R5,R7", NOINST};

char xm2_two_inst[][INSTLENGTH]= {"BEQ", "BZ", "BNE", "BNZ", "BGE", "BLT", "BC", "BNC", "BN", NOINST};
char xm3_two_inst[][INSTLENGTH]= {"EQ", "EQ", "NE", "NE", "GE", "LT", "CS", "CC", "MI", NOINST};

char xm2_inst_oper[][INSTLENGTH]= {"CALL", NOINST};
char xm3_inst_oper[][INSTLENGTH]= {"CALL", NOINST};

char xm2_inst_lhoper[][INSTLENGTH]= {"PUSH", "JUMP", NOINST};
char xm3_inst_lhoper[][INSTLENGTH]= {"ST", "MOV", NOINST};

char xm2_inst_rhoper[][INSTLENGTH]= {"PULL", "CLR.B", "CLR", "CLR.W", NOINST};
char xm3_inst_rhoper[][INSTLENGTH]= {"LD", "MOV", "MOVLZ", "MOVLZ", NOINST};

typedef struct Match_Struct{
    int* type;
    int* count;
};Match


// *MAIN*
int main(){

    FILE * xm3 = fopen("A1_solution.asm","w+");
    FILE * xm2 = fopen("A1_tests.asm","r");
    
    // CREATE a Branch Array *Stores the label of branches*

    // *Convert XM-2 instructions to XM-3 instructions*
    char record[RECORDLENGTH];
    // READ first record from XM-2 input file
    while(fgets(record, sizeof record, xm2)!=NULL){
        char * temp_record= （char*）malloc(strlen(record)+1);
        char * first_token = strtok(temp_record,"\t \n");
        char * second_token = strtok(temp_record,"\t \n");
        char * third_token = strtok(temp_record,"\t \n");
        char * fourth_token = strtok(temp_record,"\t \n");

        Match match_xm2;
        XM2Check(first_token,match_xm2);
        switch (*match_xm2.type){
        case ONE_INST:
            char*new_record=Translate(match_xm2, NOEXIST, first_token, NOEXIST, second_token?second_token:NOEXIST);
            fprintf(xm3,"%s",new_record);
            continue;
        case TWO_INST:
            char*new_record=Translate(match_xm2, NOEXIST, first_token, second_token?second_token:NOEXIST, third_token?third_token:NOEXIST);
            fprintf(xm3,"%s",new_record);
            continue;
        case INST_OPER:
            char*new_record=Translate(match_xm2, NOEXIST, first_token, second_token?second_token:NOEXIST, third_token?third_token:NOEXIST);
            fprintf(xm3,"%s",new_record);
            continue;
        case INST_LHOPER:
            char*new_record=Translate(match_xm2, NOEXIST, first_token, second_token?second_token:NOEXIST, third_token?third_token:NOEXIST);
            fprintf(xm3,"%s",new_record);
            continue;    
        case INST_RHOPER:
            char*new_record=Translate(match_xm2, NOEXIST, first_token, second_token?second_token:NOEXIST, third_token?third_token:NOEXIST);
            fprintf(xm3,"%s",new_record);
            continue;                                                                        
        default:
            break;
        }
        XM2Check(second_token);
        switch (*match_xm2.type)
        {
        case ONE_INST:
            char*new_record=Translate(match_xm2, first_token, second_token, NOEXIST, third_token?third_token:NOEXIST);
            fprintf(xm3,"%s",new_record);
            continue;
        case TWO_INST:
            char*new_record=Translate(match_xm2, first_token, second_token, third_token, fourth_token?fourth_token:NOEXIST);
            fprintf(xm3,"%s",new_record);
            continue;
        case INST_OPER:
            char*new_record=Translate(match_xm2, first_token, second_token, third_token, fourth_token?fourth_token:NOEXIST);
            fprintf(xm3,"%s",new_record);
            continue;
        case INST_LHOPER:
            char*new_record=Translate(match_xm2, first_token, second_token, third_token, fourth_token?fourth_token:NOEXIST);
            fprintf(xm3,"%s",new_record);
            continue;    
        case INST_RHOPER:
            char*new_record=Translate(match_xm2, first_token, second_token, third_token, fourth_token?fourth_token:NOEXIST);
            fprintf(xm3,"%s",new_record);
            continue;                                                                        
        default:
            break;
        }
        fprintf(xm3,"%s",record);
        // CALL TRANSLATE and PASS 1st & 2nd & 3rd (IF exists) & 4th (IF exists) token as Lbl & Inst & Oper & Comm token
        // 	ELSE
        // 		WRITE current record into XM-3 output file
        // 	ENDIF
        // 	READ next record from XM-2 input file
    }
    fclose(xm2);


    // *Replace BRA with specific branch*
    // READ first record from XM-3 file
    // WHILE (Not reach the end of file) DO
    // 	WHILE (Not reach the end of line) DO
    // GET tokens through parsing record by blank space OR tab 
    // NAME as 1st token, 2nd token (if exists), 3rd token (IF exists), 4th token (IF exists)
    // 	END WHILE
    // 	IF 1st token matches any items in Branch Array
    // 		GO THROUGH till the end of file SERACH for “BRA” + 1st token
    // 		REPLACE that record with the content of this label
    // 	READ next record from XM-3 file
    // END WHILE
    fclose(xm3);
}


void XM2Check(char *inst_token, Match match_xm2){
    int i;
    for(i=0; strcmp(xm2_one_inst[i],NOINST)!=0;i++){
        if(!strcmp(xm2_one_inst[i],inst_token)){
            *match_xm2.type=ONE_INST;
            *match_xm2.count=i;
            return;
        }
    }
    for(i=0; strcmp(xm2_two_inst[i],NOINST)!=0;i++){
        if(!strcmp(xm2_two_inst[i],inst_token)){
            *match_xm2.type=ONE_INST;
            *match_xm2.count=i;
            return;
        }
    }
    for(i=0; strcmp(xm2_inst_oper[i],NOINST)!=0;i++){
        if(!strcmp(xm2_inst_oper[i],inst_token)){
            *match_xm2.type=ONE_INST;
            *match_xm2.count=i;
            return;
        }
    }
    for(i=0; strcmp(xm2_inst_lhoper[i],NOINST)!=0;i++){
        if(!strcmp(xm2_inst_lhoper[i],inst_token)){
            *match_xm2.type=ONE_INST;
            *match_xm2.count=i;
            return;
        }
    }        
    for(i=0; strcmp(xm2_inst_rhoper[i],NOINST)!=0;i++){
        if(!strcmp(xm2_inst_rhoper[i],inst_token)){
            *match_xm2.type=ONE_INST;
            *match_xm2.count=i;
            return;
        }
    }
    return;
}

char * Translate(Match match_xm2, char * lbl_token, char* inst_token, char* oper_toekn,char*comm_token){
    char * new_record;
    switch (*match_xm2.type){
    case ONE_INST:
        
        break;
    case TWO_INST:

        break;
    case INST_OPER:

        break;
    case INST_LHOPER:

        break;    
    case INST_RHOPER:

        break;                                                                        
    default:
        break;
    }
}
// *TRANSLATE*
// IF Inst token MATCH Legacy Pri Inst list AND Oper token doesn’t exist
// FROM Legacy Pri Inst FIND corresponding token in New Pri Inst 
// CREATE new record as the form:(Lbl token) + token in New Pri Inst + (Comm token)

// IF Inst token MATCH Legacy Bit Inst list AND Oper token exists
// FROM Legacy Bit Inst FIND corresponding token in New Bit Inst 
// CREATE new record as the form:(Lbl token) + token in New Bit Inst + (Comm token)

// ELSE IF Inst token MATCH Legacy Branch Inst list
// FROM Legacy Branch Inst FIND corresponding token in New Branch Inst
// CREATE new record as the form:	
// “CEX” + token in New Branch Inst,#1,#0 
// “BRA” + Oper token + (Comm token)
// 	STORE Oper token in Branch Array

// ELSE IF Inst token MATCH Legacy Other Inst list*
// 	IF Oper token does not exist 
// 		IF Inst token same as “RET”
// CREATE new record as the form: (Lbl token) + “MOV” + R5,R7 + (Comm token)	
// 		ELSE
// 			ISSUE a Warning
// 			BREAK
// 	ELSE IF Inst token same as “CALL”
// CREATE new record as the form: (Lbl token) + “BL” + Oper token + (Comm token)
// ELSE IF Inst token same as “PULL”
// CREATE new record as the form: (Lbl token) + “LD” + R6+,Oper token + (Comm token)

// 	ELSE IF Inst token same as “PUSH”
// CREATE new record as the form: (Lbl token) + “ST” + Oper token,-R6 + (Comm token)
// 	ELSE IF Inst token same as “JUMP”
// CREATE new record as the form: (Lbl token) + “MOV” + Oper token,R7 + (Comm token)
// 	ELSE IF Inst token same as “CLR.B”
// CREATE new record as the form: (Lbl token) + “MOVL” + #0,Oper token + (Comm token)
// 	ELSE IF Inst token same as “CLR” OR “CLR.W”
// CREATE new record as the form: (Lbl token) + “MOVLZ” + #0,Oper token + (Comm token)
// ELSE
// 			ISSUE a Warning
// 			BREAK
// 	ENDIF
// ELSE
// 		ISSUE a Warning
// 		BREAK
// ENDIF
// WRITE new record into XM-3 output file
// EXIT








// Directive = [ALIGN | BSS | BYTE | END | EQU | ORG | WORD]
// Operand = Value + 0{“,” + Operand}3 
// Value = [Numeric | Label]
// Comment = * Start with “;” Text associated with the record – ignored by the assembler*
// Token = [Label | Instruction | Directive | Operand |; Comment]
// Warning = *An indicator shows the argument is missing in a record*
// Register = [R0 | R1 | R2 | R3 | R4 | R5 | R6 | R7]

// *Instruction: *
// XM3 Instruction Set= [BL | BRA | CEX | SETPRI | SVC | SETCC | CLRCC | ADD | ADDC | SUB | SUBC | DADD | CMP | XOR | AND | BIT | BIC | BIS | MOV | SWAP | SRA | RRC | SWPB | SXT | LD | ST | SVC | MOVL | MOVLZ | MOVLS | MOVLH | LDR | STR]

// Legacy Pri Inst = [SPL0 | SPL1 | SPL2 | SPL3 | SPL4 | SPL5 | SPL6 | SPL7]
// New Pri Inst=[SETPRI #0 | SETPRI #1 | SETPRI #2 | SETPRI #3 | SETPRI #4 | SETPRI #5| SETPRI #6 | SETPRI #7]
// Legacy Bit Inst = [SEV | SEN | SEZ | CLC | CLV | CLN | CLZ]
// New Branch Inst = [CLRCC C | CLRCC N | CLRCC Z | CLRCC V | SETCC C | SETCC N  | SETCC Z | SETCC V ]
// Legacy Branch Inst = [BEQ | BZ | BNE | BNZ | BGE | BC | BNC | BN]
// New Branch Inst = [EQ | NE | GE | LT | CS | CC | MI ]
// Legacy Other Inst = [CALL | PULL | PUSH | RET | JUMP | CLR.B | CLR | CLR.W]
// New Other Inst = [BL | LD | ST | MOV | MOVL | MOVLZ]
