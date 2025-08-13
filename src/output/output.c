#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../parser/parser.h"
#include "../utils.h"

//this function create our prog.ob file with all valeus needed
void ObMemoryToFile() {
    FILE *erorfp = fopen("./bin/erorFile.ob", "r"); 
    //checking if erorfp exsist and with value inside if not it will be deleted.
    if(erorfp != NULL) {
        fseek(erorfp, 0, SEEK_END);
        long filesize = ftell(erorfp);
        fclose(erorfp);
        if(filesize == 0 )
            remove("./bin/erorFile.ob");
    }
    //create prog.ob file if not exsist
    FILE *fp = fopen("./bin/prog.ob", "wb");
    if(fp == NULL)
    {
        printf("error\n");
        return;
    }
    //print table of the address and binary code of all commands in the projects
    fprintf(fp, "     Base 4 Address   Code\n");
    fprintf(fp, "     ---------------  ---------\n");
    int i = 0; 

    //Handling IC Memory
    //loop on the CI MEMORY list
    //going over the all list checking validation mathods 
    /*

        for example the opcode of  move need to be with two operands
        it check the type of the syntax then if it valid it will run the rest of the function 
        if not it will print error inside eror file and stop the proccess.
    */
    for(int i = 0; i < CI_index; i++) {
        Instruction *p = &CI_memory[i];

        int validchecker = validationMathods(p);
        if(validchecker == -1) {
            exit(1);
        }

        // Check if this instruction's name matches any macro 
        
        int isMacro = 0;
        for (int m = 0; m < macrocounter; m++) {
            if ((p->src_label && strcmp(p->src_label ,macro_table[m].name) == 0) ||( p->dist_label && strcmp(p->dist_label ,macro_table[m].name) == 0)) {
                isMacro = 1;

                // Print all instructions inside macro
                for (int mi = 0; mi < macro_table[m].lineCounter; mi++) {
                    Instruction *macroInst = macro_table[m].LineInst[mi];
                    char **binaryLines = ExtractBinaryValuesWithExtra(macroInst);    //extract combine binary value of the macro function

                    //converting the address of the command
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
        //if the command not having macro callable it will extract only what in the command 
        /*
            for example : 
             mov r1,r2
             this command will be extract and printed to prog.ob file with binary values and address witn 4 base
        */
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

/*
    wirte to a file any symbols in the entry list to prog.ent
    creating the prog.ent if needed if not deleteing it 
    going over all the list and extracting it with binary valeus and address 4base
*/
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

/*
    wirte to a file any symbols in the extern list to prog.ext
    creating the prog.ent if needed if not deleteing it 
    going over all the list and extracting it with binary valeus and address 4base
*/
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