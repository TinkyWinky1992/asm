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
    for(int i = 0; i < CI_index; i++) {
        Instruction *p = &CI_memory[i];

        int validchecker = validationMathods(p);
        if(validchecker == -1) {
            exit(1);
        }

        // Check if this instruction's name matches any macro name
        int isMacro = 0;
        for (int m = 0; m < macrocounter; m++) {
            if ((p->src_label && strcmp(p->src_label ,macro_table[m].name) == 0) ||( p->dist_label && strcmp(p->dist_label ,macro_table[m].name) == 0)) {
                
                isMacro = 1;

                // Print all instructions inside macro
                for (int mi = 0; mi < macro_table[m].lineCounter; mi++) {
                    Instruction *macroInst = macro_table[m].LineInst[mi];
                    char **binaryLines = ExtractBinaryValuesWithExtra(macroInst);

                    for (int bl = 0; bl < 256 && binaryLines[bl] != NULL; bl++) {
                        char *base4Addr = convertToBase4(i + MAX_DC_INDEX);
                        fprintf(fp, "     %s             %s\n", base4Addr, binaryLines[bl]);
                        free(base4Addr);
                        free(binaryLines[bl]);
                    }
                    free(binaryLines);
                }
                break;
            }
        }

        if (!isMacro) {
            char **binary = ExtractBinaryValuesWithExtra(p);
            for(int j = 0; j < 256 && binary[j] != NULL; j++) {
                char *base4Addr = convertToBase4(i + MAX_DC_INDEX);
                fprintf(fp, "     %s             %s\n", base4Addr, binary[j]);
                free(base4Addr);
                free(binary[j]);
    
            }
            free(binary);
        }
    }

    fclose(fp);
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