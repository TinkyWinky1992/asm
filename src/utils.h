#ifndef UTILS_H
#define UTILS_H

#include <ctype.h>

void toLowerCase(char* str);
int SkippingSpaces(int start, char* line);
int isAlphaOrNum(char c);
int isAlpha(char c);
void extractMcroName(char *line, char *macroName);
void skipping_label(char *line, char *word);
char **split_instruction_opcode(const char *line);
char **split_instruction_symbol(const char *line,int* outSize);
#endif