# Injection Matrix

The Injection Matrix is the central learning map of the InjectionLab project.

Rather than just listing techniques, it offers a structured and practical deep dive into Windows process injection. Each technique is a standalone example with working code and detailed documentation, aimed at security researchers, students, and defenders who want to understand these methods from the ground up.

---

## Purpose of the Injection Matrix

This matrix is designed to serve as:

- A catalog of real-world injection techniques used by malware and red teams
- A hands-on guide to learning Windows internals and memory operations
- A reference for detecting injection through artifacts in memory, handles, and threads
- A testing ground for behavior in AV/EDR environments (under controlled, ethical conditions)

This is an educational project only. The included techniques are detectable and should not be used offensively without proper authorization.

---

## Techniques Covered

| Technique                           | Remote Thread Used | Allocates Memory | Shellcode Injected | Complexity | Source |
|------------------------------------|--------------------|------------------|--------------------|------------|--------|
| VirtualAllocEx + WriteProcessMemory + CreateRemoteThread | Yes | Yes | Yes | Medium | [Source](../src/injection_virtualallocex.cpp) |
| QueueUserAPC Injection              | Yes (via APC)      | Yes              | Yes                | High       | [Source](../src/injection_queueuserapc.cpp) |
| DLL Injection via LoadLibrary      | Yes                | Yes              | No (DLL used)      | Easy       | [Source](../src/injection_dll_loadlibrary.cpp) |
| Manual Mapping                     | No                 | Yes              | Yes                | High       | [Source](../src/injection_manual_mapping.cpp) |
| NtCreateThreadEx                   | Yes                | Yes              | Yes                | Medium     | [Source](../src/injection_ntcreatethreadex.cpp) |

**Complexity Scale:**  
Easy = Beginner-friendly  
Medium = Intermediate  
High = Advanced and low-level

---

## What You’ll Learn

Working through the matrix, you’ll learn to:

- Understand how Windows manages memory, threads, and handles
- Safely use low-level Windows APIs for memory and process interaction
- Implement different injection methods with C++
- Recognize the behavioral and forensic signs of each technique
- Begin designing your own detection logic or tooling based on observed artifacts

---

## Project Structure

Each technique includes:

- `src/<technique>.cpp` — full source code with inline comments
- `docs/<technique>.md` — complete explanation and walkthrough
- `injection_matrix.md` — this overview file

---

## Legal Notice

All techniques are for research and educational use only. Never apply them on systems you do not own or do not have explicit permission to test. Misuse of this knowledge is your responsibility.
