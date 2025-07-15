#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#ifndef BUFFER_SIZE
# define BUFFER_SIZE 64
#endif

char* replace_all(const char* str, const char* find) {
    int find_len = strlen(find);
    int count = 0;
    const char* pos = str;
    
    char* replace = malloc(find_len + 1);
    for (int i = 0; i < find_len; i++) {
        replace[i] = '*';
    }

    replace[find_len] = '\0';
    while ((pos = memmem(pos, strlen(pos), find, find_len))) {
        count++;
        pos += find_len;
    }

    char* result = malloc(strlen(str) + 1);
    const char* src = str;
    char* dst = result;
    
    while ((pos = memmem(src, strlen(src), find, find_len))) {
        memcpy(dst, src, pos - src);
        dst += pos - src;
        memcpy(dst, replace, find_len);
        dst += find_len;
        src = pos + find_len;
    }
    strcpy(dst, src);
    free(replace);
    return result;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <word>\n", argv[0]);
        return 1;
    }
    
    char* search_word = argv[1];
    char buffer[BUFFER_SIZE];
    char* line = malloc(1);
    line[0] = '\0';
    int line_size = 0;
    ssize_t bytes_read;
    
    while ((bytes_read = read(STDIN_FILENO, buffer, BUFFER_SIZE - 1)) > 0) {
        buffer[bytes_read] = '\0';
        
        for (int i = 0; i < bytes_read; i++) {
            line = realloc(line, line_size + 2);
            line[line_size++] = buffer[i];
            line[line_size] = '\0';
            
            if (buffer[i] == '\n') {
                line[line_size - 1] = '\0';
                char* result = replace_all(line, search_word);
                printf("%s\n", result);
                free(result);
                free(line);
                line = malloc(1);
                line[0] = '\0';
                line_size = 0;
            }
        }
    }
    
    if (line_size > 0) {
        char* result = replace_all(line, search_word);
        printf("%s\n", result);
        free(result);
    }
    
    free(line);
    return 0;
}
