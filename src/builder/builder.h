#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef BUILDER_H
#define BUILDER_H

void buildMacroTable(FILE *file, char *line) ;
void buildCommandTable(char* word, FILE *file, char *line);
void buildSymbolTable(char* word, FILE *file, char *line);
#endif // BUILDER_H