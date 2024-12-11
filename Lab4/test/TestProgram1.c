#include <stdio.h>
#include "GCF.h"
#include "Pi.h"

int main() {
    int cmd;
    printf("Enter the command (1 for GCF, 2 for Pi, 3 to exit): ");
    while (scanf("%d", &cmd) != EOF) {
        if (cmd == 1) {
            int a, b;
            printf("Enter two numbers: ");
            if (scanf("%d %d", &a, &b) != 2) {
                printf("Invalid input. Please enter two integers.\n");
                while (getchar() != '\n'); // Очистка буфера ввода
                continue;
            }
            printf("GCF (Euclid): %d\n", GCF_Euclid(a, b));
        } else if (cmd == 2) {
            int k;
            printf("Enter the number of terms for Pi (Leibniz): ");
            if (scanf("%d", &k) != 1 || k < 0) {
                printf("Invalid input. Please enter a non-negative integer.\n");
                continue;
            }
            printf("Pi (Leibniz): %.6f\n", Pi_Leibniz(k));
        } else if (cmd == 3) {
            return 0;
        } else {
            printf("Unknown command. Please enter 1 for GCF or 2 for Pi.\n");
        }
        printf("Enter the command (1 for GCF, 2 for Pi, 3 to exit): ");
    }
    return 0;
}