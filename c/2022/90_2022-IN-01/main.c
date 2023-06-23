#include<unistd.h>
#include<err.h>
#include<stdlib.h>

int main(int argc, char ** argv) {

	if(argc != 3) {
		errx(1, "expected <N> <M>");
	}

	int n = atoi(argv[1]);
	int m = atoi(argv[2]);
	if(n > 9 || n < 1 || m > 9 || m < 1) {
		errx(1, "expected N and M are [1-9]");
	}

	int child_to_parent[2];
	int parent_to_child[2];
	int output[2];

	if(pipe(child_to_parent) != 0) {
		err(2, "Error while opening child to parent pipe");
	}
	if(	pipe(parent_to_child) != 0 ){
		err(2, "Error while opening parent to child pipe");
	}
	if(	pipe(output) != 0 ){
		err(2, "Error while opening output pipe");
	}

	close(output[0]);
	dup2(1, output[1]);

	int pid = fork();
	
	if(pid == 0) {
		//child
		close(parent_to_child[1]);
		close(child_to_parent[0]);
		
	} else { 
		//parent
		close(parent_to_child[0]);
		close(child_to_parent[1]);
	}

	int count = 0;
	char signal = 1;
	if(pid > 0) {
		do {
			count++;
			if(write(output[1], "DING\n", 5) == -1) {
				errx(3, "error while writing to output");
			}
			
			if(write(parent_to_child[1], &signal, 1) == -1){
				errx(3, "errro while signal child");
			}

			sleep(m);

		} while (read(child_to_parent[0], &signal, 1) > 0 && count < n);
	}

	if(pid == 0) {
		while (read(parent_to_child[0], &signal, 1) > 0) {
			int ow_res = write(output[1], "DONG\n", 5);\
			if(ow_res == -1) {
				errx(4, "Error whild child writing to output");
			}
			int sw_res = write(child_to_parent[1], &signal, 1);
			if(sw_res == -1) {
				errx(4, "Error while childs signals to parent");
			}
		}
	}


	//end
	if(pid == 0) {
		close(parent_to_child[0]);
		close(child_to_parent[1]);
		
	} else { 
		close(parent_to_child[1]);
		close(child_to_parent[0]);

	}
	close(output[1]);

	return 0;
}
