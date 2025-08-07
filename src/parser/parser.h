#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include "../types/symbol.h"
#include "../types/macro.h"
#include "../types/general.h"

#define MAX_MEMORY 256
#define MAX_MEMORY_CI_CD 128
#define MAX_DC_INDEX 128
#define MAX_CI_INDEX 256
#define MAX_SYMBOL 100
#define MAX_OPCODE 15
#define MAX_OPERAND 9


extern Symbol DC_memory[MAX_MEMORY_CI_CD];
extern Instruction CI_memory[MAX_MEMORY_CI_CD];
extern int DC_index;
extern int CI_index;
extern MainMemory memory;

//Declare operands table
extern Operand operands_table[MAX_OPERAND];

//Declare opcodes table
extern Opcode opcode_table[MAX_OPCODE];

//Declare macro table
extern Macro macro_table[MAX_MACROS];
extern char *symbolTypes_table[5];
extern int macrocounter;
extern int flagEn;
extern int flagSnt;

//Function prototypes
int isSymbol(char* word);
int isCommand(char* word);
int isLabel(char* word);
int isMacro(char *word, FILE *file, char *line);
void printMacroTable();
void printInstructionTable();
#endif

