#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 256

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "where is file name?\n");
        return 1;
    }

    HANDLE hFile = CreateFile(
        argv[1],            
        GENERIC_READ,     
        FILE_SHARE_READ,    
        NULL,               
        OPEN_EXISTING,      
        FILE_ATTRIBUTE_NORMAL, 
        NULL                
    );

    if (hFile == INVALID_HANDLE_VALUE) {
        fprintf(stderr, "failed opening file\n");
        return 1;
    }

    char buffer[BUFFER_SIZE];
    DWORD bytesRead;
    char line[BUFFER_SIZE];
    int linePos = 0;

    while (ReadFile(hFile, buffer, BUFFER_SIZE, &bytesRead, NULL) && bytesRead > 0) {
        for (DWORD i = 0; i < bytesRead; i++) {
            char c = buffer[i];

            if (c == '\n' || c == '\r') {
                if (linePos > 0) {
                    line[linePos] = '\0';

                    float sum = 0;
                    char *token = strtok(line, " ");

                    while (token != NULL) {
                        sum += atof(token);
                        token = strtok(NULL, " ");
                    }

                    printf("Sum: %.2f\n", sum);
                    linePos = 0; 
                }
            } else {
                if (linePos < BUFFER_SIZE - 1) {
                    line[linePos++] = c;
                } else {
                    fprintf(stderr, "too long string\n");
                    CloseHandle(hFile);
                    return 1;
                }
            }
        }
    }

    
    if (linePos > 0) {
        line[linePos] = '\0';
        float sum = 0;
        char *token = strtok(line, " ");

        while (token != NULL) {
            sum += atof(token);
            token = strtok(NULL, " ");
        }

        printf("sum: %.2f\n", sum);
    }

    CloseHandle(hFile);
    return 0;
}