#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAX_TOKENS 4
#define MAX_TOKEN_SYMBOL 3

char **split_instruction_symbol(const char *input,int* outSize) {
    char** result = malloc(MAX_TOKEN_SYMBOL * sizeof(char*));
    *outSize = 0;

    char* line = strdup(input);  // modifiable copy

    // 1. Extract name (before ':')
    char* colon = strchr(line, ':');
    if (!colon) {
        free(line);
        return NULL;
    }
    *colon = '\0';
    result[0] = strdup(line);  // name
    (*outSize)++;

    // 2. After colon: get type
    char* rest = colon + 1;
    while (isspace(*rest)) rest++;  // skip spaces

    if (*rest == '\0') {
        free(line);
        return result;  // only name found
    }

    // Extract type (until next space or end)
    char* typeStart = rest;
    while (*rest && !isspace(*rest)) rest++;
    char saved = *rest;
    *rest = '\0';
    result[1] = strdup(typeStart);
    (*outSize)++;

    if (saved == '\0') {
        free(line);
        return result;  // only name + type
    }

    // 3. There's more: extract value
    rest++;  // move past null terminator we injected
    while (isspace(*rest)) rest++;
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

    // Make a modifiable copy of the input
    char *line_copy = strdup(line);
    if (!line_copy) {
        free(tokens);
        return NULL;
    }

    char *token = strtok(line_copy, delimiters);
    while (token != NULL && count < MAX_TOKENS) {
        tokens[count] = strdup(token); // Duplicate the token
        if (!tokens[count]) {
            // On failure, clean up
            for (int i = 0; i < count; i++) free(tokens[i]);
            free(tokens);
            free(line_copy);
            return NULL;
        }
        count++;
        token = strtok(NULL, delimiters);
    }

    tokens[count] = NULL; // Null-terminate the array
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

