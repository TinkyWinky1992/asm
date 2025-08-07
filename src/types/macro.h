#ifndef MACRO_H
#define MACRO_H
#include <stdio.h>
#include <stdlib.h>

#define MAX_MACRO_NAME 32
#define MAX_MACROS 100

typedef struct 
{
    char name[MAX_MACRO_NAME];
    char ** lines;
    int lineCounter;
    int capacity;
} Macro;

#endif