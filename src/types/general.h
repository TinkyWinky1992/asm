#ifndef GENERAL_H
#define GENERAL_H
#include <stdio.h>
#include <stdlib.h>
#include "./symbol.h"
typedef struct opcode 
{
    
    char *name;
    int opcodeValue;
    char binary[5];
    int isOneOp;
    int validMathodsSrc[6];
    int validMathodsDist[6];

}Opcode;
 
typedef struct operand
{
    char *name;
    int value;    
    char *binary;
    int isnumber;
  

} Operand;


typedef struct Instruction {

    char *name;
    Opcode opcode;
    char *src_label;
    char *dist_label;
    Operand src;
    Operand dist;
    int lineNumber;

} Instruction;


typedef struct MainMemory
{
    Instruction *inst;
    Symbol *symbol;
    int maxDC;
    int maxCI;
    int maxMemory;

}MainMemory;

#endif

