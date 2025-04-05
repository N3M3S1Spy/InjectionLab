# Injection Matrix

| Technique                  | Key API Functions                    | Target Thread State | Detection Risk | Complexity |
|---------------------------|--------------------------------------|---------------------|----------------|------------|
| VirtualAlloc + CRT        | VirtualAllocEx, WriteProcessMemory, CreateRemoteThread | Not required        | 🔴 High       | 🟢 Low     |
| APC Injection             | QueueUserAPC, OpenThread             | Alertable           | 🟠 Medium       | 🟡 Medium  |
| Thread Hijacking          | SuspendThread, SetThreadContext      | Not required        | 🔴 High         | 🔴 High    |
| Process Hollowing         | ZwUnmapViewOfSection, WriteProcessMemory | Not required    | 🔴 Very High    | 🔴 High    |
| MapView Injection (NT)    | NtMapViewOfSection, NtCreateSection | Not required        | 🟠 Medium       | 🔴 High    |

> Legend:  
> 🟢 Low | 🟡 Medium | 🔴 High
