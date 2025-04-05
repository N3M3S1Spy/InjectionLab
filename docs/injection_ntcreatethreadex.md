# 🧪 Injection Using NtCreateThreadEx

This example demonstrates **process injection using the `NtCreateThreadEx` API**, a low-level function in the Windows kernel that allows for thread creation in a target process. Unlike the traditional `CreateRemoteThread` API, `NtCreateThreadEx` is not part of the documented Windows API, and its usage can sometimes evade detection mechanisms designed to detect more conventional thread creation methods.

This technique is advanced and often bypasses certain user-mode protections by directly interacting with the Windows kernel. Therefore, it's a useful tool for researchers, red teamers, and those studying how malicious code may hide within a process.

---

## **Injection Overview**

### **Goal of the Technique**

The objective of this technique is to inject a thread into a target process and execute shellcode. The shellcode can perform actions like opening a message box, writing to a file, or triggering other behaviors within the target process. This injection is achieved by utilizing the `NtCreateThreadEx` function, which is a low-level API for thread creation in a target process.

---

## **Steps in the Injection Process**

### **1. Obtain a Handle to the Target Process**

First, the program needs to obtain a handle to the target process. This is done by using the `OpenProcess` function, which allows us to specify the level of access we require. In this case, we request full access to the target process by passing the `PROCESS_ALL_ACCESS` flag.

```cpp
HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
```

- **PID (Process ID)**: The user is prompted to input the Process ID (PID) of the target process.
- **Access Rights**: The `PROCESS_ALL_ACCESS` flag grants the ability to perform operations such as reading from and writing to the target's memory, as well as creating new threads.

If this function fails, an error message is printed, and the program exits.

### **2. Load the `NtCreateThreadEx` Function**

Since `NtCreateThreadEx` is not part of the officially documented Windows API, we cannot directly use it via the standard Windows headers. Instead, we dynamically load the `ntdll.dll` library and retrieve the address of the `NtCreateThreadEx` function using `GetProcAddress`.

```cpp
HMODULE hNtdll = GetModuleHandle(L"ntdll.dll");
pNtCreateThreadEx NtCreateThreadEx = (pNtCreateThreadEx)GetProcAddress(hNtdll, "NtCreateThreadEx");
```

- **`ntdll.dll`**: This dynamic link library contains low-level Windows functions, including `NtCreateThreadEx`.
- **`GetProcAddress`**: This function retrieves the address of a function by name. Here, we use it to access the `NtCreateThreadEx` function.

If the function cannot be found, the program prints an error and exits.

### **3. Allocate Memory in the Target Process**

The next step is to allocate memory in the target process where the shellcode will be injected. This is done using `VirtualAllocEx`, which allows us to allocate a region of memory in the remote process.

```cpp
LPVOID alloc = VirtualAllocEx(
    hProcess, nullptr, sizeof(shellcode), MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE
);
```

- **Memory Protection**: The allocated memory is marked with the `PAGE_EXECUTE_READWRITE` protection flag, meaning it is both readable and executable. This is necessary for shellcode execution.
- **Size of Allocation**: The size of the allocation is determined by the size of the shellcode.

### **4. Write the Shellcode into the Allocated Memory**

Once memory is allocated, we need to write the shellcode into the allocated region. This is achieved using the `WriteProcessMemory` function, which writes data (in this case, the shellcode) into the target process's memory.

```cpp
WriteProcessMemory(hProcess, alloc, shellcode, sizeof(shellcode), nullptr);
```

In this example, the shellcode consists of `NOP` instructions (no-operations), which do nothing and are used as placeholders. You can replace this with actual functional shellcode, such as code that opens a message box or performs some other action.

### **5. Create the Remote Thread Using `NtCreateThreadEx`**

After the shellcode is injected into the target process's memory, we create a remote thread using `NtCreateThreadEx`. This function is called with the following parameters:

```cpp
NTSTATUS status = NtCreateThreadEx(
    &hThread, THREAD_ALL_ACCESS, nullptr, hProcess, alloc, nullptr, FALSE, 0, 0, 0, nullptr
);
```

- **`&hThread`**: This is the handle to the newly created remote thread.
- **`THREAD_ALL_ACCESS`**: This specifies that the thread will be created with full access rights.
- **`hProcess`**: The handle to the target process where the thread will be created.
- **`alloc`**: This is the address of the shellcode in the remote process’s memory, which the thread will begin executing.
- **`CreateFlags`**: A flag that determines whether the thread should start immediately. In this case, we set it to `FALSE` to allow for further configuration.
- **`ZeroBits`, `StackSize`, `MaximumStackSize`, `AttributeList`**: These parameters are typically set to their default values unless a custom stack or thread attributes are needed.

The function returns an `NTSTATUS` value, which indicates whether the operation succeeded or failed. If the status is not `0`, an error message is printed.

### **6. Clean Up**

After the thread is successfully created, we close the process handle to clean up resources.

```cpp
CloseHandle(hProcess);
```

This ensures that there are no lingering handles to the target process.

---

## **Key Concepts and Functions**

### **`NtCreateThreadEx`**
`NtCreateThreadEx` is a low-level function that creates a thread in a target process. It is not part of the documented Windows API, which makes it less likely to be flagged by security software that specifically looks for `CreateRemoteThread`.

### **`VirtualAllocEx`**
Allocates memory in the target process. This function is essential for reserving space in the target process’s memory for our shellcode.

### **`WriteProcessMemory`**
Writes data (shellcode) to the memory of the target process. This is the actual injection step where the payload is transferred into the target process’s address space.

### **`OpenProcess`**
Used to obtain a handle to the target process, allowing for memory manipulation and thread creation.

### **`GetProcAddress`**
Used to dynamically load functions from libraries at runtime. Here, it is used to load `NtCreateThreadEx` from `ntdll.dll`.

---

## **Considerations and Limitations**

### **Detection Evasion**
- While `NtCreateThreadEx` can be used to bypass certain detection mechanisms associated with `CreateRemoteThread`, it is not a foolproof method. Modern security tools (such as AV/EDR systems) may still detect this technique based on abnormal memory allocations, thread activity, and process behavior.
  
### **Error Handling**
- The error handling in this example is minimal, and it is important to expand it for production code. Proper error handling and status checking should be implemented to avoid misbehaving programs.

### **Shellcode**
- The shellcode in this example is a simple placeholder (`NOP` instructions). In practice, you would replace it with functional payloads, such as executing a command or injecting a more complex payload like a reverse shell.

### **Legal Considerations**
- This technique is demonstrated **for educational and research purposes only**. It should only be used in controlled, ethical environments, such as in your own test machines or systems you have explicit permission to test.

---

## **Summary**

Using `NtCreateThreadEx`, we can inject a thread into a target process and execute shellcode without relying on the more commonly detected `CreateRemoteThread` API. This technique, while advanced, is powerful for understanding Windows internals and exploring process injection methods. However, as with all injection techniques, it is important to remain ethical and responsible when using this knowledge.

---

For more information and related techniques, check out the **Injection Matrix**.
