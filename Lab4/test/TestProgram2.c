#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

// Типы функций, загружаемых из динамических библиотек
typedef int (*GCF_FUNC)(int, int);
typedef float (*PI_FUNC)(int);

// Функция для загрузки библиотеки
void loadLibrary(HMODULE *lib, const char *name) {
    *lib = LoadLibrary(name);
    if (!*lib) {
        printf("Failed to load library: %s\n", name);
        exit(EXIT_FAILURE);
    }
}

int main() {
    HMODULE lib1 = NULL, lib2 = NULL;
    GCF_FUNC GCF = NULL;
    PI_FUNC Pi = NULL;

    // Изначальная загрузка библиотек
    loadLibrary(&lib1, "libGCF_Euclid.dll");
    loadLibrary(&lib2, "libPi_Leibniz.dll");

    GCF = (GCF_FUNC)(void*)GetProcAddress(lib1, "GCF_Euclid");
    if (!GCF) {
        printf("Failed to load function GCF_Euclid from libGCF_Euclid.dll\n");
        FreeLibrary(lib1);
        exit(EXIT_FAILURE);
    }

    Pi = (PI_FUNC)(void*)GetProcAddress(lib2, "Pi_Leibniz");
    if (!Pi) {
        printf("Failed to load function Pi_Leibniz from libPi_Leibniz.dll\n");
        FreeLibrary(lib2);
        exit(EXIT_FAILURE);
    }

    int cmd;
    printf("Enter the command (0 to switch libraries, 1 for GCF, 2 for Pi, 3 to exit): ");
    while (scanf("%d", &cmd) != EOF) {
        if (cmd == 0) {
            // Переключение библиотек
            static int gcfFlag = 0, piFlag = 0; // Начальные значения: 0
            static int firstSwitch = 1;        // Флаг для первого переключения

            FreeLibrary(lib1);
            FreeLibrary(lib2);

            if (firstSwitch) {
                // При первом переключении загружаем Naive и Wallis
                loadLibrary(&lib1, "libGCF_Naive.dll");
                GCF = (GCF_FUNC)(void*)GetProcAddress(lib1, "GCF_Naive");

                loadLibrary(&lib2, "libPi_Wallis.dll");
                Pi = (PI_FUNC)(void*)GetProcAddress(lib2, "Pi_Wallis");

                firstSwitch = 0; // Сбрасываем флаг после первого переключения
            } else {
                // После первого переключения чередуем библиотеки
                if (gcfFlag) {
                    loadLibrary(&lib1, "libGCF_Euclid.dll");
                    GCF = (GCF_FUNC)(void*)GetProcAddress(lib1, "GCF_Euclid");
                } else {
                    loadLibrary(&lib1, "libGCF_Naive.dll");
                    GCF = (GCF_FUNC)(void*)GetProcAddress(lib1, "GCF_Naive");
                }

                if (piFlag) {
                    loadLibrary(&lib2, "libPi_Leibniz.dll");
                    Pi = (PI_FUNC)(void*)GetProcAddress(lib2, "Pi_Leibniz");
                } else {
                    loadLibrary(&lib2, "libPi_Wallis.dll");
                    Pi = (PI_FUNC)(void*)GetProcAddress(lib2, "Pi_Wallis");
                }

                gcfFlag = !gcfFlag; // Переключаем флаг
                piFlag = !piFlag;   // Переключаем флаг
            }

            if (!GCF || !Pi) {
                printf("Failed to reload functions after switching libraries.\n");
                exit(EXIT_FAILURE);
            }

            printf("Switched to another library.\n");
        } else if (cmd == 1) {
            // Вычисление GCF
            int a, b;
            printf("Enter two numbers: ");
            if (scanf("%d %d", &a, &b) != 2) {
                printf("Invalid input. Please enter two integers.\n");
                while (getchar() != '\n');
                continue;
            }
            printf("GCF: %d\n", GCF(a, b));
        } else if (cmd == 2) {
            // Вычисление Pi
            int k;
            printf("Enter the number of terms for Pi: ");
            if (scanf("%d", &k) != 1 || k < 0) {
                printf("Invalid input. Please enter a non-negative integer.\n");
                while (getchar() != '\n');
                continue;
            }
            printf("Pi: %.6f\n", Pi(k));
        } else if (cmd == 3) {
            // Завершение программы
            FreeLibrary(lib1);
            FreeLibrary(lib2);
            return 0;
        } else {
            printf("Unknown command.\n");
        }
        printf("Enter the command (0 to switch libraries, 1 for GCF, 2 for Pi, 3 to exit): ");
    }

    // Освобождение библиотек перед завершением
    FreeLibrary(lib1);
    FreeLibrary(lib2);
    return 0;
}