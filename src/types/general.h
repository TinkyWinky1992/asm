#ifndef GENERAL_H
#define GENERAL_H
#include <stdio.h>
#include <stdlib.h>
#include "./symbol.h"
typedef struct opcode 
{
    
    char *name;
    int opcodeValue;
    char binary[4];
    int isOneOp;

}Opcode;
 
typedef struct operand
{
    char name[2];
    int value;    
    char binary[3];
  

} Operand;

typedef struct Instruction {

    char name[MAX_LABEL_LEN];
    Opcode opcode;
    Operand src;
    Operand dist;
    int lineNumber;

} Instruction;
#endif

typedef struct MainMemory
{
    Instruction *inst;
    Symbol *symbol;
    int maxDC;
    int maxCI;
    int maxMemory;

}MainMemory;



