#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../parser/parser.h"
#include "../store/store.h"
#include "../utils.h"

/**
 * Counts how many tokens exist in a NULL-terminated token array.
 * 
 * @param tokens - Array of strings (tokens), ending with NULL.
 * @return Number of tokens in the array.
 */
int getTokenSize(char **tokens) {
    int counter = 0;
    while(tokens[counter] != NULL)
        counter++;
    return counter;
}

/**
 * Frees all dynamically allocated memory inside an Instruction struct.
 * 
 * @param inst - Pointer to the Instruction to free.
 */
void freeInstruction(Instruction *inst) {
    if (!inst) return;

    if (inst->name) free(inst->name);
    if (inst->src.name) free(inst->src.name);
    if (inst->src_label) free(inst->src_label);
    if (inst->dist.name) free(inst->dist.name);
    if (inst->dist_label) free(inst->dist_label);
    // Note: Binary memory should be freed here if dynamically allocated.
}

/**
 * Converts a tokenized assembly instruction into an Instruction struct.
 * Handles labels, opcodes, operands, immediate values, and labels-as-operands.
 * 
 * @param tokens - Array of tokens (e.g. {"mov", "r1", "r2"}).
 * @param tokenCount - Number of tokens.
 * @return Pointer to a dynamically allocated Instruction, or NULL on failure.
 */
Instruction *parseTokensIntoInstruction(char **tokens, int tokenCount) {
    if (!tokens || tokenCount == 0) return NULL;

    Instruction *inst = calloc(1, sizeof(Instruction));
    if (!inst) {
        perror("calloc failed");
        return NULL;
    }

    int currentIndex = 0;

    // Check if first token is a label (ends with ':')
    if (tokens[0][strlen(tokens[0]) - 1] == ':') {
        size_t len = strlen(tokens[0]);
        inst->name = malloc(len); // Allocate for label without the colon
        if (!inst->name) {
            perror("malloc failed");
            free(inst);
            return NULL;
        }
        strncpy(inst->name, tokens[0], len - 1);
        inst->name[len - 1] = '\0';
        currentIndex = 1; // Move past the label
    }

    // Ensure we still have tokens left after a label
    if (currentIndex >= tokenCount) {
        freeInstruction(inst);
        free(inst);
        return NULL;
    }

    // Find matching opcode in the opcode table
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
    if (!foundOpcode) { // Invalid opcode
        freeInstruction(inst);
        free(inst);
        return NULL;
    }

    // Parse source operand
    if (currentIndex < tokenCount) {
        char *src = tokens[currentIndex++];
        if (src[0] == '#') { // Immediate number
            int value = atoi(src + 1);
            inst->src.name = strdup(src);
            inst->src.binary = intToBinary(value);
            inst->src.value = value;
            inst->src_label = NULL;
        } else if (isOperandValid(src)) { // Known operand (register, etc.)
            for (int j = 0; j < MAX_OPERAND; j++) {
                if (operands_table[j].name == NULL) break;
                if (strcmp(src, operands_table[j].name) == 0) {
                    inst->src = operands_table[j];
                    inst->src_label = NULL;
                    break;
                }
            }
        } else { // Treat as label
            inst->src_label = strdup(src);
            inst->src.name = NULL;
        }
    }

    // Parse destination operand
    if (currentIndex < tokenCount) {
        char *dist = tokens[currentIndex++];
        if (dist[0] == '#') { // Immediate number
            int value = atoi(dist + 1);
            inst->dist.name = strdup(dist);
            inst->dist.binary = intToBinary(value);
            inst->dist.value = value;
            inst->dist_label = NULL;
        } else if (isOperandValid(dist)) { // Known operand
            for (int j = 0; j < MAX_OPERAND; j++) {
                if (operands_table[j].name == NULL) break;
                if (strcmp(dist, operands_table[j].name) == 0) {
                    inst->dist = operands_table[j];
                    inst->dist_label = NULL;
                    break;
                }
            }
        } else { // Label
            inst->dist_label = strdup(dist);
            inst->dist.name = NULL;
        }
    }

    return inst;
}

/**
 * Parses and registers all lines inside a macro into Instruction structs.
 * 
 * @param mcro - Pointer to the macro to process.
 */
void RgisterMacroInstruction(Macro *mcro) {
    mcro->LineInst = malloc(mcro->lineCounter * sizeof(Instruction*));

    printf("Data inside: %s\n", mcro->lines[0]);

    for(int i  = 0; i < mcro->lineCounter; i++) {
        char **tokens = split_instruction_opcode(mcro->lines[i]);
        if(!tokens || !tokens[0]){
            printf("Error: failed to extract data from macro %s\n", mcro->name);
            if(tokens) free(tokens);
            return;
        }

        int len = getTokenSize(tokens);
        mcro->LineInst[i] = parseTokensIntoInstruction(tokens, len);
        printf("checkinggg: %s", mcro->LineInst[i]->dist.name);
        printf("token checker: %s\n", tokens[0]);
    }
}

/**
 * Reads and stores a macro from the source file into the macro table.
 * Stops when "mcroend" is found.
 * 
 * @param file - Open file pointer.
 * @param line - First line of the macro definition.
 */
void buildMacroTable(FILE *file, char *line) {
    char macroName[MAX_MACRO_NAME];
    extractMcroName(line, macroName);

    Macro *macro = &macro_table[macrocounter];
    strncpy(macro->name, macroName, MAX_MACRO_NAME);
    macro->lineCounter = 0;
    macro->capacity = 10;
    macro->lines = malloc(macro->capacity * sizeof(char*));
    
    if (macro->lines == NULL) {        
        WirteToErrorFile("Error: Memory allocation failed for macro lines.\n");
        return -1;
    }
    
    char buffer[1024];
    char **tempLines = malloc(10 * sizeof(char*));
    int tempCapacity = 10;
    int tempLineCount = 0;

    if (!tempLines) {
        printf("Error: Temporary memory allocation failed.\n");
        return -1;
    }

    int foundEnd = 0;

    // Read each line until "mcroend"
    while (fgets(buffer, sizeof(buffer), file)) {
        buffer[strcspn(buffer, "\n")] = '\0';
        toLowerCase(buffer);

        // Skip comments
        char *semicolon = strchr(buffer, ';');
        if (semicolon) *semicolon = '\0';

        // Trim leading spaces
        char *trimmed = buffer;
        while (isspace(*trimmed)) trimmed++;

        if (*trimmed == '\0') continue; // Skip empty lines

        if (strcmp(trimmed, "mcroend") == 0) {
            foundEnd = 1;
            break;
        }

        // Expand storage if needed
        if (tempLineCount >= tempCapacity) {
            tempCapacity *= 2;
            char **newTemp = realloc(tempLines, tempCapacity * sizeof(char*));
            if (!newTemp) {
                printf("Error: Memory reallocation failed.\n");
                for (int i = 0; i < tempLineCount; i++) free(tempLines[i]);
                free(tempLines);
                return -1;
            }
            tempLines = newTemp;
        }

        tempLines[tempLineCount++] = strdup(trimmed);
    }

    if (!foundEnd) {
        WirteToErrorFile("Error: Missing 'mcroend' for macro '%s'.\n", macroName);
        for (int i = 0; i < tempLineCount; i++) free(tempLines[i]);
        free(tempLines);
        return -1;
    }

    macro->lines = tempLines;
    macro->lineCounter = tempLineCount;
    macro->capacity = tempCapacity;

    RgisterMacroInstruction(macro);
    macrocounter++;

    printMacroTable();
}

/**
 * Parses a symbol definition and stores it into the symbol table.
 * Handles .entry, .extern, .data, .string, etc.
 * 
 * @param word - The first word of the line.
 * @param file - Source file pointer.
 * @param line - Full line text.
 */
void buildSymbolTable(char* word, FILE *file, char *line) {
    if(DC_index  + 1 > MAX_DC_INDEX) {
        WirteToErrorFile("Too many Macros DC Memory is full");
        exit(1);
    }

    char *ptrline = line; 
    int len = 0;

    char **Instruction_symbol_list = split_instruction_symbol(ptrline, &len);
    char *var = NULL;
    char *var2 = NULL;
    char *values = NULL;

    // Check if it's an .extern or .entry directive
    if(strchr(Instruction_symbol_list[0], '.')) {
        var = findSymbolname(Instruction_symbol_list[0]);
        var2 = strdup(Instruction_symbol_list[1]);
    }
    else { // Normal data/string definition
        var2 = findSymbolname(Instruction_symbol_list[1]);
        var = strdup(Instruction_symbol_list[0]);
    }
        
    // Extract values if present
    if(Instruction_symbol_list[2] != NULL) 
        values = strdup(Instruction_symbol_list[2]);
    else
        values = strdup("");

    // Free token list
    for (int j = 0; j < len; j++) {
        free(Instruction_symbol_list[j]);
    }
    free(Instruction_symbol_list);
    
    // Store symbol (argument order depends on type)
    if(strchr(var, '.'))
        StoreSymbol(var2, var, values, ptrline);
    else
        StoreSymbol(var, var2, values, ptrline);
}

/**
 * Parses a command line and stores it into the command table.
 * Handles opcodes, operands, immediate numbers, and label references.
 * 
 * @param word - First word of the line (could be a label).
 * @param file - Source file pointer.
 * @param line - Full text of the instruction line.
 */
void buildCommandTable(char* word, FILE *file, char *line) {
    char *ptrline = line;
    skipping_label(&ptrline, word);

    char **Instruction_list = split_instruction_opcode(ptrline);
    if (!Instruction_list) {
        printf("Failed to split instruction.\n");
        return;
    }

    int index_command = -1;
    int index_operand_src = -1;
    int index_opernad_dist = -1;
    int isnumber = 0;

    int i = 0;
    while(Instruction_list[i] != NULL){
        char *inst = Instruction_list[i];

        // First token should match an opcode
        if(i == 0) {
            for(int j = 0; j < MAX_OPCODE; j++) {
                if(strcmp(inst,opcode_table[j].name ) == 0) {
                    index_command = j;
                    int isNeedOp = opcode_table[j].isOneOp;
                    if(isNeedOp == 1) {
                        index_operand_src = -999; // Single operand only
                    }
                    else if(opcode_table[j].isOneOp == 0) {
                        index_operand_src = -999; // No operands
                        index_opernad_dist = -999;
                    }
                    break;
                }
            }
        }   

        // Process operands (register, number, label)
        if((index_operand_src != -999 || index_opernad_dist != -999) && i > 0) {
            for (int j = 0; j < MAX_OPERAND; j++) {
                if (strcmp(inst, operands_table[j].name) == 0) {
                    if (index_operand_src == -1)
                        index_operand_src = j;
                    else if(index_opernad_dist == -1)
                        index_opernad_dist = j;
                    break;
                }
                else if (*inst == '#') { // Immediate value
                    if(i == 1) {
                        if(index_operand_src == -999) {
                            index_opernad_dist = atoi(inst -1);
                            isnumber = 2;
                        }
                        else {
                            index_operand_src = atoi(inst + 1);
                            isnumber = 1;
                        }
                    }
                    else if( i == 2) {
                        index_opernad_dist = atoi(inst + 1);
                        isnumber = 2;
                    }
                    break;
                }
                // Operand is a label
                else if (*inst != '\0' && isOperandValid(inst) == 0) {
                    if (i == 2)
                        index_opernad_dist = -998;
                    else if (i == 1)
                        if(index_operand_src == -999)
                            index_opernad_dist = -998;
                        else
                            index_operand_src = -998;
                    break;
                }
            }
        }
        i++;
    }

    // Free token list
    int j = 0;
    while(Instruction_list[j] != NULL) {
        free(Instruction_list[j]);
        j++;
    }
    free(Instruction_list);

    // Validation
    if(index_command == -1 || index_operand_src == -1 || index_opernad_dist == -1) {
        WirteToErrorFile("problem with the command of: %s\n", ptrline);
        exit(1);
        return;
    }

    StoreCommands(index_command, index_operand_src, index_opernad_dist, line, isnumber);
}
