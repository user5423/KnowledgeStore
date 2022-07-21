

### Linux Implementation of Threads

Threads are a popular programming abstraction that allow for multiple threads of exuection within the same process in a shared memory addres space. Additionally, they can share resources such as open files and signals. Threads enable *concurrent programming* and on multiprocessor system, *true parallelism*

The Linux Kernel has a unique implementation of threads. There is no concept of a thread, and threads are constructed as standard processes and described in the same way (e.g. using `task_struct`). The Kernel doesn't provide any special scheduling semantics or data structures to represent the thread. Instead, threads just happen to share resources such as address spaces with other processes.

In other Operating Systems such as Windows or Solaris, their kernels have explicit support for threads which are a different construct from processes. In these types of systems, threads are sometimes called **lightweight** processes as threads are meant to be an abstraction that provides a lighter and quicker exuection until than a heavy process

However, in Linux, threads are essentially processes that happen to share s set of resources with each other (e.g. memory address space, open files, etc.). In Linux threads and process both have `task_structs` in the same task list. The threads are children of the parent parent process, and the threads are setup to share certain resources from the parent process.

In other Operating Systems, you might have a process descriptor that points to several threads, where the process descriptor describes the shared resources between the threads.


#### Creating Threads

**TODO**: Continue finishing notes
