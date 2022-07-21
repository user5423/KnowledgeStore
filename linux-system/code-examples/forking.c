#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char **argv){
	pid_t entry_pid = getpid();
	printf("The current pid of this entry process is %d \n\n", entry_pid);

	pid_t c;
	pid_t parent_pid;
	pid_t current_pid;

	int child_num = 6;
	// We iterate and fork() `child_num` number of children processes
	for (int i = 0; i < child_num; i++){
		// the parent fork() call returns the pid of the child
		// the child fork() call returns 0 if the fork was succesful
		c = fork();
		if (c == 0){
			// If the fork was succesful than the child needs to break
			// out of the for loop, otherwise we'd have n*(n-1)/2 forks
			// as otherwise the child and parent would continue forking
			break;
		}
	}
	
	// each process (incl parent and fork has a different current_pid
	current_pid = getpid();
	// each fork should have the same parent_pid
	parent_pid = getppid();
	
	if (entry_pid == current_pid){
		printf("I am the parent with the pid %d\n", entry_pid);
	} else {
		printf("I am a child with pid %d and my parent has pid %d\n", current_pid, parent_pid);
	}

	if(current_pid != entry_pid){
		// Here we are exiting the process if it is a child
		// We exit with exit-code 0 to represent success
		exit(0);
	}

	printf("\nI am the last remaining process and I am the entry process\n");
	printf("My pid is %d", current_pid);

}
