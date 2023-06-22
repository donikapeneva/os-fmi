#include <err.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>



/* custom error exit status
 * 1: validation error
 *
 */


/* Thinking ...
 *
 * fork()
 *
 * parent 
 *  write "Ding" -> stdout
 *  tells child (pipe to child)
 *
 *	waits to child to answer
 *	waits D seconds
 *	again;
 *
 *
 *
 * child
 * 	read "Ding"/sth <- parent (pipe to child)
 * 	write "Dong" -> stdout
 * 	signals parent
 *
 * way2: signal from parent - counter
 *
 */


int main (int argc, char * argv[]) {

	if(argc != 3) {
		errx(1, "Wrong args count");
	}

	int execution_count = atoi(argv[1]);
	int wait_time =  atoi(argv[2]);


	int parent_to_child [2];
	int child_to_parent[2];

	pipe(parent_to_child);
	pipe(child_to_parent);


	int pid_id = fork();

 	if (pid_id == 0) {
		close(child_to_parent[0]);
		close(parent_to_child[1]);

		char signal_from_parent;
	
		int r_status;

		//ends when parent closes the door
		while((r_status = read(parent_to_child[0], &signal_from_parent, 1)) > 0) {
			printf("DONG\n");
			write(child_to_parent[1], "1", 1);
		}
		printf(">> parent closed the door\n");

		close(parent_to_child[0]);
		close(child_to_parent[1]);
		exit(0);
	

	} else {


		close(child_to_parent[1]);
		close(parent_to_child[0]);
		char signal_from_child;

		do {

			if(execution_count < atoi(argv[1])){
				sleep(wait_time);
			}

			printf("DING\n");

			execution_count--;

			write(parent_to_child[1], "1", 1);
				
		} while(read(child_to_parent[0], &signal_from_child, 1) > 0
				&& execution_count > 0) ;

		printf(">> coutn 0\n");

		close(child_to_parent[0]);
		close(parent_to_child[1]);

		wait(NULL);

		printf(">> child ended\n");
		exit(0);

	}




}
