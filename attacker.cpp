#include <windows.h>

int main() {
    HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

    CHAR inputBuffer[2048];
    DWORD charsRead, charsWritten;
    CHAR basePath[] = "\\\\192.168.24.247\\c$\\Users\\NativT\\Documents\\";
    CHAR fullPathA[MAX_PATH];
    CHAR fullPathB[MAX_PATH];
    CHAR fullPathC[MAX_PATH];

    while (1) {
        WriteConsoleA(hOut, "computer name: ", 17, &charsWritten, NULL);

        if (ReadFile(hIn, inputBuffer, sizeof(inputBuffer) - 1, &charsRead, NULL)) {
            if (charsRead >= 2) inputBuffer[charsRead - 2] = '\0';
            else inputBuffer[charsRead] = '\0';
        }
        else {
            WriteConsoleA(hOut, "Failed to read input.\n", 22, &charsWritten, NULL);
            return 1;
        }

        if (lstrcmpiA(inputBuffer, "exit") == 0) break;

        lstrcpyA(fullPathA, basePath); 
        lstrcatA(fullPathA, "A.txt");
        lstrcpyA(fullPathB, basePath); 
        lstrcatA(fullPathB, "B.txt");
        lstrcpyA(fullPathC, basePath); 
        lstrcatA(fullPathC, "C.txt");

        LPCSTR UncPathA = fullPathA;
        LPCSTR UncPathB = fullPathB;
        LPCSTR UncPathC = fullPathC;

        HANDLE hFile = CreateFileA(
            UncPathC,
            GENERIC_WRITE,
            FILE_SHARE_READ,
            NULL,
            CREATE_ALWAYS,
            FILE_ATTRIBUTE_NORMAL,
            NULL
        );
        if (hFile == INVALID_HANDLE_VALUE) {
            WriteConsoleA(hOut, "Failed to open file.\n", 20, &charsWritten, NULL);
            return 1;
        }

        if (!WriteFile(hFile, inputBuffer, (DWORD)strlen(inputBuffer), &charsWritten, NULL)) {
            WriteConsoleA(hOut, "Failed to write to file.\n", 25, &charsWritten, NULL);
            CloseHandle(hFile);
            return 1;
        }
        CloseHandle(hFile);

        while (1) {
            WriteConsoleA(hOut, ">: ", 3, &charsWritten, NULL);

            if (ReadFile(hIn, inputBuffer, sizeof(inputBuffer) - 1, &charsRead, NULL)) {
                if (charsRead >= 2) inputBuffer[charsRead - 2] = '\0';
                else inputBuffer[charsRead] = '\0';

                HANDLE hFileA = CreateFileA(
                    UncPathA,
                    GENERIC_WRITE,
                    FILE_SHARE_READ,
                    NULL,
                    CREATE_ALWAYS,
                    FILE_ATTRIBUTE_NORMAL,
                    NULL
                );

                if (hFileA == INVALID_HANDLE_VALUE) {
                    WriteConsoleA(hOut, "Failed to open file.\n", 20, &charsWritten, NULL);
                    return 1;
                }

                if (!WriteFile(hFileA, inputBuffer, (DWORD)strlen(inputBuffer), &charsWritten, NULL)) {
                    WriteConsoleA(hOut, "Failed to write to file.\n", 25, &charsWritten, NULL);
                    CloseHandle(hFileA);
                    return 1;
                }
                CloseHandle(hFileA);

                if (_stricmp(inputBuffer, "exit") == 0) {
                    while (!DeleteFileA(UncPathC)) {
                        WriteConsoleA(hOut, "Failed to delete c.txt\n", 23, &charsWritten, NULL);
                    }
                    break;
                }

                while (1) {
                    HANDLE hFileB = CreateFileA(
                        UncPathB,
                        GENERIC_READ,
                        FILE_SHARE_READ | FILE_SHARE_WRITE,
                        NULL,
                        OPEN_EXISTING,
                        FILE_ATTRIBUTE_NORMAL,
                        NULL
                    );

                    if (hFileB != INVALID_HANDLE_VALUE) {
                        DWORD bytesReadB;
                        char bufferB[2048];

                        while (ReadFile(hFileB, bufferB, sizeof(bufferB) - 1, &bytesReadB, NULL) && bytesReadB > 0) {
                            bufferB[bytesReadB] = '\0';
                            WriteConsoleA(hOut, bufferB, bytesReadB, &charsWritten, NULL);
                        }
                        WriteConsoleA(hOut, "\r\n", 2, &charsWritten, NULL);
                        CloseHandle(hFileB);

                        int cnt = 0;
                        while (!DeleteFileA(UncPathB)) {
                            WriteConsoleA(hOut, "Failed to delete b.txt\n", 23, &charsWritten, NULL);
                            cnt++;
                            if (cnt == 5) return 1;
                            Sleep(1000);
                        }
                        break;
                    }
                    Sleep(500);
                }
            }
            else {
                WriteConsoleA(hOut, "Failed to read input.\n", 22, &charsWritten, NULL);
                return 1;
            }
        }
    }
    return 0;
}
