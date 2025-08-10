#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../parser/parser.h"
#include "../utils.h"
#include "../output/output.h"


//help function to trim whitespace from the start and end of a string
 char *trim_whitespace(char *str) {
    while (isspace((unsigned char)*str)) str++; // skip leading spaces

    if (*str == 0)  // all spaces?
        return str;

    // remove trailing spaces
    char *end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;

    // write new null terminator
    *(end + 1) = '\0';

    return str;
}



//this function is used to store the symbols in the DC_memory
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
    if(temp_type == SYMBOL_ENTRY){
      //  printf("Entry symbol: %s\n", name);
        isEntry = 1;
    }
        
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
    if(newSymbol.isEntry){
        //printf("Entry symbol: %s\n", newSymbol.name);
        entry_memory[entry_index++] = newSymbol;
        //printf("index: %d\n", entry_index);
    }
        
    else if(newSymbol.isExternal)
        extern_memory[extern_index++] = newSymbol;

    //for debugging purposes
    //printSymbolsTable();


    
}
//this function is used to store the commands with the operands in the IC_memory
void StoreCommands(int command, int src, int dist, char *line, int isnumber) {
    //printf("hasfs: %d %d\n", src, dist);
    char *ptrline = line;
    Instruction inst;
    memset(&inst, 0, sizeof(Instruction));

    char line_copy[MAX_LABEL_LEN];
    strncpy(line_copy, line, MAX_LABEL_LEN - 1);
    line_copy[MAX_LABEL_LEN - 1] = '\0';

    char *label = NULL;
    char *instruction_part = line_copy;

    // Check and extract label
    if (line_copy[0] != '\0' && strchr(line_copy, ':')) {
        label = strtok(line_copy, ":");
        label = trim_whitespace(label);
        instruction_part = strtok(NULL, "");
        if (instruction_part == NULL) instruction_part = "";
    }

    Opcode opcode = opcode_table[command];

    Operand operand_src = {0};
    Operand operand_dist = {0};

    // Handle src operand
    if (isnumber == 1) {
        // src is an immediate number
        int len = snprintf(NULL, 0, "%d", src);
        char *num = malloc(len + 1);
        sprintf(num, "%d", src);

        char *binary = intToBinary(src);
        operand_src.name = malloc(len + 2);  // +1 for '#', +1 for '\0'
        sprintf(operand_src.name, "#%s", num);
        operand_src.binary = strdup(binary);

       // printf("src name: %s\n", operand_src.name);
        //printf("src binary: %s\n", operand_src.binary);
        free(num);

    } else if (src == -998) {
        // src is a label
        char **spliter = split_instruction_opcode(ptrline);
        if (label != NULL)
            inst.src_label = strdup(spliter[2]);
        else
            inst.src_label = strdup(spliter[1]);
        free(spliter);

    } else if (src != -999) {
        // src is a known operand
        operand_src = operands_table[src];
    } else {
        // src is not used (e.g., one-operand instruction)
        operand_src = operands_table[8]; // assume 8 is for "no operand"
    }

    // Handle dist operand
    if (isnumber == 2) {
        // dist is an immediate number
        int len = snprintf(NULL, 0, "%d", dist); //getting the len of dist
        char *num = malloc(len + 1); // allocate memory for number
        sprintf(num, "%d", dist); // convert to string

        char *binary = intToBinary(dist); // getting binary representation
        operand_dist.name = malloc(len + 2);  // +1 for '#', +1 for '\0'
        sprintf(operand_dist.name, "#%s", num); // create name with '#'
        operand_dist.binary = strdup(binary); // copy binary representation
        free(num); 

    } else if (dist == -998) {
        // dist is a label
        char **spliter = split_instruction_opcode(ptrline);
        if (label != NULL)
            inst.dist_label = strdup(spliter[3]);
        else
            inst.dist_label = strdup(spliter[2]);
        free(spliter);

    } else if (dist != -999) {
        // dist is a known operand
        operand_dist = operands_table[dist];
    } else {
        // dist is not used
        operand_dist = operands_table[8]; // assume 8 is for "no operand"
        //printf("dist name: %s\n", operand_dist.name);
    }

    // Store the instruction label (if present)
    if (label) {
        int len = snprintf(NULL, 0, "%s", label);
        inst.name = malloc(len + 1);
        if (!inst.name) {
            perror("malloc failed");
            exit(1);
        }
        snprintf(inst.name, len + 1, "%s", label);
    } else {
        inst.name = NULL;
    }


    // Finalize instruction
    inst.opcode = opcode;
    inst.src = operand_src;
    inst.dist = operand_dist;
    inst.lineNumber = CI_index;

    // Store in memory
    if (CI_index > MAX_MEMORY_CI_CD) {
        printf("Out of memory for Operation of: %s", line);
        return;
    }
    CI_memory[CI_index] = inst;
    CI_index++;


    //for debugging purposes to print the instruction table
   // printInstructionTable();
}
