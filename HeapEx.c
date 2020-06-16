#include <malloc.h>
#include <stdio.h>
#include <ctype.h>

/***************************************
 * Heap Example 1 - Allocating storage *
 ***************************************/

void HeapExample1()
{
/* Allocating memory from a heap */
int *iptr;  /* Integer pointer */
char *cptr; /* Char pointer */
/* 
 - Allocate storage using malloc()
 - Casting should be used (keeps compiler happy)
*/
iptr = (int*) malloc(sizeof(int));
cptr = (char*) malloc(sizeof(char));

/* Display addresses */
printf("iptr: 0x%08x cptr: 0x%08x\n", 
    (unsigned int)iptr, (unsigned int)cptr);

/* Assign values to lvalues (dereferencing to malloc()'d storage */
*iptr = 3;
*cptr = 'A';

/* Check by dereferencing the pointers */
printf("*iptr: %d *cptr: %c\n", *iptr, *cptr);

/* Manipulation using rvalues and lvalues */
*iptr = (*iptr)++;
*cptr = *cptr + 1;

/* Check */
printf("*iptr: %d *cptr: %c\n", *iptr, *cptr);

/* Return storage */
free(iptr);
free(cptr);
getchar();
}

/*******************************************
 * Heap Example 2 - creating a linked list *
 *******************************************/

/* Node in linked list */
typedef struct item
{
int value;
struct item* next_item;
} Item;

Item* IList; /* Pointer to Master list of items */

Item* get_item()
{
/* 
 - Create an item for a linked list
 - Return ptr to allocated Item if +ve data or NULL
*/
Item* new_item;
int data;
printf("Another integer? (-ve: stop adding)");
scanf("%d", &data);

if (data < 0)
    return NULL; /* Stop */

/* Allocate a new Item - NULL indicates no memory */
new_item = (Item*) malloc(sizeof(Item));

if (new_item == NULL)
    return NULL; /* No memory available */
    
new_item->value = data;
new_item->next_item = NULL;
return new_item; /* Non-negative value */
}

void HeapExample2()
{
/*
 - Build a linked list of Items and then display it
*/
Item* next_item;
IList = NULL; /* Default empty list */

/* Build the list */
while (next_item = get_item())
{
    /* Notice the last item points to NULL */
    next_item->next_item = IList;
    IList = next_item;
}
/* No more items, print list */
next_item = IList; 

/* 
 - next_item is a pointer (address)
 - Stay in loop as long as next_item is not NULL
 - Since NULL = 0x00..00, loop will stop on last item
*/
while (next_item) /* Print list */
{
    printf("%d ", next_item->value);
    /*
     - Move pointer to next item in list
     - next_item is a pointer (address) of the next Item structure
    */
    next_item = next_item->next_item;
}

/* Exit condition: next_item == NULL */
printf("\n");

getchar();
}

/*************************
 * NUL character example *
 *************************/

#define NUL '\0'
char String[] = "The cat appeared on youtube!";

void catvideo()
{
/* Example of dereferencing a char pointer */
char* cptr;

/* Print the string */
printf("%s\n", String);

/* 
 - Assign string to pointer
 - String refers to an address (location containing 'T')
*/
cptr = String;

/* 
 - Example of dereferencing a pointer:
    - rvalue returns the contents of the address specified by *cptr
    - lvalue stores the rvalue in the address specified by *cptr
*/
while (*cptr != NUL)
{
    *cptr = toupper(*cptr);
    /*
     - Move through array by increasing pointer value
     - Since cptr points to (i.e., the address of) a char, it increases by 1 (byte)
    */
    cptr++;
}

printf("%s\n", String);
getchar();
}
