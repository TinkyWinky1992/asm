#include <stdio.h>
#include <stdlib.h>
#include "./parser/parser.h"
#include "./utils.h"
#include "./builder/builder.h"
#include <string.h>

#define MAX_LlINE 128
#define MAX_WORD_LENGTH 100


char *read_line(FILE* file) {
    int size = MAX_LlINE;
    char *buffer = malloc(size);
    if(buffer == NULL) {
        printf("we have problem with malloc\n");
        free(buffer);
        return NULL;
       
    }

    int character = '\0';
    int index = 0;

    while((character = fgetc(file)) != '\n' && character != EOF) {
        buffer[index++] = character;

        if(index >= size){
            size *= 2;
            char *new_buffer = realloc(buffer, size);
            if(new_buffer == NULL) {
                free(buffer);
                return NULL;
            }
            buffer = new_buffer;
        }
    }
    if(index == 0 && character == EOF) {
        free(buffer);
        return NULL;
    }

    buffer[index] = '\0';
    return buffer;
}



void RegisterLine(char* line, FILE *file) {
    if (line == NULL) {
        printf("Error: line is NULL\n");
        return;
    }

    int i = 0;
    int word_start = 0;
    char word[MAX_WORD_LENGTH];

    while (line[i] != '\0') {
        // Skip any leading spaces
        i = SkippingSpaces(i, line);
        if (line[i] == '\0') break;

        // Mark start of the word
        word_start = i;

        // Find the end of the word (until next space or end)
        while (line[i] != '\0' && line[i] != ' ' && line[i] != '\t') {
            i++;
        }

        int word_len = i - word_start;
        if (word_len >= MAX_WORD_LENGTH) {
            continue; // skip this word
        }

        // Copy the word into the local buffer
        strncpy(word, &line[word_start], word_len);
        word[word_len] = '\0'; // null-terminate

        toLowerCase(word); // normalize the word

        // Debug print
        //printf("Parsed word: \"%s\"\n", word);

        // Now handle word logic
        //printf("word: %s\n", word);
        if (isSymbol(word) == 1) {
          //  printf("Detected symbol: %s\n", word);
          buildSymbolTable(word, file, line);
        }
        else if(isMacro(word, file, line) == 1) {
            buildMacroTable(word, file, line);
            printMacroTable();
            
           // printMacroTable();
        }
        else if (isCommand(word)== 1 ) {

          printf("Detected command: %s\n", word);
          buildCommandTable(word, file, line);

        }
    
        

        // Error checking
        if (flagEn == -1) {
            printf("You have a problem in line: \"%s\"\n", line);
            return;
        }
    }
}


int main(int argc, char *argv[]) {
    FILE *fptr = fopen("src/test.asm", "r");
    if (!fptr) {
        printf("Could not open file\n");
        return -1;
    }

    char* line;
    
    while ((line = read_line(fptr)) != NULL) {
       // printf("line: %s\n", line);  // Optional debug print

        RegisterLine(line, fptr);    // Process the line

        free(line);  // Free the memory allocated in read_line
    }

    fclose(fptr);
    return 0;
}



