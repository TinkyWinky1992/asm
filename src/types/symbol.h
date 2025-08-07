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
    char name[MAX_LABEL_LEN]; // לדוגמה: "MAIN"
    int address;              // כתובת בזיכרון (IC או DC)
    SymbolType type;          // סוג הסמל
    int isExternal;           // 1 אם מוגדר כ .extern
    int isEntry;              // 1 אם הוגדר כ .entry
} Symbol;
#endif