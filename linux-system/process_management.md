## Section 2: Process Management

### What is a process?

A process is the set of resources used by an executing program. This includes resources such as:

- **Open files and pending signals** (i.e. I/O)
  - In Linux, most resources are stored as files, meaning we can interact with them using the system calls including:
    - `int fputc( int c, FILE *fp )` - Writes the char `c` to the file stream `fp`
    - `int fputs( char *s, FILE *fp )` - Writes the char array `*s` to the file stream `fp`
    - `int fgetc( FILE *fp)` - Reads a char `c` from the file stream `fp`
    - `int fgets( char *s, int size, FILE *fp)` - Reads at most `size` chars from the file string `fp` into the char array `*s`
  - Signals help faciliate control of execution and the process state. There are a large number of *standard signals* that the kernel defines. These can be found in the [man pages](https://man7.org/linux/man-pages/man7/signal.7.html) under `Standard Signals`
- **Internal kernel data**
  - The main point of having a kernel is to manage userspace processes. As will be seen shortly, in order to manage processes (and threads), the kernel needs to store data about them 
- **Processor state**
  - The processor has several registers and memory locations that it uses in order to execute the current instruction including
    - **Program Counter (PC)** - Provides the address of the next instruction to be executed
    - **Registers** - Contains small amount of data that may be used in intermediate calculations
    - more!!
- **Memory address space**
  - A memory address space is a space in virtual memory that is available for a process to use
  - Memory address spaces can have multiple memory mappings (e. many processes have access to the same memory address space). A memory space of a given process is accessable by all its threads
  - with different segments in the virtual memory address space
    - ***text segment*** - this is the address space of a process which contains the executable instructions of the process
    - ***data segment*** - this is the address space of a process which contains static variables (i.e. variables which span the lifetime of the program)
    - ***bss segment*** - this is the address space of a process which contains static variables that have been declared but **not** been initialized
    - ***stack segment*** - this is the address space of a process that contains stack structures where each frame pushed or popped holds a state of the function. Each thread has its own stack inside the stack segment to ensure that their flow execution is independent. The information stored on the state of the process is dependent on the OS, but usually contains
      - local variables
      - parameter arguments
      - return address
      - return value address (i.e. where to store)
    - ***heap segment*** - this is the address space of a process that contains dynamically allocated memory which is shared by all threads and shared libraries / dll. Unlike *stack* memory, this isn't garbage collected meaning that the programmer has to allocate and deallocate memory (to avoid leaks)


### Creating processes

**TODO**: Add code examples

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


