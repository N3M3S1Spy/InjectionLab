# DLL Injection via LoadLibrary + CreateRemoteThread

---

## 🔍 Overview

This technique demonstrates **DLL injection** using three key Windows API functions:

- `VirtualAllocEx()` – allocates memory in a remote process.
- `WriteProcessMemory()` – writes a string (path to the DLL) into that memory.
- `CreateRemoteThread()` – creates a thread in the target process that runs `LoadLibraryA` with the injected path as an argument.

> 🧠 **Goal:** Dynamically load an external DLL into a remote process's address space, causing it to execute code defined in the DLL's `DllMain`.

---

## 🧰 Use Case

This technique is a **real-world classic** in red teaming and malware engineering, often used to:

- Hook or hijack the behavior of a process.
- Inject surveillance, logging, or API manipulation code.
- Load arbitrary code with fewer AV flags compared to raw shellcode.

---

## 📚 Step-by-Step Breakdown

### 1. 🎯 Target Identification

```cpp
DWORD pid;
std::cin >> pid;
```

The user is prompted to provide the Process ID (PID) of the process to be targeted. You can obtain a PID via Task Manager or tools like `Process Explorer`.

---

### 2. 🧪 Accessing the Process

```cpp
OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
```

The process must be opened with sufficient privileges to:

- Allocate memory (`PROCESS_VM_OPERATION`)
- Write memory (`PROCESS_VM_WRITE`)
- Start threads (`PROCESS_CREATE_THREAD`)

⚠️ If your program lacks the necessary privileges (e.g., not run as Administrator), `OpenProcess()` will fail.

---

### 3. 📦 Allocating Remote Memory

```cpp
VirtualAllocEx(..., PAGE_READWRITE);
```

We allocate enough memory in the remote process to store the **full path of the DLL**. This memory must be:

- Committed and reserved (`MEM_COMMIT | MEM_RESERVE`)
- Writable (`PAGE_READWRITE`) for copying the string

---

### 4. ✍️ Writing the DLL Path

```cpp
WriteProcessMemory(hProcess, remoteMemory, dllPath.c_str(), ...);
```

The DLL path is written byte-by-byte into the allocated memory block of the target process.

🧠 Important: The path **must be null-terminated**, or `LoadLibraryA` will not work.

---

### 5. 🔍 Resolving LoadLibraryA

```cpp
GetProcAddress(GetModuleHandleA("kernel32.dll"), "LoadLibraryA");
```

We get the address of `LoadLibraryA()` from `kernel32.dll`, which is **almost always loaded** in every Windows process. This function becomes the **entry point** of the new remote thread.

---

### 6. 🧵 Creating the Remote Thread

```cpp
CreateRemoteThread(..., loadLibraryAddr, remoteMemory, ...);
```

- The new thread will start executing at the address of `LoadLibraryA`
- It will receive the injected DLL path as argument
- Result: `LoadLibraryA(dllPath)` is executed inside the remote process

✅ If successful, this will load your DLL, triggering its `DllMain()` function.

---

## 📌 Summary

| API                   | Purpose                               |
|-----------------------|----------------------------------------|
| `OpenProcess`         | Get a handle to the target process     |
| `VirtualAllocEx`      | Allocate memory in the remote process  |
| `WriteProcessMemory`  | Write the DLL path into the memory     |
| `GetProcAddress`      | Find the address of `LoadLibraryA`     |
| `CreateRemoteThread`  | Trigger `LoadLibraryA(dllPath)` remotely |

---

## 🔐 Security Considerations

- **User Account Control (UAC):** Required privileges can block injection.
- **AV/EDR detection:** This technique is widely known and often monitored.
- **LoadLibrary spoofing:** Some defenses check for suspicious DLL loads or block from untrusted paths.

---

## 💡 Educational Value

- Teaches fundamental concepts of memory allocation, pointer management, and inter-process communication.
- Helps understand how benign Windows APIs can be (ab)used for malicious purposes.
- Lays the groundwork for more advanced techniques like **manual DLL mapping** (coming later in this lab).

---

## 🔗 Source Code

See [`../src/injection_dll_loadlibrary.cpp`](../src/injection_dll_loadlibrary.cpp)
