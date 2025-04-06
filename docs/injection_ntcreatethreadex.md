# NtCreateThreadEx Injection

This example demonstrates **remote process injection using the undocumented `NtCreateThreadEx` API**. Unlike the conventional `CreateRemoteThread`, `NtCreateThreadEx` resides in `ntdll.dll` and provides lower-level access to thread creation capabilities. Because it bypasses certain user-mode APIs and hooks, it is sometimes used to evade detection by security products monitoring standard injection patterns.

This method is widely studied in red teaming and malware analysis for its stealthier characteristics.

---

## Technique Overview

The goal of this technique is to inject shellcode into a target process and execute it by creating a new thread using `NtCreateThreadEx`. The basic workflow is:

1. Open a handle to the target process  
2. Allocate memory in the target process (`VirtualAllocEx`)  
3. Write the shellcode into the allocated memory (`WriteProcessMemory`)  
4. Dynamically resolve and call `NtCreateThreadEx` to start execution

---

## Step-by-Step Breakdown

### 1. Open a Handle to the Target Process

To interact with another process, a valid handle must be obtained using `OpenProcess`. Full access rights are typically requested for memory and thread manipulation.

```cpp
HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
```

If this operation fails, the injection cannot proceed.

---

### 2. Resolve NtCreateThreadEx

Because `NtCreateThreadEx` is undocumented, its address must be manually resolved from `ntdll.dll` at runtime:

```cpp
HMODULE hNtdll = GetModuleHandle(L"ntdll.dll");
pNtCreateThreadEx NtCreateThreadEx = (pNtCreateThreadEx)GetProcAddress(hNtdll, "NtCreateThreadEx");
```

If this step fails, it’s likely due to changes in API exports or an unsupported OS version.

---

### 3. Allocate Memory in the Remote Process

Allocate memory in the target process to hold the shellcode. This is performed using `VirtualAllocEx`:

```cpp
LPVOID alloc = VirtualAllocEx(
    hProcess, nullptr, sizeof(shellcode), MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE
);
```

The memory region must be marked executable to allow shellcode execution.

---

### 4. Write Shellcode into Remote Memory

The shellcode is written into the allocated region using `WriteProcessMemory`:

```cpp
WriteProcessMemory(hProcess, alloc, shellcode, sizeof(shellcode), nullptr);
```

In test environments, this might be a NOP sled or a harmless payload (e.g., MessageBox). For real use cases, this would contain meaningful shellcode.

---

### 5. Invoke NtCreateThreadEx

With shellcode in place, a remote thread is created via `NtCreateThreadEx`, pointing to the memory region containing the payload:

```cpp
NTSTATUS status = NtCreateThreadEx(
    &hThread, THREAD_ALL_ACCESS, nullptr, hProcess, alloc, nullptr, FALSE, 0, 0, 0, nullptr
);
```

If the function returns `STATUS_SUCCESS` (typically 0), the thread was successfully created.

---

### 6. Cleanup

Finally, the process handle is closed to release resources:

```cpp
CloseHandle(hProcess);
```

---

## Key APIs and Internals

### `NtCreateThreadEx`
Undocumented native API used to spawn a thread in a remote process. Not present in official Windows headers and must be resolved manually from `ntdll.dll`.

### `VirtualAllocEx`
Allocates memory within a remote process. Required for placing the payload in the target’s address space.

### `WriteProcessMemory`
Performs the actual memory write to the remote process. Transfers shellcode or any other payload data.

### `OpenProcess`
Grants access to the target process for memory and thread operations.

### `GetProcAddress`
Used to locate `NtCreateThreadEx` at runtime from `ntdll.dll`.

---

## Detection and Limitations

### Detection
- Although stealthier than `CreateRemoteThread`, `NtCreateThreadEx` is still subject to detection by EDRs that monitor memory protection flags, suspicious memory patterns, or undocumented API usage.

### Shellcode
- The example uses NOPs as placeholder shellcode. For real use, functional payloads such as reverse shells or DLL loaders are typically injected.

### Stability
- Minimal error handling is used here. In practice, robust error checking and parameter validation are required for stability and debugging.

### Legal and Ethical Use
- This technique is documented strictly for research and educational purposes. It must not be used against systems without explicit permission.

---

## Summary

`NtCreateThreadEx` offers a lower-level, stealthier alternative to traditional thread injection methods. While not inherently malicious, its misuse by malware has made it a focus in defensive tooling. Understanding its internals is essential for red teamers, reverse engineers, and those studying Windows internals.
