# InjectionLab – Educational Code Injection Toolkit

**InjectionLab** is a collection of C++ code injection techniques designed for educational and research purposes. It demonstrates various ways to perform code injection into Windows processes using native WinAPI.

> **Disclaimer:**  
> This project is for learning purposes only.  
> No real malicious payloads are included.  
> Use responsibly and only in safe environments.

---

## Features

- VirtualAllocEx + CreateRemoteThread (classic method)
- APC Injection
- Thread Hijacking *(coming soon)*
- Process Hollowing *(coming soon)*
- Shellcode examples (e.g., MessageBoxA)
- Detection notes and EDR behavior overview

---

## File Structure

```
InjectionLab/
├── src/
│   ├── injection_virtualallocex.cpp
│   ├── injection_apc.cpp
│   └── shellcode_messagebox.h
├── docs/
│   ├── injection_matrix.md
│   └── edr_detection_notes.md
└── README.md
```

---

## Building

1. Open a Visual Studio Developer Command Prompt  
2. Compile using `cl /EHsc src\\injection_virtualallocex.cpp`  
3. Run with admin privileges and provide a valid PID of a target process (e.g., notepad.exe)

---

## Learning Goals

- Understand how WinAPI functions can be used to inject code
- Learn the limitations and risks of each technique
- Study detection vectors used by modern security tools

---

## Legal

This project is provided **as is**, without warranty.  
Any misuse is the sole responsibility of the user.
