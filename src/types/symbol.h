#ifndef SYMBOL_H
#define SYMBOL_H
#include <stdio.h>
#include <stdlib.h>

#define MAX_LABEL_LEN 32  // Maximum length of a label name (including null terminator)

/*
 * SymbolType:
 * Represents the type/category of a symbol in the assembler.
 * This is useful for distinguishing between labels for code, data, externals, etc.
 */
typedef enum {
    SYMBOL_CODE,    // A label that refers to a code (instruction) address
    SYMBOL_DATA,    // A label that refers to a data (variable) address
    SYMBOL_ENTRY,   // A label marked as 'entry' for linking
    SYMBOL_EXTERN,  // A label defined outside this file/module
    SYMBOL_STRING,  // A label that points to a string value
    SYMBOL_METRIX,  // A label that points to a matrix or array
} SymbolType;

/*
 * Symbol:
 * Represents a label or named memory location in the assembler.
 *
 * Fields:
 *  name       - The label name (e.g., "LOOP", "START", "myVar").
 *  address    - Memory address or location counter for the symbol.
 *  type       - The type of the symbol (from SymbolType enum).
 *  isExternal - 1 if symbol is defined externally, otherwise 0.
 *  isEntry    - 1 if symbol is marked as entry, otherwise 0.
 *  values     - Optional string or data values associated with the symbol.
 */
typedef struct {
    char *name;         // Symbol's name (allocated dynamically or as a constant)
    int address;        // Memory address the symbol points to
    SymbolType type;    // What kind of symbol this is
    int isExternal;     // Flag for external definition
    int isEntry;        // Flag for entry definition
    char *values;       // For storing associated data (string, numbers, etc.)
} Symbol;

/*
 * Example:
 *
 * Symbol mySymbol = {
 *     .name = "LOOP",
 *     .address = 100,
 *     .type = SYMBOL_CODE,
 *     .isExternal = 0,
 *     .isEntry = 1,
 *     .values = NULL
 * };
 *
 * This means:
 *  - The label "LOOP" points to address 100 in code memory.
 *  - It is an ENTRY label.
 *  - It is not external.
 */
#endif
