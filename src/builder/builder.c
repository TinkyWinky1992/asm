#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../parser/parser.h"
#include "../store/store.h"
#include "../utils.h"

//void 

void buildMacroTable(char* word, FILE *file, char *line) {


}

void buildSymbolTable(char* word, FILE *file, char *line) {
    char *ptrline = line;
    //printf("good?: %s\n",ptrline);


    int len = 0;
    char **Instruction_symbol_list = split_instruction_symbol(ptrline, &len);

    
    
    char *name = strdup(Instruction_symbol_list[0]);
    char *symbol_type = NULL;
    char *values = NULL;
    // printf("length: %d\n", len);
    for(int j = 0; j < 5; j++) {
               //printf("instertion: %s\n", symbolTypes_table[j]);  
        if (symbolTypes_table[j] == NULL) {
          //  printf("Warning: symbolTypes_table[%d] is NULL\n", j);
            continue;
        }
    
        //printf("vtext: %d\n", strcmp(Instruction_symbol_list[1], symbolTypes_table[j]));    
        if(strcmp(Instruction_symbol_list[1], symbolTypes_table[j]) == 0) {
          //printf("type: %s", symbolTypes_table[j]);

                symbol_type = strdup(symbolTypes_table[j]);
                //printf("type: %s", symbol_type);
                break;
            }
                
    }
    //printf("hi: %s\n", symbol_type);
    if(Instruction_symbol_list[2] != NULL) {
        values = strdup(Instruction_symbol_list[2]);
        printf("check: %s\n", values);
    }
    else
        values = strdup("");

    for (int j = 0; j < len; j++) {
        free(Instruction_symbol_list[j]);
    }
    free(Instruction_symbol_list);
    
    StoreSymbol(name, symbol_type, values, ptrline);
   // printf("instruction_symbol: %s\n", **Instruction_symbol_list);
}


void buildCommandTable(char* word, FILE *file, char *line) {
    //printf("Building command table for line: %s\n", line);
    char *ptrline = line;
    
    //printf("is skip good?: %s\n",ptrline);
        // Get the token array from the skipped line
    skipping_label(&ptrline, word);
    char **Instruction_list = split_instruction_opcode(ptrline);
    if (!Instruction_list) {
        printf("Failed to split instruction.\n");
        return;
    }

    
    // Print each token
    int index_command = -1;
    int index_operand_src = -1;
    int index_opernad_dist = -1;
    int isnumber = 0;

    int i = 0;
    while(Instruction_list[i] != NULL){
        char *inst = Instruction_list[i];
        printf("inst: %s\n", inst);
        printf("index: %d\n", i);
        if(i == 0) {
            for(int j = 0; j < MAX_OPCODE; j++) {
                //printf("[%s]\n", inst);
                //printf("current command: %s\n", opcode_table[j].name);
                if(strcmp(inst,opcode_table[j].name ) == 0)
                {

                    index_command = j;
                    int isNeedOp = opcode_table[j].isOneOp;
                    if(isNeedOp == 1) {
                        printf("dist cant be use on the function");
                        index_opernad_dist = -999; //not needed to use dist operand!
                        

                    }
                    else if(opcode_table[j].isOneOp == 0) {
                        //no needed to use two operands for the function!
                        printf("current command without operands: %d\n", opcode_table[j].isOneOp);
                        index_operand_src = -999;
                        index_opernad_dist = -999;
                    
                    }
                    break;
                }

            }
        }   

           // printf("%d, %d : %s\n", index_operand_src, index_opernad_dist, inst);
            if(index_operand_src != -999 || index_opernad_dist != -999)
                for (int j = 0; j < MAX_OPERAND; j++) {
                    //printf("%s : %s\n", operands_table[j], index_operand_src);
                    if (strcmp(inst, operands_table[j].name) == 0) { 
                        if (index_operand_src == -1)
                            index_operand_src = j;
                        else if(index_opernad_dist == -1)
                            index_opernad_dist = j;
                    }
                    if(*inst == '#') {
                        *inst++;;
                        index_opernad_dist = *inst - '0';
                        printf("dist: %d\n", index_opernad_dist);
                        isnumber = 1;
                        continue;
                    }
                }
          i++;
        }

          //  printf("%d, %d : %s\n", index_operand_src, index_opernad_dist, I);



    // Free each token and the token array
    
    int j = 0;
    while(Instruction_list[j] != NULL) {
        free(Instruction_list[j]);
        j++;
        
    }
    free(Instruction_list);
        printf("command index: %d\n", index_command);
    printf("operand src index: %d\n", index_operand_src);
    printf("operand dist index: %d\n", index_opernad_dist);
    if(index_command == -1 || index_operand_src == -1 || index_opernad_dist == -1)
    {
        printf("problem with the command of: %s\n", ptrline);
        return;

    }
    StoreCommands(index_command, index_operand_src, index_opernad_dist, line);
    
}
