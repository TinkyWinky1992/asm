#ifndef OUTPUT.H
#define OUTPUT.H
#include <stdio.h>
#include <stdlib.h>
#include "../parser/parser.h"
void ObMemoryToFile(); // making all the commands inside prog.ob with binary values and memory locations of all commands
void entryToFile(); // make all symbol type of .entry in prog.ent with binary valeus of the location in the memory and name of the entry
void externToFile(); // make alll symbol type of .extern in prog.ext with binray values of the location in the memory and name of the extern
#endif