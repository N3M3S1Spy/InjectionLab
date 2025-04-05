# EDR Detection Notes

## 🎯 VirtualAlloc + CreateRemoteThread
- Very noisy.
- EDRs commonly flag:
  - `PAGE_EXECUTE_READWRITE` + `WriteProcessMemory`
  - `CreateRemoteThread` targeting another process

## ⚠️ APC Injection
- Often missed if target thread is idle or alertable state is not active
- Some EDRs flag the `QueueUserAPC` + memory allocation pattern

## 🚨 Process Hollowing
- Almost always flagged
- Behavior like unmapping image sections, writing PE headers, and resuming main thread are suspicious

## 🧠 General Detection Techniques
- Memory scanning for executable pages
- Heuristics: injected threads, suspicious memory permissions
- API hooking (e.g. on `WriteProcessMemory`, `CreateRemoteThread`)
- Syscalls and ETW (Event Tracing for Windows)

---

💡 Tip: Use `PAGE_READWRITE` + RWX switch later via `VirtualProtectEx` to reduce detection.
