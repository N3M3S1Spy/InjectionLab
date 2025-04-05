# 🧬 Injection Matrix

Welcome to the **Injection Matrix** – the core learning map of the InjectionLab project.

This matrix is not just a checklist of known techniques, but a **structured and educational journey** through the landscape of Windows process injection. Each entry is a standalone case study – coded, documented, and explained for researchers, students, and defenders.

---

## 🎯 Purpose of the Injection Matrix

The matrix serves as:

- ✅ A **catalog of real-world injection techniques**, used by malware and red teams.
- 🧠 A **learning tool** to understand low-level Windows internals.
- 🔍 A **blueprint for detecting injection** via memory, handle, and thread artifacts.
- 🛠️ A **sandbox** to test AV/EDR behavior in controlled, ethical environments.

This project is for **educational use only**. Every technique here is potentially detectable and not meant for evasion or offensive security without authorization.

---

## 🧪 Techniques Covered

| Technique                           | Uses Remote Thread | Allocates Memory | Shellcode Injection | Complexity | Source Link |
|------------------------------------|--------------------|------------------|---------------------|------------|-------------|
| VirtualAllocEx + WriteProcessMemory + CreateRemoteThread | ✅ | ✅ | ✅ | 🟡 Medium | [View](../src/injection_virtualallocex.cpp) |
| QueueUserAPC Injection              | ✅ (via APC)        | ✅               | ✅                  | 🔴 High    | [View](../src/injection_queueuserapc.cpp) |
| DLL Injection via LoadLibrary      | ✅ | ✅               | ❌ (DLL instead)   | 🟢 Easy    | [View](../src/injection_dll_loadlibrary.cpp) |
| Manual Mapping                     | ❌ (no API thread)  | ✅               | ✅                  | 🔴 High    | [View](../src/injection_manual_mapping.cpp) |
| NtCreateThreadEx                   | ✅ | ✅               | ✅                  | 🟡 Medium | [View](../src/injection_ntcreatethreadex.cpp) |

🟢 = Beginner-friendly  
🟡 = Intermediate  
🔴 = Advanced

---

## 🧠 What You’ll Learn

By exploring this matrix, you’ll learn to:

- Understand Windows process memory and handle mechanics
- Use WinAPI safely and intentionally
- Identify different code injection workflows
- Evaluate detection surfaces and artifacts
- Build your own defensive rules and heuristics

---

## 📂 Project Layout

Each technique lives in:

- `src/<technique>.cpp` – full, well-documented implementation
- `docs/<technique>.md` – detailed explanation, step-by-step walkthrough
- `injection_matrix.md` – this file you're reading now

---

## ⚠️ Legal Notice

This project is for **educational and research purposes only**. Do not use it on machines or networks you do not own or have permission to test. You are responsible for how you use this information.

---

Happy learning, and stay ethical. 🧪🔐
