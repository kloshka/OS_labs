#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

int main() {
    char fileName[256];
    printf("Enter the name of the file: ");
    scanf("%s", fileName);
    
    // создание канала
    HANDLE pipeRead, pipeWrite; //дескрипторы для чтения и записи в канал.
    SECURITY_ATTRIBUTES sa = { sizeof(SECURITY_ATTRIBUTES), NULL, TRUE }; /*bInheritHandle = TRUE*/
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
    /*
    Флаг STARTF_USESTDHANDLES:
Этот флаг сообщает CreateProcess, что STARTUPINFO содержит действительные дескрипторы для 
стандартных потоков, которые процесс должен использовать вместо стандартных консольных дескрипторов.
Дескрипторы, описанные ранее:*/
    
    char cmdLine[512];
    sprintf(cmdLine, "ChildProcess.exe %s", fileName); 
    
    if (!CreateProcess(NULL, cmdLine, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi)) {
        fprintf(stderr, "Process creation failed\n");
        return 1;
    }
    /*
    Второй параметр — это строка команд для запуска процесса (с аргументами).
    Параметр TRUE для bInheritHandles.
    */

    CloseHandle(pipeWrite);
    
    // чтение с канала в buffer
    char buffer[128];
    DWORD bytesRead;//переменная для хранения количества байтов
    while (ReadFile(pipeRead, buffer, sizeof(buffer) - 1, &bytesRead, NULL)) {
        buffer[bytesRead] = '\0';
        printf("%s", buffer);
    }//ReadFile блокируется (ждет), пока не будет доступно для чтения
    // достаточно данных, отправленных дочерним процессом.
    
    WaitForSingleObject(pi.hProcess, INFINITE);
    
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    CloseHandle(pipeRead);
    
    return 0;
}
