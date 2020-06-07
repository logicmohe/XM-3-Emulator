/*
 - table3.c
 - Create a structure of the two arrays
 - 30 May 20
 - lh
*/
#include <stdio.h>

struct conv_data2
{
char* xm2;
char* xm3;
char* operand;
};

struct conv_data2 convtbl2[] = {
{"SPL0", "SETPSW #0", NULL}, {"SPL1", "SETPSW #1", NULL},
{"CLR.W", "MOV #0,", NULL},
{"PUSH", "ST", ",-R6"},
{"BEQ", "CEX EQ,#1,#0", NULL},
{NULL}
};

void table3()
{
int i;
i = 0;

while (convtbl2[i].xm2)
{
    printf("%s -> %s ", convtbl2[i].xm2, convtbl2[i].xm3);
    if (convtbl2[i].operand)
        printf("%s", convtbl2[i].operand);
    printf("\n");
    i++;
}
getchar();

}
