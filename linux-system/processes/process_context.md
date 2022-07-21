### Process Context

One of the most important parts of a process is the executing program code.The corresponding executable is read from secondary storage and loaded into the process's address space. 

Processes typically execute in `user-space`. However when a process requires to execute an action that can only be performed in `kernel-space`, the process then the kernel executes in `kernel-space`. The kernel is said to be in **process context** when it is executing intructions on behalf of the user-space process.

Once the process exits the kernel, the process can either resume in `user-space`, or if there is a higher priority process that is runable, then that process is executed. **NOTE**: The `current` macro is only usable in `kernel-space`.
S
The kernel has several well defined interfaces including system calls and exception handlers. A process can only be executed in `kernel-space` if it uses a kernel interface.
