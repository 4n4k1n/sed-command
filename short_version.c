#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_SIZE 64

int main(int argc, char* argv[]) {
    if (argc != 2) return 1;
    
    char* find = argv[1];
    int find_len = strlen(find);
    char buffer[BUFFER_SIZE];
    char* line = malloc(1);
    int line_size = 0;
    ssize_t bytes_read;
    
    *line = 0;
    while ((bytes_read = read(0, buffer, BUFFER_SIZE - 1)) > 0) {
        buffer[bytes_read] = 0;
        for (int i = 0; i < bytes_read; i++) {
            line = realloc(line, line_size + 2);
            line[line_size++] = buffer[i];
            line[line_size] = 0;
            
            if (buffer[i] == '\n') {
                line[--line_size] = 0;
                char* pos;
                while ((pos = memmem(line, line_size, find, find_len))) {
                    for (int j = 0; j < find_len; j++) pos[j] = '*';
                }
                printf("%s\n", line);
                *line = 0;
                line_size = 0;
            }
        }
    }
    
    if (line_size > 0) {
        char* pos;
        while ((pos = memmem(line, line_size, find, find_len))) {
            for (int j = 0; j < find_len; j++) pos[j] = '*';
        }
        printf("%s\n", line);
    }
    
    free(line);
    return 0;
}
