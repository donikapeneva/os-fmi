#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

#include <sys/wait.h>


//async children
int main() {

	int pf [2];
	pipe(pf);

	printf(">>> pipe [%d] [%d]\n", pf[0], pf[1]);

	int i = 9; 

	while (i >= 0) {
		int pid_id = fork();

		if(pid_id == 0){
			char number = i;

			int w_status = write(pf[1], &number, 1);

		//	printf(">>> status %d\n", w_status);
			printf(">>> child %d\n", i);
			
			close(pf[1]);
			//child ends
			exit(0);
		}
	

		i--;
	}


	//parent

	close (pf[1]);
	char input;


	while(read(pf[0], &input, 1) > 0) {
		printf(">>> read %d\n", input);

	}

	close(pf[0]);
	exit(0);

}
