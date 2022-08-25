#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


//cool pipes :)
int main (int argvc, char * argv[]) {
	
	int pf [2];
	pipe(pf);
	printf(">>> pipe [%d] [%d]\n", pf[0], pf[1]);

	int i = 0;
	while(i < 10) {
		int pid_id = fork();
		if(pid_id == 0) {
			char number = i;
			int status = write(pf[1], &number, 1);
			
			printf(">>> status %dn", status);
			printf(">>> write %d\n", number);

			if(i!=1){
				sleep(2);
			}
			printf(">>> write %d\n", number);

			close(pf[1]);
			exit(0);

		}
		i++;
	}

	close(pf[1]);
	
	//parent
	char input;

	int child_status;
	//wait(&child_status);
	//printf(">>> child status %d\n", child_status);

	while(read(pf[0], &input, 1) > 0) {
	printf(">>> read %d\n", input);
	}
	close(pf[0]);
	exit(0);

}
