#ifndef UTILS_H
#define UTILS_H

#include <ctype.h>
#include "./types/symbol.h"
void toLowerCase(char* str);
int SkippingSpaces(int start, char* line);
int isAlphaOrNum(char c);
int isAlpha(char c);
void extractMcroName(char *line, char *macroName);
void skipping_label(char *line, char *word);
int isOperandValid(char *var);
char **split_instruction_opcode(const char *line);
char **split_instruction_symbol(const char *line,int* outSize);
char* intToBinary(int num);
char* convertToBase4(int num);
SymbolType getSymbolType(const char *typeStr);
char * findSymbolname(char * var);
char *padTo10Bits(const char *binary);
char **ExtractBinaryValuesWithExtra(Instruction *p);
#endif // UTILS_H