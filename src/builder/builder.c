#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../parser/parser.h"
#include "../store/store.h"
#include "../utils.h"

int getTokenSize(char **tokens) {

    int counter = 0;
    while(tokens[counter] != NULL)
        counter++;

    return counter;

}
void freeInstruction(Instruction *inst) {
    if (!inst) return;

    if (inst->name) free(inst->name);
    if (inst->src.name) free(inst->src.name);
    if (inst->src_label) free(inst->src_label);
    if (inst->dist.name) free(inst->dist.name);
    if (inst->dist_label) free(inst->dist_label);
    // free binary if dynamically allocated
}

Instruction *parseTokensIntoInstruction(char **tokens, int tokenCount) {
    if (!tokens || tokenCount == 0) return NULL;

    Instruction *inst = calloc(1, sizeof(Instruction));
    if (!inst) {
        perror("calloc failed");
        return NULL;
    }

    int currentIndex = 0;

    // Check if first token is label (ends with ':')
    if (tokens[0][strlen(tokens[0]) - 1] == ':') {
        size_t len = strlen(tokens[0]);
        inst->name = malloc(len); // len - 1 + 1 for '\0'
        if (!inst->name) {
            perror("malloc failed");
            free(inst);
            return NULL;
        }
        strncpy(inst->name, tokens[0], len - 1);
        inst->name[len - 1] = '\0';
        currentIndex = 1;
    }

    if (currentIndex >= tokenCount) {
        freeInstruction(inst);
        free(inst);
        return NULL;
    }

    char *opcodeStr = tokens[currentIndex++];
    int foundOpcode = 0;
    for (int i = 0; i < MAX_OPCODE; i++) {
        if (opcode_table[i].name == NULL) break;
        if (strcmp(opcodeStr, opcode_table[i].name) == 0) {
            inst->opcode = opcode_table[i];
            foundOpcode = 1;
            break;
        }
    }
    if (!foundOpcode) {
        freeInstruction(inst);
        free(inst);
        return NULL;
    }

    // Parse src operand
    if (currentIndex < tokenCount) {
        char *src = tokens[currentIndex++];
        if (src[0] == '#') {
            int value = atoi(src + 1);
            inst->src.name = strdup(src);
            inst->src.binary = intToBinary(value);
            inst->src.value = value;
            inst->src_label = NULL;
        } else if (isOperandValid(src)) {
            for (int j = 0; j < MAX_OPERAND; j++) {
                if (operands_table[j].name == NULL) break;
                if (strcmp(src, operands_table[j].name) == 0) {
                    inst->src = operands_table[j];
                    inst->src_label = NULL;
                    break;
                }
            }
        } else {
            inst->src_label = strdup(src);
            inst->src.name = NULL;
        }
    }

    // Parse dist operand
    if (currentIndex < tokenCount) {
        char *dist = tokens[currentIndex++];
        if (dist[0] == '#') {
            int value = atoi(dist + 1);
            inst->dist.name = strdup(dist);
            inst->dist.binary = intToBinary(value);
            inst->dist.value = value;
            inst->dist_label = NULL;
        } else if (isOperandValid(dist)) {
            for (int j = 0; j < MAX_OPERAND; j++) {
                if (operands_table[j].name == NULL) break;
                if (strcmp(dist, operands_table[j].name) == 0) {
                    inst->dist = operands_table[j];
                    inst->dist_label = NULL;
                    break;
                }
            }
        } else {
            inst->dist_label = strdup(dist);
            inst->dist.name = NULL;
        }
    }

    return inst;
}

void RgisterMacroInstruction(Macro *mcro) {
    mcro->LineInst = malloc(mcro->lineCounter * sizeof(Instruction*));
    Instruction *inst;
    printf("Data inside: %s\n", mcro->lines[0]);




    for(int i  = 0; i < mcro->lineCounter; i++) {
        char **tokens = split_instruction_opcode(mcro->lines[i]);
        if(!tokens || !tokens[0]){
            printf("Erorr: failed to extract data from macro %s\n", mcro->name);
            if(tokens) free(tokens);
            return;
           


        }
        int len = getTokenSize(tokens);
        mcro->LineInst[i] = parseTokensIntoInstruction(tokens,len);
        printf("checkinggg: %s", mcro->LineInst[i]->dist.name);
        printf("token checker: %s\n", tokens[0]);
    }

}
void buildMacroTable(FILE *file, char *line) {
    char macroName[MAX_MACRO_NAME];
    extractMcroName(line, macroName);

      // Initialize the macro
    Macro *macro = &macro_table[macrocounter];
    strncpy(macro->name, macroName, MAX_MACRO_NAME);
    macro->lineCounter = 0;
    macro->capacity = 10; // Initial capacity for lines
    macro->lines = malloc(macro->capacity * sizeof(char*));
    
    if (macro->lines == NULL) {
        printf("Error: Memory allocation failed for macro lines.\n");
        return -1;
    }
    
// Read lines into temporary buffer
    char buffer[256];
    char **tempLines = malloc(10 * sizeof(char*));
    int tempCapacity = 10;
    int tempLineCount = 0;

    if (!tempLines) {
        printf("Error: Temporary memory allocation failed.\n");
        return -1;
    }

    int foundEnd = 0;

    while (fgets(buffer, sizeof(buffer), file)) {
       // Remove trailing newline and convert to lowercase
        buffer[strcspn(buffer, "\n")] = '\0';
        toLowerCase(buffer);

        //skipping comments
        char *semicolon = strchr(buffer, ';');
        if (semicolon) *semicolon = '\0';

        // Skip leading spaces
        char *trimmed = buffer;
        while (isspace(*trimmed)) trimmed++;

        if (*trimmed == '\0') {
            // Line is empty (only spaces or nothing), skip it
            continue;
        }

        if (strcmp(trimmed, "mcroend") == 0) {
            foundEnd = 1;
            break;
        }

        // add line to temporary storage
        if (tempLineCount >= tempCapacity) {
            tempCapacity *= 2;
            char **newTemp = realloc(tempLines, tempCapacity * sizeof(char*));
            if (!newTemp) {
                printf("Error: Memory reallocation failed.\n");
                //clean all
                for (int i = 0; i < tempLineCount; i++) free(tempLines[i]);
                free(tempLines);
                return -1;
            }
            tempLines = newTemp;
        }

        tempLines[tempLineCount++] = strdup(trimmed);
    }

    if (!foundEnd) {
        printf("Error: Missing 'mcroend' for macro '%s'.\n", macroName);
        for (int i = 0; i < tempLineCount; i++) free(tempLines[i]);
        free(tempLines);
        return -1;
    }

    // save macro if it exists
    macro->lines = tempLines;
    macro->lineCounter = tempLineCount;
    macro->capacity = tempCapacity;

    RgisterMacroInstruction(macro);
    macrocounter++;


    printMacroTable();
}



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


   // printf("is a number: %d \n", isnumber);

   StoreCommands(index_command, index_operand_src, index_opernad_dist, line, isnumber);
    
}
