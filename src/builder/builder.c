#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../parser/parser.h"
#include "../store/store.h"
#include "../utils.h"

void buildSymbolTable(char* word, FILE *file, char *line) {
    char *ptrline = line; 
    int len = 0;
    char **Instruction_symbol_list = split_instruction_symbol(ptrline, &len);
    char *var = NULL;
    char *var2 = NULL;
    char *values = NULL;

    //checking if it a symbol of extern or entry
    if(strchr(Instruction_symbol_list[0], '.')) {
        var = findSymbolname(Instruction_symbol_list[0]);
        var2 = strdup(Instruction_symbol_list[1]);
    }
        
    else { //checkig if it a symbol of data mat string etc..
        var2 = findSymbolname(Instruction_symbol_list[1]);
        var = strdup(Instruction_symbol_list[0]);
    }
        
    //checking for data inside the symbol
    if(Instruction_symbol_list[2] != NULL) 
        values = strdup(Instruction_symbol_list[2]);
    
    else
        values = strdup("");

    //free allocate memory
    for (int j = 0; j < len; j++) {
        free(Instruction_symbol_list[j]);
    }
    free(Instruction_symbol_list);
    
    //change the order of passing data if it type of .entry or .extern
    if(strchr(var, '.'))
        StoreSymbol(var2, var, values, ptrline);
    else
        StoreSymbol(var, var2, values, ptrline);
   // printf("instruction_symbol: %s\n", **Instruction_symbol_list);
}


void buildCommandTable(char* word, FILE *file, char *line) {
    //printf("is inside:?");
    char *ptrline = line;
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
        //printf("index: %d\n", i);
        if(i == 0) {
            for(int j = 0; j < MAX_OPCODE; j++) {
                //printf("[%s]\n", inst);
                //printf("current command: %s\n", opcode_table[j].name);
             
                if(strcmp(inst,opcode_table[j].name ) == 0)
                {

                    index_command = j;
                    int isNeedOp = opcode_table[j].isOneOp;
                    if(isNeedOp == 1) {
                        //printf("dist cant be use on the function");
                        index_opernad_dist = -999; //not needed to use dist operand!
                        

                    }
                    
                    else if(opcode_table[j].isOneOp == 0) {
                        //no needed to use two operands for the function!
                        //printf("current command without operands: %d\n", opcode_table[j].isOneOp);
                        index_operand_src = -999;
                        index_opernad_dist = -999;
                    
                    }
                    //printf("gasdgsd %d\n", index_opernad_dist);
                    break;
                }


            }
        }   

           // printf("%d, %d : %s\n", index_operand_src, index_opernad_dist, inst);
            if((index_operand_src != -999 || index_opernad_dist != -999) && i > 0)
            {
                for (int j = 0; j < MAX_OPERAND; j++) {
                    
                    if (strcmp(inst, operands_table[j].name) == 0) {
                        if (index_operand_src == -1)
                            index_operand_src = j;
                        else if(index_opernad_dist == -1)
                            index_opernad_dist = j;
                        break;
                    }
                    else if (*inst == '#') {
                        if(i == 1) {
                            index_operand_src = atoi(inst + 1);
                            isnumber = 1;
                        }
                        else if( i == 2) {
                            index_opernad_dist = atoi(inst + 1);
                            isnumber = 2;
                        }

                        break;

                    }// when the value it a number and not operand or label like - #5
                    else if (*inst != '\0' && isOperandValid(inst) == 0) {
                        //printf("hisfafasf\n");
                        if (i == 2)
                            index_opernad_dist = -998;
                        else if (i == 1)
                            index_operand_src = -998;
                        break;
                    }
                }
            }

          i++;
        }

    // Free each token and the token array
    
    int j = 0;
    while(Instruction_list[j] != NULL) {
        free(Instruction_list[j]);
        j++;
        
    }
    free(Instruction_list);
    //printf("command index: %d\n", index_command);
    //printf("operand src index: %d\n", index_operand_src);
    //printf("operand dist index: %d\n", index_opernad_dist);
    if(index_command == -1 || index_operand_src == -1 || index_opernad_dist == -1)
    {
        printf("problem with the command of: %s\n", ptrline);
        exit(1);
        return;


    }


    printf("is a number: %d \n", isnumber);

   StoreCommands(index_command, index_operand_src, index_opernad_dist, line, isnumber);
    
}
