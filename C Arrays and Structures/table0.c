/*
 - table0.c
 - Create an array of strings
 - 30 May 20
 - lh
*/
#include <stdio.h>

char* xm2[] = { "SPLn", "CLR.W", "PUSH", "BEQ", NULL };

void table0()
{
int i;
i = 0;

while (xm2[i])
{
    printf("%s\n", xm2[i]);
    i++;
}

getchar();
}
