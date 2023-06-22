
#include<unistd.h>
#include <stdio.h>
#include<stdint.h>
#include<stdlib.h>i
#include<err.h>

int main (int argc, char * argv[]) {
	
	if(argc != 2) {
		errx(1, "Wrong number args. Please enter <loop count>, <sleep time>");
	}
	
	int pipe_to_child[2];
	int pipe_to_parent[2];

	pipe(pipe_to_child);
	pipe(pipe_to_parent);

	int pid_id = fork();

	if(pid_id == 0){
		close(pipe_to_child[1]);
		close(pipe_to_parent[0]);
		uint8_t signal = 2;
		while(read(pipe_to_child[0], &signal, 1) > 0) {
			write(1, "DONG\n", 5);
			write(pipe_to_parent[1], &signal, 1); 
				
		}
		close(pipe_to_child[0]);
		close(pipe_to_parent[1]);
		exit(0);
		

	}

	
	uint8_t counter = (uint8_t)atoi(argv[1]);
	uint8_t sleep_time = (uint8_t)atoi(argv[2]);
	uint8_t signal = 1;

	for(uint8_t i = 0; i < counter; i++) {
		write(1, "DING\n", 5);
		write(pipe_to_child[1], &signal, 1);

		read(pipe_to_parent[0], &signal, 1);
		sleep(sleep_time);

	}

	close(pipe_to_child[1]);
	close(pipe_to_parent[0]);
	
	//printf(">>> child status: %d\n",child_end_status); 
	exit(0);	
	
}
