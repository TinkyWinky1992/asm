#ifndef MACRO_H
#define MACRO_H
#include <stdio.h>
#include <stdlib.h>
#include "./general.h"

#define MAX_MACRO_NAME 32  // Maximum length for a macro's name (including null terminator)
#define MAX_MACROS 100     // Maximum number of macros allowed

/*
    Represents a macro definition in the assembler.

    A macro here stores:
    - name:          The macro's name (e.g., "PRINT_HELLO")
    - lines:         The raw source lines that belong to this macro (each line is a string)
    - lineCounter:   How many lines are currently stored in 'lines'
    - capacity:      How many lines 'lines' can currently hold before resizing
    - LineInst:      An array of pointers to parsed Instruction structures corresponding to each line
*/
typedef struct 
{
    char name[MAX_MACRO_NAME];  // Name of the macro
    char **lines;               // Array of strings (lines of the macro body)
    int lineCounter;            // Number of lines currently in the macro
    int capacity;               // Allocated capacity for lines
    Instruction **LineInst;     // Array of pointers to compiled Instruction objects

} Macro;

/*
    Example usage:

    Macro myMacro;
    strcpy(myMacro.name, "PRINT_HELLO");
    myMacro.lineCounter = 0;
    myMacro.capacity = 10;
    myMacro.lines = malloc(myMacro.capacity * sizeof(char *));
    myMacro.LineInst = malloc(myMacro.capacity * sizeof(Instruction *));

    // Adding a line to the macro
    myMacro.lines[myMacro.lineCounter++] = strdup("mov r1, #5");
    myMacro.lines[myMacro.lineCounter++] = strdup("prn r1");

    // Later, each line can be converted into an Instruction and stored in LineInst
*/

#endif
