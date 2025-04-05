### 📄 `docs/injection_manual_dll_mapping.md`

# Manual DLL Mapping (Advanced Injection)

---

## 🔍 Overview

This technique demonstrates **manual DLL mapping**—a method of injecting a DLL into a target process without relying on the `LoadLibraryA` function. Instead of using the standard Windows injection API calls like `LoadLibraryA`, we manually map the DLL by allocating memory in the target process, writing the DLL path, and directly invoking the relevant functions to load the DLL. This approach offers a more stealthy method of injection and is harder for anti-virus (AV) software to detect.

> 🧠 **Goal:** To teach how DLLs can be injected using low-level techniques, providing a deeper understanding of process memory manipulation and function resolution.

---

## 🧰 Use Case

This technique is valuable for:

- **Advanced Malware Development:** Avoids detection by traditional API hooking methods.
- **Penetration Testing:** Helps bypass AV/EDR systems that may flag traditional injection methods.
- **Red Team Operations:** Demonstrates a method to execute code within the context of another process.

---

## 📚 Step-by-Step Breakdown

### 1. 🎯 Target Identification

```cpp
std::cout << "[>] Enter the PID of the target process: ";
std::cin >> pid;
```

The user is prompted to provide the **Process ID (PID)** of the target process. This process is where the DLL will be injected. The PID can be obtained using system tools like Task Manager or Process Explorer.

---

### 2. 🧪 Accessing the Process

```cpp
OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
```

The `OpenProcess()` function is used to obtain a handle to the target process. This handle must have appropriate access rights to:

- Allocate memory (`PROCESS_VM_OPERATION`)
- Write memory (`PROCESS_VM_WRITE`)
- Create and manipulate threads (`PROCESS_CREATE_THREAD`)

---

### 3. 📦 Allocating Remote Memory

```cpp
VirtualAllocEx(hProcess, nullptr, dllPath.length() + 1, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
```

Memory is allocated in the remote process to store the DLL path. The memory is set to be readable and writable so that the DLL path can be copied into it. The memory is reserved and committed to ensure that the process has space for the string.

---

### 4. ✍️ Writing the DLL Path

```cpp
WriteProcessMemory(hProcess, remoteMemory, dllPath.c_str(), dllPath.length() + 1, nullptr);
```

The **DLL path** is written into the allocated memory in the target process. It's essential that the string is **null-terminated**, as this is a requirement for `LoadLibraryA` to work correctly.

---

### 5. 🔍 Resolving LoadLibraryA

```cpp
pLoadLibraryA pLoadLib = (pLoadLibraryA)GetProcAddress(hKernel32, "LoadLibraryA");
```

We need the address of `LoadLibraryA` to invoke it in the remote process. `LoadLibraryA` is the function that actually loads a DLL from a specified path. We use `GetProcAddress()` to retrieve the address of this function in `kernel32.dll`.

---

### 6. 🧵 Creating the Remote Thread

```cpp
CreateRemoteThread(hProcess, nullptr, 0, (LPTHREAD_START_ROUTINE)pLoadLib, remoteMemory, 0, nullptr);
```

A remote thread is created in the target process. This thread will start execution at the address of `LoadLibraryA`, passing the injected DLL path as an argument. The target process will then execute `LoadLibraryA` with the injected DLL, causing it to load the DLL into the process's memory space.

---

### 7. 🔄 Synchronizing Execution

```cpp
WaitForSingleObject(hThread, INFINITE);
```

We wait for the remote thread to finish execution. Once the DLL has been loaded into the target process, the thread ends, and we can clean up resources.

---

## 📌 Summary

| **API**                | **Purpose**                                                   |
|------------------------|---------------------------------------------------------------|
| `OpenProcess`          | Opens the target process with necessary access rights         |
| `VirtualAllocEx`       | Allocates memory in the target process for the DLL path       |
| `WriteProcessMemory`   | Writes the DLL path into the allocated memory                 |
| `GetProcAddress`       | Resolves the address of `LoadLibraryA` in `kernel32.dll`      |
| `CreateRemoteThread`   | Creates a remote thread that invokes `LoadLibraryA`           |

---

## 🔐 Security Considerations

- **Privilege Requirements:** The process must be run with sufficient privileges (Admin rights), or `OpenProcess()` may fail.
- **AV/EDR Detection:** Many modern AV and EDR solutions can detect and block manual DLL mapping techniques, especially if the DLL path is suspicious or the process is known to execute malicious code.
- **Bypassing Detection:** This technique bypasses typical detection mechanisms by avoiding `LoadLibrary` and directly invoking the system function, making it harder to detect.

---

## 💡 Educational Value

- **Deep Understanding:** Teaches how to control the process memory and function execution in a much more granular manner than basic API injections.
- **Stealthier Technique:** Provides insight into more advanced and stealthy injection methods that are less likely to trigger traditional AV solutions.
- **Foundation for Advanced Techniques:** This technique lays the groundwork for more sophisticated methods such as **manual DLL mapping** using PE file headers, bypassing more advanced detection systems.

---

## 🔗 Source Code

See [`../src/injection_manual_dll_mapping.cpp`](../src/injection_manual_dll_mapping.cpp)
