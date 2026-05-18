#include <windows.h>
#include <tlhelp32.h>
#include <psapi.h>
#include <stdio.h>

void GetMemoryUsage(DWORD pid) {
    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);

    if (hProcess == NULL) {
        wprintf(L"%-15s", L"N/A");
        return;
    }

    PROCESS_MEMORY_COUNTERS pmc;

    if (GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc))) {
        double memMB = (double)pmc.WorkingSetSize / (1024 * 1024);
        wprintf(L"%-10.2f MB", memMB);
    }
    else {
        wprintf(L"%-15s", L"N/A");
    }

    CloseHandle(hProcess);
}

int main() {

    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if (hSnapshot == INVALID_HANDLE_VALUE) {
        wprintf(L"Failed to create snapshot. Error: %lu\n", GetLastError());
        return 1;
    }


    PROCESSENTRY32W pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32W);


    if (!Process32FirstW(hSnapshot, &pe32)) {
        wprintf(L"Failed to get first process. Error: %lu\n", GetLastError());
        CloseHandle(hSnapshot);
        return 1;
    }


    wprintf(L"\n");
    wprintf(L"================================================================\n");
    wprintf(L"                   PROCESS LIST VIEWER                         \n");
    wprintf(L"================================================================\n");
    wprintf(L"%-40s %-10s %-15s\n", L"Process Name", L"PID", L"Memory Usage");
    wprintf(L"----------------------------------------------------------------\n");


    do {
        wprintf(L"%-40s %-10lu ", pe32.szExeFile, pe32.th32ProcessID);
        GetMemoryUsage(pe32.th32ProcessID);
        wprintf(L"\n");
       
    } while (Process32NextW(hSnapshot, &pe32));

    wprintf(L"================================================================\n");
    wprintf(L"\n");


    CloseHandle(hSnapshot);

    wprintf(L"Press Enter to exit...");
    getchar();

    return 0;
}