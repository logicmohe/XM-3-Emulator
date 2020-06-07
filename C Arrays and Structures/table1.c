/*
 - table1.c
 - Create two "parallel" arrays of strings
 - 30 May 20
 - lh
*/
#include <stdio.h>

char *xm2a[] = { "SPL0", "SPL1", "CLR.W", "PUSH", "BEQ", NULL };
char *xm3[] = {"SETPSW #0", "SETPSW #1", "MOV #0,", "ST", "CEX EQ,#1,#0", NULL};

void table1()
{
int i;
i = 0;

while (xm2a[i])
{
    printf("%s -> %s\n", xm2a[i], xm3[i]);
    i++;
}

getchar();
}
