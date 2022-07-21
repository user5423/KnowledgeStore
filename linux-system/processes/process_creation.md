### Process Creation

Most Operating systems tend to implement a *spawn* mechanism that
1. Creates a new process in a new memory address space
2. Reads in an executable and begins execution

However, Unix differs in that it performs these two operations as **separate distinct** system calls:
1. `fork()` - it creates a copy of the current task where the only difference is the PID, PPID, memory address space, pending signals and I/O, and certain resources and statistics
2. `exec()` - Reads the executable from storage and loads it into the address space and beings executing it
**NOTE**: There are numerous variants of each of these that are described later

The combination of performing `fork` followed by `exec` is similar to a single function that most other OSs provide. 

#### Copy On Write

Historically, when `fork()` was executed, all resources owned by the parent were duplicated and the copies were given to the child. However, this is pretty **inefficient** for two reasons
1. There will likely be duplicated resources that could otherwise be shared
2. In the scenario where we want to perform `exec` to load a **different** executable, then the Kernel wouldn't have wasted CPU on duplicating resources for a child task that will **not** be used by the child, and will have to be overwritten again.

Nowadays, `fork()`'s implementation uses **Copy-On-Write** in order to reduce wasted memory and computation. When a `fork()` is executed
1. Each of the parent resources are shared with the child
2. The child and parent share resources as long as no operations have modified the shared resources/values since the fork occured
3. Once a write operation is to occur on a shared resource, the resource is duplicated so that the parent and child have their **own** copies

This technique delays duplication of pages from the parent's memory address until it is written to. In the case where `exec()` is called immediately after `fork()` none of the resources are duplicated, to reduce wasted computations. This is a considerable optimization that improves quick process execution.

The only incurred overhead by `fork()` is:
1. The duplication of the parent's page tables (so that the child has a way to map virtual address to physical addresses for the resoures it shares with the parent)
2. The creation of the unique process descriptor for the child.

#### Forking

There are a variety of fork methods that can be used that modify the beha`ior or resources shared between the parent and children. The two main system calls for forking are `fork()` and `vfork()`, which both invoke the `clone()` system call.

The `clone()` system call can take a set of parameters that specify which resources are shared betwen the parent and child, (as well as the behavior of the cloning of the process). The `fork()` and `vfork()` methods don't have any arguments, but call the `clone()` with a set of constant arguments that define their documented behavior (which will be defined shortly)

The `clone()` syscall calls yet another syscall called `do_fork()` which performs the majority of the forking work, and can be found  in `kernel/linux.c`. This function calls`copy_process()`, and then continues execution of the program.

The interesting work is done by the `copy_process()` function:
1. **Duplicates the parent's task related structures by calling `dup_task_struct()** - A new kernel stack, `thread_info` struct, and task_struct are created for the child process which have identical values to parent. The task descriptors for child and parent at this point are identical
2. **Checks if child exceeds resource limits** - There are resource limits in place such as process number limits on a per-user basis, that cannot be exceeded
3. **Clears/Reinitializes members of the child `task_struct`** - A lot of the members of `task_struct` should not be inherited by the child. This includes usage and statistics information.
4. **Sets the child process's status to `TASK_UNINTERUPTABLE`** - This ensures that no signals interfer with the process causing it to prematurely run
5. **Updates the flags of the child `task_struct` by calling `copy_flags()`** - Flags such as `PF_SUPERPRIV` (which states if a task has used sudo privileges), and `PF_FORKNOEXEC` (which denotes whether a task has called exec(), are cleared and set respectively
6. **Allocates a new PID to child by calling `alloc_pid()`**
7. **Duplicates/Shares resources depending on `clone()` arguments** - Depending on what arguments where passed to clone, resources such as open files, filesystem information, signal handlers, process memory addresses, and namespaces are either shared or duplicated. This is the stage that will semantically different processes and threads.
8. **Cleanup is performed and a pointer to the child `task_struct` is returned**

#### vfork()

`vfork()` is another way to create a child process. However, it behaves slightly different from `fork()` in a few ways:
- When `vfork()` is called, **nothing** is duplicated for the child process. The child is the sole process running in the parent's memory address space. The child has **read-only** access and cannot modify anything in the address space
- When `vfork()` is called the parent is blocked until the child exits or the child calls `exec()`

Historically, `fork()` was not implemented using Copy-On-Write principles, so there was more cpu and memory wastage. `vfork()` was introduced during this time when `fork()` didn't use CoW. Therefore it was a welcome addition, as it reduced the wastage caused from:
1. Duplicating parent resources, eventhough a later exec call would allocate and overwrite the `task_struct` entries that pointed to these resources
2. Duplicating *all* parent resources, eventhough the child and parent will likely share a subset of resources throughout their lifecycle

However, with the implementation of `fork()` using CoW, the only benefits gained from `vfork()` is the memory/cpu saved from copying resource duplicating the parent's page tables. The semantics of `vfork()` can be sometimes tricky.

`vfork()` follows the same workflow of calling `clone()` which in turn calls `do_fork()` like we saw previously for `fork()`. However, this `vfork()` syscall is implemented by passing a special flag to the `clone()` syscall.

Here is some of the behavior of `do_fork()` which diverges from `fork()` for `vfork()`:
1. During the `copy_process()` the `task_struct.vfork_done` member is set to null
2. Later on, the `task_struct.vfork_done` is pointed to a specific address (where ?)
3. After the child starts running, inside of returning a pointer to the child's `task_struct`, the parent is blocked until the child signals it through the `vfork_done` pointer
4. During the release of memory address ,(which is done when a task exist a memory address space by calling `mm_release()` syscall), the `task_struct.vfork_done` is checked. If `vfork_done` is not NULL then the parent is signalled
5. Back in `do_fork()` the parent wakes up and returns

Assuming the `vfork()` call was successful, the child is executing in a new address space, and the parent is executing again in its original address space. Although the overhead is lower, the implementation isn't pretty.


**TODO**: Add code examples
