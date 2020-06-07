/*
 - table2.c
 - Create a structure of the two arrays
 - 30 May 20
 - lh
*/
#include <stdio.h>

struct conv_data
{
char* xm2;
char* xm3;
};

struct conv_data convtbl[] = {
{"SPL0", "SETPSW #0"}, 
{"SPL1", "SETPSW #1"},
{"CLR.W", "MOV #0,"},
{"PUSH", "ST"},
{"BEQ", "CEX EQ,#1,#0"},
{NULL}
};

void table2()
{
int i;
i = 0;

while (convtbl[i].xm2)
{
    printf("%s -> %s\n", convtbl[i].xm2, convtbl[i].xm3);
    i++;
}
getchar();

}
