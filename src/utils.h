#ifndef UTILS_H
#define UTILS_H

#include <ctype.h>
#include "./types/symbol.h"
void toLowerCase(char* str);//lower case the string Form ABC -> abc
int SkippingSpaces(int start, char* line);//skipping spaces in the line
int isAlphaOrNum(char c);//check chracter a-z and 0-9
int isAlpha(char c);// check character a-z
void extractMcroName(char *line, char *macroName);// extract mcro name from the line
void skipping_label(char *line, char *word); // skipping label if needed
int isOperandValid(char *var);// check if operand exsits in the operand table
char **split_instruction_opcode(const char *line); // split instucrtion of opcode
char **split_instruction_symbol(const char *line,int* outSize); // split insturciton of symbol line
char* intToBinary(int num);// get integer and convert it to binray
char* convertToBase4(int num); // convert to base 4 
SymbolType getSymbolType(const char *typeStr);/// get symbol type enum
char * findSymbolname(char * var); // find the symbol in the list of DC_MEMORY
char *padTo10Bits(const char *binary); // add 10 bits if needed
char **ExtractBinaryValuesWithExtra(Instruction *p); // extractBinaryvalue add extra bits if needed and combine
void WirteToErrorFile(const char *format, ...); //print erros in the error file
#endif // UTILS_H