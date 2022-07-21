
### Process Descriptor and the Task Structure

Another name for a process is a **task**. These tend to be used interchangablly, and internally, the Linux kernel refers to processes as tasks.

The kernel stores the set of all processes in a circular doubly linked list called the **task list**. Each element in the task list, is a **process descriptor** of the type `struct task_struct` which is defined in `<linux/sched.h>` header file. This struct stores all the information that the linux kernel needs to know about the process to manage it.

In the first section, we defined what a process was - It is a set of resources that an executing program uses. The `struct task_struct` stores all the neccessary information about these resources including open files, processes's address space, pending signals, processor state, and much more.

**Hint**: Take a look at the `<linux/sched.h>` file to look at what other information is stored in the `struct task_struct`.

![The task list and process descriptor](assets/task_list.png)


### Allocating the process descriptor

Each userspace thread has two stacks:
- User Stack - this is used to store stack frames that correspond to instructions that are executed in userspace. This is stored in the user address space
- Kernel Stack - When the thread makes a syscall to execute a privileged action in kernelspace, this stack is used. This is stored in the kernel address space

The `task_struct` structure is allocated by the `slab allocator`, which is a memory management mechanism for efficient allocation of objects.

In > Linux 2.7, a `struct thread_info` structure is allocated at the end of the kernel stack which holds information about the thread including a pointer to the corresponding `task_struct` that can contain multiple threads (and therefore multiple `struct thread_info`)

In < Linux 2.7, this wasn't the case, and the `struct thread_info` structure did not exist

![The thread_info structure](assets/thread_info.png)


![The process kernel stack](assets/process_kernel_stack.png)


### Storing the process descriptor

The system identifies processes by the unique `pid` (process ID) which is a nnumerical value represented by the opaque type `pid_t`. This is typically a signed short int (32bit) for the sake of backwards compatability with earlier Unix/Linux versions.

Since each process needs to have a unique PID, the max number of PIDs available effectively limits the number of processes that can be allocated (32,768). In some cases, this may be an insufficient number of processes for large servers/systems. We can modify <linux/threads.h> before compiling linux in order to increase this size.

PIDs are allocated in contiguously increasing order. Therefore the notion that processes with larger PIDs are newer than processes with smaller PIDs. However, once the max PID number is used, the system then wraps around to 0 and finds the next smallest PID available.

**NOTE**: The reason why `pid_t` is signed is so that negative values can be returned from syscalls such as `fork()` to denote errors. However, the process ID's themselves cannot be negative, which is why the max number of processes is 2^(intsize - 1)

Inside the kernel, tasks (`struct task_struct`) are typically referenced directly by pointers (as opposed to using the task list). This is done to avoid wasted computations of searching the the task list for a process descriptor.

In order to get the process descriptor about the currently executing task, the kernel provides the `current` macro. This macro is architecture specific:
- some architectures may store a pointer to the current process descriptor (`task_struct`) in a register, 
- other architectures don't do this so need to calculate the memory address of the process descriptor every time the macro `current` is used

In x86 architecture, the `current` macro needs to calculate the memory address of the process descriptor:
1. It starts by calculated the address of the `struct thread_info` that is stored in the currently used stack. The assembly that the `current_thread_info()` method is shown here:
- `movl $-8192, $eax` - it stores the negative of the size of the stack. In this case the size of the stack is 8192 bytes
- `andl $esp, $eax` - It then masks out the least significant bits by anding the stacking pointer (stored in `$esp`), with the size of the kernel stack (stored in `$eax`)
- This returns a pointer to the `struct thread_info` structure. The reason why we use a negative value here is because in x86, the stack grows downwards
2. The `struct thread_info` stores a pointer to its corresponding `struct task_struct`. The `current` macro then dereferences the `task` member of the `struct thread_info`:
- `current_thread_info()->task;`

In other architectrures such as PowerPC (IBM's RISC-based microprocessor), the current `task_struct` address is stored in the register `r2` meaning that the macro `current` just reads from this. The reason this can be done in PowerPC is because it has plenty of registers, and its kernel develoeprs deemed the address to the current `task_struct` worthy of a process register
