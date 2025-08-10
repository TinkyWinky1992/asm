#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "./parser.h"
#include "../utils.h"
#define MAX_MACROS 100
int DC_index = 0;
int CI_index = 0;
int entry_index = 0;
int extern_index = 0;
MainMemory memory = {CI_memory, DC_memory , MAX_CI_INDEX, MAX_DC_INDEX, MAX_MEMORY};
Symbol DC_memory[MAX_MEMORY_CI_CD];
Instruction CI_memory[MAX_MEMORY_CI_CD];
Macro macro_table[MAX_MACROS];
Symbol entry_memory[MAX_MEMORY_CI_CD];
Symbol extern_memory[MAX_MEMORY_CI_CD];

char *symbolTypes_table[] = {
    ".entry", ".mat", ".data", ".extern", ".string"
};


Operand operands_table[MAX_OPERAND] = {
    {"r0", 0, "000"},
    {"r1", 1, "001"},
    {"r2", 2, "010"},
    {"r3", 3, "011"},
    {"r4", 4, "100"},
    {"r5", 5, "101"},
    {"r6", 6, "110"},
    {"r7", 7, "111"},
    {"ud", -999, "000"},

};

Opcode opcode_table[MAX_OPCODE] = {
    {"mov", 0, "0000", -1},
    {"sub", 1, "0001", -1},
    {"cmp", 2, "0010", -1},
    {"add", 3, "0011", -1},
    {"not", 4, "0100", 1},
    {"clr", 5, "0101", 1},
    {"lea", 6, "0110", -1},
    {"inc", 7, "0111", 1},
    {"dec", 8, "1000", 1},
    {"jmp", 9, "1001", 1},
    {"bne", 10, "1010", 1},
    {"red", 11, "1011", 1},
    {"prn", 12, "1100", 1},
    {"jsr", 13, "1101", 1},
    {"rts", 14, "1110", 0},
    {"stop", 15, "1111", 0}
};

int macrocounter = 0;
int flagEn = 0;





//checking for a symbol
int isSymbol(char* word) {

    const char *directives[] = {
        ".entry", ".data", ".string", ".mat", ".extern"
    };
    int len  = sizeof(directives)/sizeof(directives[0]);
    for(int i = 0; i < len; i++){
        if(strcmp(word, directives[i]) == 0)
            return 1;
            
    }
    return -1;
}

//checking for a Instruction 
int isCommand(char* word) {
            
    for (int i = 0; i < MAX_OPCODE; i++) {
        //printf("size %d", sizeof(word));
        if (strcmp(word, opcode_table[i].name) == 0)
            return 1;
    }
    return -1;
}

int isLabel(char* word) {
    int len = strlen(word);
    //checking if it a valid label
    if(len == 0 || word[len -1 ] != ':')
    {
        return -1;
    }
    //first char need to be in the alphabet
    if(!isAlpha(word[0]) == -1) {
        printf("first letter in the lable doesn't a alphbet in: %s\n", word);
        return -1;
    }
    //checking all of the characters in the word is a number or alphabets
    for(int i = 1; i <len; i++) {
        if(isAlphaOrNum(word[i]) == -1){
            printf("there is a problem with the lable:%s\n", word);
        }
    }
    return 1;

}

int isMacro(char *word, FILE *file, char *line) {
    if (strcmp(word, "mcro") != 0)
        return -1;
    
    if(macrocounter >= MAX_MACROS) {
        printf("Error: Maximum number of macros reached.\n");
        return -1;
    }
   // printf("Macro detected: %s\n", word);
    char macroName[MAX_MACRO_NAME];
    extractMcroName(line, macroName);
    printf("%s\n", macroName);
    if(strlen(macroName) == 0) {
        printf("Error: Macro name is empty.\n");
        return -1;
    }
    
    //checking if mcro exists
    for(int i = 0; i < macrocounter; i++) {
        if(strcmp(macro_table[i].name, macroName) == 0) {
            printf("Error: Macro '%s' already exists.\n", macroName);
            return -1;
        }
    }

  
    return 1;
    //printf("Enter macro name: %s\n", fscanf(file, "%s", macroName));
    
    }


void printMacroTable() {
    printf("\n==== MACRO TABLE (%d macros) ====\n", macrocounter);
    for (int i = 0; i < macrocounter; i++) {
        Macro *macro = &macro_table[i];
        printf("Macro #%d: %s\n", i + 1, macro->name);
        printf("Body (%d lines):\n", macro->lineCounter);
        for (int j = 0; j < macro->lineCounter; j++) {
            printf("  %s\n", macro->lines[j]);
        }
        printf("----------------------\n");
    }
}
void printInstructionTable() {
    printf("\n==== CI MEMOY(%d used) ====\n", CI_index);
    for(int i = 0; i < CI_index; i++) {
        Instruction *inst = &CI_memory[i];
        char *src_name;
        char *dist_name;
        //printf("checler: %s\n", inst->src.name);
        if (inst->src_label != NULL)
            src_name = strdup(inst->src_label);
        else if (inst->src.name != NULL)
            src_name = strdup(inst->src.name);
        else
            src_name = strdup("???");


        if (inst->dist_label != NULL)
            dist_name = strdup(inst->dist_label);
        else if (inst->dist.name != NULL)
            dist_name = strdup(inst->dist.name);
        else
            dist_name = strdup("???");

        printf("[%d] %s %s %s , %s\n", MAX_DC_INDEX + i ,inst->name ,inst->opcode.name, src_name, dist_name);
        free(src_name);
        free(dist_name);
    }
    printf("----------------------\n");

}


void printSymbolsTable() {
    printf("\n==== DC MEMOY(%d used) ====\n", DC_index);
    for(int i = 0; i < DC_index; i++) {
        Symbol *sym = &DC_memory[i];
        printf("[%d] %s %d %s\n", i ,sym->name ,sym->address, sym->values);
    }
    printf("----------------------\n");
}