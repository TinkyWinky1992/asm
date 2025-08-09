#include <stdio.h>
#include <stdlib.h>
#include "./parser/parser.h"
#include "./utils.h"
#include "./builder/builder.h"
#include <string.h>
#include "./output/output.h"

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

    int i = 0;
    int word_start = 0;
    char word[MAX_WORD_LENGTH];

    while (line[i] != '\0') {
        
        // Skip any leading spaces
        i = SkippingSpaces(i, line);
        if (line[i] == '\0') break;
       // printf("line in registerline: %s\n", line);
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
         //converting the line to lower case

        toLowerCase(line);
        if (isSymbol(word) == 1)
          buildSymbolTable(word, file, line);
        
        else if(isMacro(word, file, line) == 1)
                printMacroTable();
        else if (isCommand(word)== 1 ) {
         
          buildCommandTable(word, file, line);

        }
    
        

        // Error checking
        if (flagEn == -1) {
            printf("You have a problem in line: \"%s\"\n", line);
            return;
        }
    }
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <ctype.h>

// Function to check if a filename ends with ".as"
int ends_with_as(const char *filename) {
    size_t len = strlen(filename);
    if (len < 3) return 0;
    const char *ext = filename + len - 3; // Get the last three characters
    return (tolower(ext[0]) == '.' && tolower(ext[1]) == 'a' && tolower(ext[2]) == 's');
}

int main(int argc, char *argv[]) {
    struct dirent *entry;
    DIR *dir = opendir("tests");

    if (!dir) {
        perror("Could not open TEST directory");
        return 1;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (ends_with_as(entry->d_name)) {
            char filepath[512];
            snprintf(filepath, sizeof(filepath), "tests/%s", entry->d_name);

            printf("Processing file: %s\n", filepath);

            FILE *fptr = fopen(filepath, "r");
            if (!fptr) {
                perror("Error opening file");
                continue;
            }

            char *line;
            while ((line = read_line(fptr)) != NULL) {
                RegisterLine(line, fptr);
                free(line);
            }

            fclose(fptr);

            // After processing the file, generate output files
            entryToFile();
            externToFile();
            ObMemoryToFile();
        }
    }

    closedir(dir);
    return 0;
}



