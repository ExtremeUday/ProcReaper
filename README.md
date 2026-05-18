# ProcReaper 🔪
> A lightweight Windows process enumeration tool built in C using the Win32 API.

![Language](https://img.shields.io/badge/Language-C-blue)
![Platform](https://img.shields.io/badge/Platform-Windows-lightgrey)
![API](https://img.shields.io/badge/API-Win32-orange)

---

## Overview

ProcReaper enumerates all running processes on a Windows system and displays their name, PID, and memory usage. Built as part of learning Windows Internals and the Win32 API.

---

## Output

```
================================================================
                   PROCESS LIST VIEWER
================================================================
Process Name                             PID        Memory Usage
----------------------------------------------------------------
System                                   4          N/A
chrome.exe                               1234       320.56 MB
notepad.exe                              5678       8.12 MB
explorer.exe                             9012       112.34 MB
================================================================
```

---

## APIs Used

| API | Purpose |
|-----|---------|
| `CreateToolhelp32Snapshot` | Takes a snapshot of all running processes |
| `Process32FirstW` | Retrieves the first process from the snapshot |
| `Process32NextW` | Iterates through remaining processes |
| `OpenProcess` | Opens a process handle for memory querying |
| `GetProcessMemoryInfo` | Retrieves memory usage of a process |
| `CloseHandle` | Closes open handles to prevent memory leaks |

---

## Build Instructions

**Requirements**
- Windows 10/11
- Visual Studio 2019 or later

**Steps**
1. Clone the repo
   ```
   git clone https://github.com/ExtremeUday/ProcReaper
   ```
2. Open `ProcReaper.sln` in Visual Studio
3. Link `psapi.lib`  Project → Properties → Linker → Input → Additional Dependencies
4. Build the solution  `Ctrl + Shift + B`
5. Run as Administrator for full process visibility

---

## Why Run as Administrator?

Some system-level processes deny access to unprivileged callers. Running as admin allows ProcReaper to query memory info for a wider range of processes. Processes that still deny access will show `N/A` under Memory Usage  this is expected behavior.

---

## What I Learned

- How Windows manages processes internally
- Using `PROCESSENTRY32W` and the Toolhelp32 snapshot API
- Difference between ANSI and Wide character APIs in Win32
- Proper handle management to avoid memory leaks
- How `OpenProcess` access rights work

---

## Part of My Learning Path

This tool was built while studying **Windows Internals** (Pavel Yosifovich)

