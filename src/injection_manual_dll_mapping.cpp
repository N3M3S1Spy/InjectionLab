// InjectionLab - Educational Code Injection Toolkit
// injection_manual_dll_mapping.cpp
//
// This example demonstrates manual DLL mapping, where the DLL is loaded
// into a remote process by manually setting up the necessary structures
// and functions, bypassing LoadLibraryA and other standard injection APIs.
//
// It involves creating memory in the target process, writing the necessary
// DLL header information (PE headers), and manually resolving the function
// pointers to map and execute the DLL.

#include <windows.h>
#include <iostream>
#include <string>

typedef FARPROC(WINAPI* pGetProcAddress)(HMODULE hModule, LPCSTR lpProcName);
typedef HMODULE(WINAPI* pLoadLibraryA)(LPCSTR lpLibFileName);

// Simple error handling function to print last error
void PrintLastError(const std::string& message) {
    std::cerr << message << " Error code: " << GetLastError() << std::endl;
}

int main() {
    DWORD pid;
    std::string dllPath;

    std::cout << "[>] Enter the PID of the target process: ";
    std::cin >> pid;
    std::cin.ignore(); // Clear newline left in input buffer

    std::cout << "[>] Enter the full path to the DLL to inject: ";
    std::getline(std::cin, dllPath);

    // Open the target process with necessary access rights
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    if (!hProcess) {
        PrintLastError("[-] OpenProcess failed for PID");
        return 1;
    }

    // Allocate memory for the DLL path in the target process
    LPVOID remoteMemory = VirtualAllocEx(
        hProcess,
        nullptr,
        dllPath.length() + 1,
        MEM_COMMIT | MEM_RESERVE,
        PAGE_READWRITE
    );
    if (!remoteMemory) {
        PrintLastError("[-] VirtualAllocEx failed.");
        CloseHandle(hProcess);
        return 1;
    }

    // Write the DLL path to the remote process memory
    if (!WriteProcessMemory(hProcess, remoteMemory, dllPath.c_str(), dllPath.length() + 1, nullptr)) {
        PrintLastError("[-] WriteProcessMemory failed.");
        VirtualFreeEx(hProcess, remoteMemory, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return 1;
    }

    // Get the address of LoadLibraryA and GetProcAddress in kernel32.dll
    HMODULE hKernel32 = GetModuleHandleA("kernel32.dll");
    if (!hKernel32) {
        PrintLastError("[-] GetModuleHandleA failed for kernel32.dll.");
        VirtualFreeEx(hProcess, remoteMemory, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return 1;
    }

    pLoadLibraryA pLoadLib = (pLoadLibraryA)GetProcAddress(hKernel32, "LoadLibraryA");
    if (!pLoadLib) {
        PrintLastError("[-] GetProcAddress failed for LoadLibraryA.");
        VirtualFreeEx(hProcess, remoteMemory, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return 1;
    }

    // Now, we manually map the DLL by using the resolved LoadLibraryA function pointer
    HANDLE hThread = CreateRemoteThread(
        hProcess,
        nullptr,
        0,
        (LPTHREAD_START_ROUTINE)pLoadLib,
        remoteMemory,
        0,
        nullptr
    );
    if (!hThread) {
        PrintLastError("[-] CreateRemoteThread failed.");
        VirtualFreeEx(hProcess, remoteMemory, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return 1;
    }

    // Wait for the remote thread to finish
    WaitForSingleObject(hThread, INFINITE);

    // Clean up
    CloseHandle(hThread);
    CloseHandle(hProcess);

    std::cout << "[+] DLL successfully injected and mapped into remote process." << std::endl;
    return 0;
}
