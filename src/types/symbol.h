#ifndef SYMBOL_H
#define SYMBOL_H
#include <stdio.h>
#include <stdlib.h>
#define MAX_LABEL_LEN 32

typedef enum {
    SYMBOL_CODE,
    SYMBOL_DATA,
    SYMBOL_ENTRY,
    SYMBOL_EXTERN,
    SYMBOL_STRING,
    SYMBOL_METRIX,
} SymbolType;


typedef struct {
    char *name;
    int address;              
    SymbolType type;          
    int isExternal;       
    int isEntry;           
    char *values;      
} Symbol;
#endif


