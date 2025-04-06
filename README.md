# InjectionLab

**InjectionLab** is a hands-on learning project focused on understanding and implementing Windows process injection techniques. It's built for security researchers, red teamers, and defenders who want to explore how code injection works under the hood — and how it can be detected and understood in real-world scenarios.

---

## What is Process Injection?

Process injection is the act of running custom code inside the memory space of another process. It’s a technique often used by malware and offensive tools to evade detection, gain persistence, or manipulate the behavior of trusted processes.

Understanding how these methods work is essential not only for those working in offensive security, but also for those building defenses and detection capabilities.

---

## Why InjectionLab?

This project was created to:

- **Educate**: Learn how Windows manages memory, processes, and threads at a low level.
- **Demonstrate**: Show real examples of injection techniques used in red teaming and malware.
- **Document**: Provide fully documented C++ implementations with clear, step-by-step explanations.
- **Raise Awareness**: Understand the artifacts and detection points that each technique leaves behind.

---

## What You’ll Learn

Working through InjectionLab, you'll get practical experience with:

- Low-level Windows API usage (`VirtualAllocEx`, `WriteProcessMemory`, `NtCreateThreadEx`, etc.)
- Shellcode injection and remote thread execution
- Different code injection workflows and their detection surfaces
- Memory and thread forensics
- Core concepts of Windows internals

---

## The Injection Matrix

The Injection Matrix is the main roadmap of the project. It breaks down each injection method by complexity and explains:

- What the technique does
- How it works internally
- Where it can be detected
- Complete C++ code and deep technical documentation

Techniques are grouped by difficulty, from beginner-friendly to advanced memory manipulation.

---

## Included Techniques

Some of the techniques currently implemented and documented include:

- VirtualAllocEx + WriteProcessMemory + CreateRemoteThread
- QueueUserAPC Injection
- DLL Injection via LoadLibrary
- Manual Mapping
- NtCreateThreadEx Injection

Each method is backed by real source code and a deep-dive explanation.

---

## Legal and Ethical Notice

This project is intended for **educational and research purposes only**. Do not use these techniques on systems you do not own or explicitly have permission to test. You are responsible for how you use the knowledge shared here.

---

## Getting Started

Check out the [injection_matrix.md](docs/injection_matrix.md) file for an overview of available techniques. Each technique includes a corresponding source file under `src/` and documentation under `docs/`.
