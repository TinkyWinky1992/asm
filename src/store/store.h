#ifdef STORE.H
#define STORE.H
#include <stdio.h>
#include <stdlib.h>


void StoreSymbol(char *name, char *type, char *values, char * line;);
void StoreCommands(int command, int src, int dist, char * line);

#endif