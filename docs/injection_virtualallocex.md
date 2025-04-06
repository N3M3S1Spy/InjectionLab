# VirtualAllocEx + CreateRemoteThread

This is one of the oldest and most widely documented code injection techniques on the Windows platform.

---

## Overview

The method involves a simple but powerful sequence of Windows API calls:

1. Opening a handle to a target process  
2. Allocating memory within that process using `VirtualAllocEx`  
3. Writing shellcode or other payload data via `WriteProcessMemory`  
4. Launching execution of the payload using `CreateRemoteThread`

This results in arbitrary code being executed within the context of the remote process.

---

## Why It Works

Windows allows inter-process memory operations under specific privileges. If the calling process has appropriate access rights, it can:

- Allocate memory in another process  
- Write data into that memory  
- Spawn a thread inside the target process

These capabilities exist for legitimate use cases such as debugging or inter-process communication.

---

## Why It Gets Detected

EDR systems commonly monitor and flag the classic injection sequence:

```
VirtualAllocEx → WriteProcessMemory → CreateRemoteThread
```

This detection becomes more likely when:

- The memory region is marked as `PAGE_EXECUTE_READWRITE`  
- The target process is commonly abused (e.g., `explorer.exe`, `svchost.exe`, `notepad.exe`)  
- The payload has suspicious characteristics or known shellcode patterns

This method has been heavily abused by malware and loaders for decades, making it a high-profile signature in behavioral engines.

---

## Technical Breakdown

- `VirtualAllocEx` creates a memory region in the target process's address space.
- The caller must have a process handle with `PROCESS_VM_OPERATION`, `PROCESS_VM_WRITE`, and `PROCESS_CREATE_THREAD` rights.
- `CreateRemoteThread` causes the remote process to execute code at the memory address provided — typically pointing to the injected shellcode or a loader stub.

---

## Evasion Techniques (For Research and Educational Use)

To reduce the visibility of this method during analysis or testing, researchers may consider:

- Allocating with `PAGE_READWRITE` and changing permissions to executable later via `VirtualProtectEx`
- Using `NtCreateThreadEx` instead of `CreateRemoteThread` to avoid API hooking
- Avoiding common target processes that are frequently monitored
- Applying alternative thread start techniques such as thread hijacking or early thread injection

These adjustments may alter behavioral patterns enough to avoid certain static or heuristic flags.

---

## Source Code

See [`../src/injection_virtualallocex.cpp`](../src/injection_virtualallocex.cpp) for a working implementation.

---

## References

- [MalwareTech – Remote Thread Injection](https://www.malwaretech.com/2015/11/remote-thread-injection.html)  
- [NT API Internals – Geoff Chappell](https://www.geoffchappell.com/)  
- [Microsoft Docs – CreateRemoteThread](https://learn.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-createremotethread)
