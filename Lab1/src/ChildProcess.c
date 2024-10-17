#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "File name not provided\n");
        return 1;
    }
    
    FILE *file = fopen(argv[1], "r");
    if (!file) {
        fprintf(stderr, "Failed to open the file\n");
        return 1;
    }
    
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        float sum = 0;
        char *token = strtok(line, " ");
        
        while (token != NULL) {
            sum += atof(token);
            token = strtok(NULL, " ");
        }
        
        printf("Sum: %.2f\n", sum);
    }
    
    fclose(file);
    return 0;
}
