# VirtualAllocEx + CreateRemoteThread

This is one of the oldest and most well-known code injection techniques in Windows.

---

## 🧠 What does it do?

It works by:

1. **Opening a handle** to a target process
2. **Allocating memory** inside that remote process (`VirtualAllocEx`)
3. **Writing shellcode or data** into that memory (`WriteProcessMemory`)
4. **Creating a remote thread** that starts executing the shellcode (`CreateRemoteThread`)

---

## 📦 Why it works

Windows allows processes to:
- Allocate memory in other processes (if permissions allow)
- Write into that memory
- Trigger a thread to run inside another process

This is actually part of **legitimate Windows behavior** (e.g. debuggers use similar APIs).

---

## 🧨 Why it's detected

EDRs look for the **pattern**:
```
VirtualAllocEx → WriteProcessMemory → CreateRemoteThread
```
Especially when:
- The memory is marked as `PAGE_EXECUTE_READWRITE`  
- The remote process is a common target like `explorer.exe`, `notepad.exe`

> 🔎 Most AVs and EDRs flag this because it is widely abused by malware loaders and shellcode injectors.

---

## 🔬 Internals

- `VirtualAllocEx` is a kernel call that creates a memory region in the remote process.
- That region is accessible only because you have a valid `HANDLE` with `PROCESS_ALL_ACCESS`.
- The thread created by `CreateRemoteThread` runs in the context of the target process, but executes code you placed there.

---

## 🛡️ Evasion Tips (Educational Only)

- Use `PAGE_READWRITE` + `VirtualProtectEx` after writing
- Use `NtCreateThreadEx` instead of `CreateRemoteThread`
- Avoid common target processes
- Use "early bird" injection or thread hijacking instead

---

## 📚 References

- [MalwareTech – Remote Thread Injection](https://www.malwaretech.com/2015/11/remote-thread-injection.html)
- [NT API Internals – Geoff Chappell](https://www.geoffchappell.com/)
- [Microsoft Docs – CreateRemoteThread](https://learn.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-createremotethread)
