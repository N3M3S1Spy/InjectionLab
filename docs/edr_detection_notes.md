# EDR Detection Notes

## VirtualAlloc + CreateRemoteThread

This combination is highly detectable and commonly flagged by Endpoint Detection and Response (EDR) systems. Key indicators include:

- Use of `PAGE_EXECUTE_READWRITE` memory pages  
- Writing to remote process memory using `WriteProcessMemory`  
- Spawning a remote thread via `CreateRemoteThread` targeting another process

These patterns are widely recognized and often appear in known injection signatures.

---

## APC Injection

Asynchronous Procedure Call (APC) injection can sometimes evade detection, particularly when:

- The target thread is not in an alertable state  
- The APC is queued but not immediately executed

However, some EDRs detect the typical pattern of memory allocation followed by `QueueUserAPC`.

---

## Process Hollowing

This technique is heavily monitored and almost always triggers alerts. EDRs typically flag:

- Unmapping and rewriting of the target process’s memory space  
- Overwriting of PE headers or image sections  
- Resuming the main thread after payload replacement

Its behavioral footprint is significant, especially when restoring execution in a previously suspended process.

---

## General Detection Techniques

EDRs use a combination of static and behavioral analysis to detect injection activity. Common techniques include:

- Scanning for memory regions marked executable that don't map to valid modules  
- Heuristic analysis of memory permissions and thread origins  
- API hooking to monitor functions like `WriteProcessMemory`, `CreateRemoteThread`, `VirtualAllocEx`, and `NtQueueApcThread`  
- Monitoring syscalls and telemetry through Event Tracing for Windows (ETW)

---

**Tip:** Use `PAGE_READWRITE` during allocation, then change to executable later using `VirtualProtectEx` to reduce detection surface.
