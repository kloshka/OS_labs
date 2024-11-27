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

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printMessage(GetStdHandle(STD_OUTPUT_HANDLE), "Usage: parent.exe <filename>\n");
        return 1;
    }

    const char* filename = argv[1];

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE); // Получаем дескриптор консоли
    if (hConsole == INVALID_HANDLE_VALUE) {
        fprintf(stderr, "Error getting console handle (%d).\n", GetLastError());
        return 1;
    }

    // Создание отображаемого файла
    HANDLE hMapFile = CreateFileMapping(
        INVALID_HANDLE_VALUE,
        NULL,
        PAGE_READWRITE,
        0, BUFFER_SIZE,
        MAPPING_NAME
    );

    if (hMapFile == NULL) {
        DWORD error = GetLastError();
        char errorMessage[256];
        sprintf(errorMessage, "Could not create file mapping object. Error code: %d\n", error);
        printMessage(hConsole, errorMessage);
        return 1;
    }

    // Получаем указатель на отображаемую память
    LPVOID pBuf = MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, BUFFER_SIZE);
    if (pBuf == NULL) {
        printMessage(hConsole, "Could not map view of file.\n");
        CloseHandle(hMapFile);
        return 1;
    }

    // Записываем имя файла в память
    sprintf((char*)pBuf, "%s", filename);

    // Создание дочернего процесса
    PROCESS_INFORMATION pi;
    STARTUPINFO si = { sizeof(si) };

    if (!CreateProcess(NULL, "child.exe", NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
        printMessage(hConsole, "CreateProcess failed.\n");
        UnmapViewOfFile(pBuf);
        CloseHandle(hMapFile);
        return 1;
    }

    // Ожидание завершения дочернего процесса
    WaitForSingleObject(pi.hProcess, INFINITE);

    // Читаем результат из памяти и выводим его
    printMessage(hConsole, "Result from child process: ");
    printMessage(hConsole, (char*)pBuf);
    printMessage(hConsole, "\n");

    // Освобождение ресурсов
    UnmapViewOfFile(pBuf);
    CloseHandle(hMapFile);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return 0;
}
