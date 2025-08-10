#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "./parser/parser.h"
#define MAX_TOKENS 4
#define MAX_TOKEN_SYMBOL 3

char **split_instruction_symbol(const char *input, int* outSize) {
    char** result = malloc(MAX_TOKEN_SYMBOL * sizeof(char*));
    *outSize = 0;

    // Make a modifiable copy of input
    char* line = strdup(input);
    if (!line) {
        perror("strdup failed");
        return NULL;
    }

    // Skip leading spaces in line
    char* start = line;
    while (isspace((unsigned char)*start)) start++;

    if (*start == '\0') {
        // Empty or all spaces input
        free(line);
        return result;
    }

    // Now find first space after the trimmed start
    char* colon = strchr(start, ' ');
    if (!colon) {
        printf("Error: No space found in input.\n");
        free(line);
        return NULL;
    }

    *colon = '\0';

    // Strip trailing colon from name if exists
    size_t len = strlen(start);
    if (len > 0 && start[len - 1] == ':') {
        start[len - 1] = '\0';
    }

    result[0] = strdup(start);
    (*outSize)++;

    // After the first space (colon), skip spaces again
    char* rest = colon + 1;
    while (isspace((unsigned char)*rest)) rest++;

    if (*rest == '\0') {
        free(line);
        return result;  // only name found
    }

    // Extract type (until next space or end)
    char* typeStart = rest;
    while (*rest && !isspace((unsigned char)*rest)) rest++;
    char saved = *rest;
    *rest = '\0';

    result[1] = strdup(typeStart);
    (*outSize)++;

    if (saved == '\0') {
        free(line);
        return result;  // only name + type
    }

    // Extract value if exists
    rest++; // move past the null terminator we injected
    while (isspace((unsigned char)*rest)) rest++;

    if (*rest != '\0') {
        result[2] = strdup(rest);
        (*outSize)++;
    }

    free(line);
    return result;
}


char **split_instruction_opcode(const char *line) {
    char **tokens = malloc(sizeof(char*) * (MAX_TOKENS + 1)); // +1 for NULL at end
    if (!tokens) return NULL;

    int count = 0;
    const char *delimiters = " ,";

    char *line_copy = strdup(line);
    if (!line_copy) {
        free(tokens);
        return NULL;
    }

    char *token = strtok(line_copy, delimiters);
    while (token != NULL && count < MAX_TOKENS) {
        // If token contains '[', truncate at that point
        char *bracket_pos = strchr(token, '[');
        if (bracket_pos) {
            *bracket_pos = '\0'; // Cut string at first '['
        }

        tokens[count] = strdup(token); // Save clean token
        if (!tokens[count]) {
            for (int i = 0; i < count; i++) free(tokens[i]);
            free(tokens);
            free(line_copy);
            return NULL;
        }

        count++;
        token = strtok(NULL, delimiters);
    }

    tokens[count] = NULL;
    free(line_copy);
    return tokens;
}


// skipping spaces in the line
int SkippingSpaces(int start, char* line ) {
    int index = start;
    while (line[index] != '\0' && isspace(line[index]))
        index++;
    return index;
}

// lower the characters from C to c 
void toLowerCase(char* str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}


// checking for numbers
int isAlphaOrNum(char c) {
    return (c >= '0' && c <='9') || (c >= 'a' && c <= 'z');
}

int isAlpha(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

void extractMcroName(char *line, char *macroName) {
  char *p = line;
  while (*p && isspace((unsigned char)*p)) p++; // Skip leading spaces
  p += 4; // Skip "mcro" 
  if(isspace(++*p) || *p != '\0') {
   // printf("it good we have space and it not end of the line ");
    //extract mcro name
    int i = 0;
    *p++;
    while(*p != '\0')
    
     // printf("we are in the loop: %c\n", *p);
      macroName[i++] = *p++;
    macroName[i] = '\0';
  }


}

void skipping_label(char **line, const char *word) {
    char *ptr = *line;

    // Skip leading spaces
    while (isspace(*ptr)) {
        ptr++;
    }

    // Check if there is a label ending with ':'
    char *colon = strchr(ptr, ':');
    if (colon) {
        // Move past the colon
        ptr = colon + 1;

        // Skip any spaces after the label
        while (isspace(*ptr)) {
            ptr++;
        }
    }

    // Optional: search for the word and align to it
    char *found = strstr(ptr, word);
    if (found) {
        *line = found;
    } else {
        // If not found, still move past label
        *line = ptr;
    }
}

SymbolType getSymbolType(const char *typeStr) {
    if (strcmp(typeStr, ".code") == 0) {
        return SYMBOL_CODE;
    } else if (strcmp(typeStr, ".data") == 0) {
        return SYMBOL_DATA;
    } else if (strcmp(typeStr, ".entry") == 0) {
        return SYMBOL_ENTRY;
    } else if (strcmp(typeStr, ".extern") == 0) {
        return SYMBOL_EXTERN;
    } else if (strcmp(typeStr, ".string") == 0) {
        return SYMBOL_STRING;
    } else if (strcmp(typeStr, ".metrix") == 0) {
        return SYMBOL_METRIX;
    }
    return -1; // Invalid type
}



char *findSymbolname(char * var) {
    char * result;
    for(int j = 0; j < 5; j++) {
               //printf("instertion: %s\n", symbolTypes_table[j]);  
        if (symbolTypes_table[j] == NULL) {
          //  printf("Warning: symbolTypes_table[%d] is NULL\n", j);
            continue;
        }
    
        //printf("vtext: %d\n", strcmp(Instruction_symbol_list[1], symbolTypes_table[j]));    
        if(strcmp(var, symbolTypes_table[j]) == 0) {
          //printf("type: %s", symbolTypes_table[j]);

                result = strdup(symbolTypes_table[j]);
                //printf("type: %s", symbol_type);
                break;
            }
                
    }
    return result;
}
char* convertToBase4(int num) {
    // handle zero case
    if (num == 0) {
        char* zeroStr = (char*)malloc(2);
        strcpy(zeroStr, "0");
        return zeroStr;
    }

    char buffer[33]; // enough for 32-bit integers
    int i = 0;

    // convert number to base 4
    while (num > 0) {
        buffer[i++] = (num % 4) + '0';  // Convert digit to character
        num /= 4;
    }

    // Allocate memory for result string
    char* result = (char*)malloc(i + 1);
    if (!result) return NULL;

    // Reverse the digits into result
    for (int j = 0; j < i; j++) {
        result[j] = buffer[i - j - 1];
    }
    result[i] = '\0';

    return result;
}

char *intToBinary(int num) {
    // Allocate enough space for 32 bits + null terminator
    int totalBits = sizeof(char) * 10;
    char *output = malloc(totalBits + 1); // +1 for null terminator
    if (!output) {
        perror("Memory allocation failed");
        return NULL;
    }

    unsigned int uNum = (unsigned int) num; // use unsigned to preserve 2's complement

    for (int i = totalBits - 1; i >= 0; --i) {
        output[totalBits - 1 - i] = ((uNum >> i) & 1) + '0';
    }

    output[totalBits] = '\0'; // null-terminate

    return output;
}
char *padTo10Bits(const char *binary) {
    size_t len = strlen(binary);
    if (len >= 10) {
        return strdup(binary);  // already valid
    }

    char *padded = malloc(11); // 10 bits + null terminator
    if (!padded) {
        perror("Failed to allocate padded binary");
        exit(1);
    }

    // Fill with '0's
    memset(padded, '0', 10);
    // Copy the original binary to the end
    strcpy(padded + (10 - len), binary);
    padded[10] = '\0';

    return padded;
}

int isOperandValid(char *var) {
    for (int i = 0; i < MAX_OPERAND; i++) {
        if (strcmp(var,operands_table[i].name ) == 0) {
            return 1; // Operand is valid
        }
    }
    return 0; // Operand is not valid
}


char *ExtractBinary(char *label, Operand temp) {
    //printf("temp : %s %d %s\n ",temp.name, temp.value, temp.binary);
    char *binary = NULL;
    int found = 0;
    int labelAddr = 0;
    printf("label checking %s\n", label);
    
        if(label != NULL && strlen(label) > 0) {
        // Search for the label in DC_memory
        int found = 0;
        for (int i = 0; i < DC_index; i++) {
            if ( DC_memory[i].name != NULL && strcmp(DC_memory[i].name,label) == 0) {
                labelAddr = i;
                found = 1;
                break;
            }
        }

        if(!found) {
            for (int i = 0; i < CI_index; i++) {
                if (CI_memory[i].name != NULL && strcmp(CI_memory[i].name, label) == 0) {
                    labelAddr = i + MAX_DC_INDEX;
                    found = 1;
                    break;
                }
            }
        }
        if(!found) {
            for(int i = 0; i < macrocounter; i++){
                if(strcmp(label, macro_table[i].name) == 0)
                    found = 1;
                    
            }
        }

        if (!found) {
            printf("Error: Label '%s' not found for SRC operand.\n", label);
            exit(1);
        }

        
        binary = intToBinary(labelAddr);
       
        
    }
    else if(temp.value == -999) {
        binary = NULL;
        
    }   

    
    else {

        binary = strdup(temp.binary);
        
    }
        
   // printf("addr %s\n", binary);
    return binary;

}


char **ExtractBinaryValuesWithExtra(Instruction *p) {
   // printf("name of opcode: %s\n", p->opcode.name);

    char *opcodeBin = strdup(p->opcode.binary);
    if (!opcodeBin) {
        perror("Failed to allocate opcode binary");
        exit(1);
    }

    char *srcbin = ExtractBinary(p->src_label, p->src);
    char *distbin = ExtractBinary(p->dist_label, p->dist);
    //printf("checking element: %s %s\n", srcbin ? srcbin : "(null)", distbin ? distbin : "(null)");

    int count = 1;  // opcodeBin is always present
    if (srcbin) count++;
    if (distbin) count++;

    char **binaryWords = malloc((count + 1) * sizeof(char *));
    if (!binaryWords) {
        perror("Failed to allocate binaryWords array");
        exit(1);
    }

    int index = 0;
    binaryWords[index++] = padTo10Bits(opcodeBin);
    free(opcodeBin);

    if (srcbin != NULL) {
        binaryWords[index++] = padTo10Bits(srcbin);
        free(srcbin);
    }
    if (distbin != NULL) {
        binaryWords[index++] = padTo10Bits(distbin);
        free(distbin);
    }

    binaryWords[index] = NULL;

    return binaryWords;
}
