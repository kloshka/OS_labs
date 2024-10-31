#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

int main() {
    char fileName[256];
    printf("Enter the name of the file: ");
    scanf("%s", fileName);
    
    // создание канала
    HANDLE pipeRead, pipeWrite;
    SECURITY_ATTRIBUTES sa = { sizeof(SECURITY_ATTRIBUTES), NULL, TRUE };
    if (!CreatePipe(&pipeRead, &pipeWrite, &sa, 0)) {
        fprintf(stderr, "Pipe creation failed\n");
        return 1;
    }
    
    // создание дочернего
    PROCESS_INFORMATION pi;
    STARTUPINFO si;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    si.hStdOutput = pipeWrite;
    si.dwFlags |= STARTF_USESTDHANDLES;
    
    char cmdLine[512];
    sprintf(cmdLine, "ChildProcess.exe %s", fileName);
    
    if (!CreateProcess(NULL, cmdLine, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi)) {
        fprintf(stderr, "Process creation failed\n");
        return 1;
    }
    // закрытие дескриптора записи в канал, так как он больше не нужен в родительском процессе
    CloseHandle(pipeWrite);
    
    // чтение с канала
    char buffer[128];
    DWORD bytesRead;
    while (ReadFile(pipeRead, buffer, sizeof(buffer) - 1, &bytesRead, NULL)) {
        buffer[bytesRead] = '\0';
        printf("%s", buffer);
    }
    
    // ожидание завершения дочернего процесса
    WaitForSingleObject(pi.hProcess, INFINITE);
    
    // Close handles
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    CloseHandle(pipeRead);
    
    return 0;
}
