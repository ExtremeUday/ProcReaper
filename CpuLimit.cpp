#include <windows.h>
#include <VersionHelpers.h>
#include <stdio.h>
#include <stdlib.h>

int Error(const char* msg) {
    printf("%s (%d)\n", msg, ::GetLastError());
    return 1;
}

int main(int argc, const char* argv[]) {

    // Check Windows version+
    if (!::IsWindows8OrGreater()) {
        printf("CPU Rate control is only available on Windows 8 and later\n");
        return 1;
    }

    // Validate arguments
    if (argc < 3) {
        printf("Usage: CpuLimit <pid> [<pid> ... ] <percentage>\n");
        return 0;
    }

    // Create Job Object
    HANDLE hJob = ::CreateJobObject(nullptr, L"CpuRateJob");

    if (!hJob)
        return Error("Failed to create object");

    // Add processes to the job object
    for (int i = 1; i < argc - 1; i++) {

        int pid = atoi(argv[i]);

        HANDLE hProcess =
            ::OpenProcess(PROCESS_SET_QUOTA | PROCESS_TERMINATE, FALSE, pid);

        if (!hProcess) {
            printf("Failed to open handle to process %d (error=%d)\n",
                pid,
                ::GetLastError());
            continue;
        }

        if (!::AssignProcessToJobObject(hJob, hProcess)) {
            printf("Failed to assign process %d to job (error=%d)\n",
                pid,
                ::GetLastError());
        }
        else {
            printf("Added process %d to job\n", pid);
        }

        ::CloseHandle(hProcess);
    }

    // Configure CPU limit
    JOBOBJECT_CPU_RATE_CONTROL_INFORMATION info;

    info.CpuRate = atoi(argv[argc - 1]) * 100;

    info.ControlFlags =
        JOB_OBJECT_CPU_RATE_CONTROL_ENABLE |
        JOB_OBJECT_CPU_RATE_CONTROL_HARD_CAP;

    // Apply CPU limit
    if (!::SetInformationJobObject(
        hJob,
        JobObjectCpuRateControlInformation,
        &info,
        sizeof(info)))
    {
        return Error("Failed to set job limits");
    }

    printf("CPU limit set successfully.\n");

    printf("Press ENTER to quit.\n");

    char dummy[10];
    gets_s(dummy);

    ::CloseHandle(hJob);

    return 0;
}