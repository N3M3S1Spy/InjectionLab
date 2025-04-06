# Manual DLL Mapping (Advanced Injection)




---

## Overview

This technique demonstrates **manual DLL mapping** — a method of injecting a DLL into a target process without using the standard `LoadLibraryA` function. Instead of relying on traditional Windows API calls, we manually allocate memory in the target process, write the DLL path into it, and invoke the function directly. This approach provides a stealthier injection method that can be harder for antivirus (AV) software to detect.

**Goal:** To provide a deeper understanding of process memory manipulation and function resolution by implementing DLL injection at a lower level.

---

## Use Case

Manual DLL mapping is particularly relevant for:

- **Malware authors** seeking to bypass API hook-based detection mechanisms.
- **Penetration testers** attempting to evade AV/EDR detections.
- **Red team operators** who need more control and stealth during engagements.

---

## Step-by-Step Breakdown

### 1. Target Identification

```cpp
std::cout << "[>] Enter the PID of the target process: ";
std::cin >> pid;
```

The user is prompted to provide the **Process ID (PID)** of the target process, which can be obtained via tools like Task Manager or Process Explorer.

---

### 2. Accessing the Process

```cpp
OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
```

A handle to the target process is retrieved with full access rights. This is necessary to:

- Allocate memory within the process
- Write to that memory
- Create threads within the process

---

### 3. Allocating Remote Memory

```cpp
VirtualAllocEx(hProcess, nullptr, dllPath.length() + 1, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
```

Memory is allocated in the target process to store the DLL path. The memory is marked as readable and writable to support writing the string.

---

### 4. Writing the DLL Path

```cpp
WriteProcessMemory(hProcess, remoteMemory, dllPath.c_str(), dllPath.length() + 1, nullptr);
```

The DLL path string is written to the allocated memory. It must be null-terminated, as expected by `LoadLibraryA`.

---

### 5. Resolving LoadLibraryA

```cpp
pLoadLibraryA pLoadLib = (pLoadLibraryA)GetProcAddress(hKernel32, "LoadLibraryA");
```

The address of `LoadLibraryA` is retrieved from `kernel32.dll` using `GetProcAddress`. This address will be used as the entry point for the remote thread.

---

### 6. Creating the Remote Thread

```cpp
CreateRemoteThread(hProcess, nullptr, 0, (LPTHREAD_START_ROUTINE)pLoadLib, remoteMemory, 0, nullptr);
```

A remote thread is created in the target process. It begins execution at the address of `LoadLibraryA` and uses the DLL path as its argument, resulting in the DLL being loaded into the target process.

---

### 7. Synchronizing Execution

```cpp
WaitForSingleObject(hThread, INFINITE);
```

The calling process waits for the remote thread to complete execution. Once complete, the DLL is fully loaded and initialized.

---

## Summary Table

| API Function           | Purpose                                                       |
|------------------------|---------------------------------------------------------------|
| `OpenProcess`          | Acquires a handle to the target process                        |
| `VirtualAllocEx`       | Allocates memory in the target process                         |
| `WriteProcessMemory`   | Writes the DLL path into the allocated memory                  |
| `GetProcAddress`       | Retrieves the address of `LoadLibraryA`                        |
| `CreateRemoteThread`   | Executes `LoadLibraryA` in the target process to load the DLL  |

---

## Security Considerations

- **Privilege Requirements:** Administrative privileges may be required to open certain target processes.
- **AV/EDR Detection:** Many security products monitor remote thread creation and memory writes. Even though this technique avoids using `LoadLibraryA` directly, the presence of shellcode or suspicious memory patterns can still be flagged.
- **Detection Evasion:** Manual mapping techniques attempt to evade detection by minimizing or bypassing common API usage, but they are not inherently undetectable.

---

## Educational Value

- **Granular Control:** Offers insight into low-level process interaction beyond standard API usage.
- **Foundation for Advanced Techniques:** Forms the basis for techniques like reflective DLL injection or full manual PE mapping, which involve parsing and loading PE structures manually.
- **AV Evasion Concepts:** Introduces the idea of avoiding standard behavior patterns monitored by security solutions.

---

## Source Code

The full implementation is available here:  
[`../src/injection_manual_dll_mapping.cpp`](../src/injection_manual_dll_mapping.cpp)
