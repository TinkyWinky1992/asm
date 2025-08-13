#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef BUILDER_H
#define BUILDER_H

void buildMacroTable(FILE *file, char *line) ;// handling of buling each macro to the macro list. 
void buildCommandTable(char* word, FILE *file, char *line);//handling of build each command to CI memory
void buildSymbolTable(char* word, FILE *file, char *line);// handling of build each symbol to DC memory
#endif // BUILDER_H