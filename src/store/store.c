
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../parser/parser.h"

void StoreSymbol(char *name, char *type, char *values, char * line) {
    printf("name %s\n", name);
    printf("type: %s\n", type);
    //SymbolType type;
    

    
}
void StoreCommands(int command, int src, int dist, char * line) {
    //printf("in store commands");
    Opcode opcode = opcode_table[command];
    Operand operand_src;
    Operand operand_dist;
    printf("opcode name: %s\n", opcode.name);
    if(src != -999)
        operand_src = operands_table[src];
    else 
        operand_src = operands_table[8];

    if(dist != -999)
        operand_dist = operands_table[dist];
    else 
        operand_dist = operands_table[8];

    printf("operands: %s, %s\n", operand_src.name, operand_dist.name);
    Instruction inst = {opcode, operand_src, operand_dist, CI_index};
    if(CI_index > MAX_MEMORY_CI_CD) {
        printf("Out of memory for Operation of: %s", line);
        return;
    }
    CI_memory[CI_index] = inst;
    CI_index++;
    //store it to CI_MEMORY

    printInstructionTable();
    


}