#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../parser/parser.h"
#include "../utils.h"


void ObMemoryToFile() {
    printf("starting proccess of Instruction...\n");
    FILE *fp = fopen("./bin/prog.ob", "wb");
    if(fp == NULL)
    {
        printf("error\n");
        return;
    }
    fprintf(fp, "     Base 4 Address   Code\n");
    fprintf(fp, "     ---------------  ---------\n");
    int i = 0; 
    //printf("CI_index: %d\n", CI_index);

    //Handling IC Memory
    for(i = 0; i < CI_index; i++) {
        if(i + MAX_DC_INDEX > MAX_MEMORY) {
            printf("Error: CI index exceeds maximum limit.\n");
            fclose(fp);
            return;
        }

        Instruction *p = &CI_memory[i];
        //printf("binary before: %s\n", p->src.binary);
        char **binary = ExtractBinaryValuesWithExtra(p);
        
        for(int j = 0; j < 256 && binary[j] != NULL; j++) {
            char *base4 = convertToBase4(i + MAX_DC_INDEX);
            fprintf(fp, "     %s             %s\n", base4, binary[j]);
            free(binary[j]); // Free each binary string
        }
        free(binary); // Free the allocated memory for binary string
        
    }
}

void entryToFile() {
    if(entry_index == 0) {
        remove("./bin/prog.ent");
        return;
    }
    printf("starting proccess of entry...\n");
    FILE *fp = fopen("./bin/prog.ent", "wb");
    if(fp == NULL)
    {
        printf("error\n");
        return;
    }

    for(int i= 0; i <entry_index; i++) {
        Symbol *p = &entry_memory[i];
        char *binary = intToBinary(p->address);
         fprintf(fp, "%s  %s\n", p->name, binary);

    }
}

void externToFile() {
    if(extern_index == 0) {
        remove("./bin/prog.ext");
        return;
    }
    printf("starting proccess of extern...\n");
    FILE *fp = fopen("./bin/prog.ext", "wb");
    if(fp == NULL)
    {
        printf("error\n");
        return;
    }

    for(int i= 0; i <extern_index; i++) {
        Symbol *p = &extern_memory[i];
        char *binary = intToBinary(p->address);
         fprintf(fp, "%s %s\n", p->name, binary);

    }
}