#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../parser/parser.h"
#include "../utils.h"
void StoreSymbol(char *name, char *type, char *values, char * line) {
   // printf("name %s\n", name);
   // printf("type: %s\n", type);
   // printf("values: %s\n", values);
    if(sizeof(name) > MAX_LABEL_LEN) {
        printf("Error: Symbol name exceeds maximum length of %d characters.\n", MAX_LABEL_LEN);
        return;
    }
    if(DC_index >= MAX_MEMORY_CI_CD) {
        printf("Error: Out of memory for storing symbol: %s\n", name);
        return;
    }
    int isExternal = 0;
    int isEntry = 0;
    SymbolType temp_type = getSymbolType(type);
    if(temp_type == SYMBOL_ENTRY)
        isEntry = 1;
    else if(temp_type == SYMBOL_EXTERN)
        isExternal = 1;


    Symbol newSymbol = {0};
    strncpy(newSymbol.name, name, MAX_LABEL_LEN - 1);
    newSymbol.address = DC_index;
    newSymbol.type = temp_type;
    newSymbol.isExternal = isExternal;
    newSymbol.isEntry = isEntry;


    // If values are provided, handle them accordingly
    if (values && strlen(values) > 0) {
        newSymbol.values = strdup(values); // Allocate memory for values
    }
    else 
        newSymbol.values = NULL; // No values provided

        
    // Store the symbol in the DC_memory
    DC_memory[DC_index] = newSymbol;
    DC_index++;
    //printf("Stored symbol: %s, type: %d, address: %d, values: %s\n", newSymbol.name, newSymbol.type, newSymbol.address, newSymbol.values ? newSymbol.values : "NULL");

    
}

void StoreCommands(int command, int src, int dist, char * line) {
    //printf("in store commands");
    char *ptrline = line;
    char line_copy[MAX_LABEL_LEN];
    strncpy(line_copy, line, MAX_LABEL_LEN - 1);
    line_copy[MAX_LABEL_LEN - 1] = '\0';
    char *label = NULL;
    char *instruction_part = line_copy;
    // Check if the line starts with a label
    // Extract label if present
    if (line_copy[0] != '\0' && strchr(line_copy, ':')) {
        label = strtok(line_copy, ":");
        instruction_part = strtok(NULL, "");
        if (instruction_part == NULL) instruction_part = "";
        printf("Label found: %s\n", label);
    }

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
    Instruction inst;
    memset(&inst, 0, sizeof(Instruction));

    if (label) {
        strncpy(inst.name, label, MAX_LABEL_LEN - 1);
        inst.name[MAX_LABEL_LEN - 1] = '\0';  // ensure null termination
    } else {
        inst.name[0] = '\0';
    }

    inst.opcode = opcode;
    inst.src = operand_src;
    inst.dist = operand_dist;
    inst.lineNumber = CI_index;

    if(CI_index > MAX_MEMORY_CI_CD) {
        printf("Out of memory for Operation of: %s", line);
        return;
    }
    CI_memory[CI_index] = inst;
    printf("Stored instruction: %s, opcode: %s, src: %s, dist: %s, line number: %d\n", CI_memory[CI_index].name, CI_memory[CI_index].opcode.name, inst.src.name, inst.dist.name, inst.lineNumber);
    CI_index++;
    //store it to CI_MEMORY

    printInstructionTable();
    
}