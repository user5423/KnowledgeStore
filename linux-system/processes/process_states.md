
### Process State

A process descriptor (`task_struct`) has a field called `state` which defines the current state of the process. Every process on the system must have exactly one of five states:
- `TASK_RUNNING` - The task is runnable - it is either currently running or in a run queue. This is the only possible state for a process executing in user-space; it can also apply to a process in kernel-space that is actively running
- `TASK_INTERRUPTABLE` - The task is currently sleeping (i.e. blocked) and is waiting for some condition to exist. When this condition exists, the kernel will set the process descriptor's state to `TASK_RUNNING`. Alternatively, if a signal is received, then the kernel awakes the task prematurely, by setting its state to `TASK_RUNNING`
- `TASK_UNINTERRUPTABLE` - In some cases,  we may want tasks to be able to sleep until some condition exists, **without** being able to be prematurely worken up by a signal. This is used a lot less than `TASK_INTERRUPTABLE` as this behavior is usually not wanted in most cases. This is why you have unkillable processes some time (since the process will not respond to signals while in this state).
- `__TASK_TRACED` - The process is being `traced` by another process (e.g. a debugger using `ptrace`)
- `__TASK_STOPPED` - The task is no longer runnable. Its execution has stopped and it is not eligible to run any more. This occurs after a task receives the signals `SIGSTOP`, `SIGTSTP`, `SIGTTIN`, `SIGTTOU` or if it receives *any* signal while it is being debugged


The set of process states can be modelled by a FSM, which can be seen in the diagram below:

![Process State FSM](assets/process_state_fsm.png)

### Manipulating the current process state

The Kernel will often need to change the state of processes using the macro `set_task_state(task, state)` defined in `<linux/sched.h>`.

On SMP systems, it also provides a memory barrier to enforce ordering of process execution on other processors. Memory barriers are also used to enforce consistency to avoid unexpected behavior from out-of-order execution. Here are some more resources on memory barriers and memory ordering:
- https://preshing.com/20120710/memory-barriers-are-like-source-control-operations/
- https://preshing.com/20120625/memory-ordering-at-compile-time/
- https://preshing.com/20120930/weak-vs-strong-memory-models/

The method `set_task_state(task, state)` is equivalent to `set_current_state(state)`. If you'd like to see more of these macros and functions, view the `<linux/sched.h>` header file.


