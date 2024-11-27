#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

#define MAPPING_NAME "Local\\SharedMemoryExample"
#define BUFFER_SIZE 256

// Функция для вывода текста на консоль с использованием WriteConsole
void printMessage(HANDLE hConsole, const char* message) {
    DWORD written;
    WriteConsole(hConsole, message, strlen(message), &written, NULL);
}

int main() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hConsole == INVALID_HANDLE_VALUE) {
        return 1;
    }

    // Открываем отображаемый файл
    HANDLE hMapFile = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, MAPPING_NAME);
    if (hMapFile == NULL) {
        DWORD error = GetLastError();
        char errorMessage[256];
        sprintf(errorMessage, "Could not open file mapping object. Error code: %d\n", error);
        printMessage(hConsole, errorMessage);
        return 1;
    }

    // Получаем указатель на память
    LPVOID pBuf = MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, BUFFER_SIZE);
    if (pBuf == NULL) {
        printMessage(hConsole, "Could not map view of file.\n");
        CloseHandle(hMapFile);
        return 1;
    }

    // Чтение имени файла из памяти
    char filename[BUFFER_SIZE];
    strcpy(filename, (char*)pBuf);

    // Открытие файла через WinAPI
    HANDLE hFile = CreateFile(
        filename,               // Имя файла
        GENERIC_READ,           // Открыть для чтения
        FILE_SHARE_READ,        // Разрешить совместное чтение
        NULL,                   // Атрибуты безопасности
        OPEN_EXISTING,          // Открыть существующий файл
        FILE_ATTRIBUTE_NORMAL,  // Обычный файл
        NULL                    // Нет шаблона файла
    );

    if (hFile == INVALID_HANDLE_VALUE) {
        sprintf((char*)pBuf, "Failed to open file.\n");
        UnmapViewOfFile(pBuf);
        CloseHandle(hMapFile);
        return 1;
    }

    // Чтение содержимого файла и подсчёт суммы
    char buffer[BUFFER_SIZE];
    DWORD bytesRead;
    float sum = 0.0;

    while (ReadFile(hFile, buffer, sizeof(buffer) - 1, &bytesRead, NULL) && bytesRead > 0) {
        buffer[bytesRead] = '\0';  // Завершаем строку
        char* token = strtok(buffer, " \n");
        while (token != NULL) {
            sum += atof(token);  // Преобразуем и суммируем числа
            token = strtok(NULL, " \n");
        }
    }

    // Закрываем файл
    CloseHandle(hFile);

    // Запись результата обратно в отображаемую память
    sprintf((char*)pBuf, "Sum: %.2f", sum);

    // Освобождение ресурсов
    UnmapViewOfFile(pBuf);
    CloseHandle(hMapFile);

    return 0;
}
