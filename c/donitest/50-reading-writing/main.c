#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <err.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

//int main(int argc, char* argv[]) {
int main(){
	int fd_numbers = open("./numbers", O_CREAT | O_TRUNC |O_RDWR, S_IRWXU);
	if (fd_numbers == -1) {
		err(1, "Error while open file");
	}

	int fd_coo = open("./coordinates", O_CREAT | O_TRUNC | O_RDWR, S_IRWXU);
	if (fd_coo == -1) {
		err(1, "Error while open file");
	}

	int pid_id = fork();
	if(pid_id == -1) {
		close(fd_numbers);
		err(3, "error while fork");	
	}

	

	if(pid_id == 0){
		//child
		//const char name[5] = "doni\n";
		//const long name[1] = {0x02, 0x45, 0x69, "a"};
		const int name[10] = {1, 1000000, 3, 4, 5, 6, 7 ,8 ,9,10} ;

		//const char name[10] = {'1', 2, 3, 4, 5, 6, 7 ,8 ,9,10} ;

		int written_size = write(fd_numbers, &name, 40);
	
		printf("write size %d\n", written_size);

		if (written_size == -1) {
			close(fd_numbers);
			err(2, "error while writing");
		}

		//close(fd_numbers);
		exit(0);

	} 

	int pid_id_file2 = fork();
	if(pid_id == -1) {
		close(fd_coo);
		err(3, "error while fork");	
	}

	if(pid_id_file2 == 0) {
		const int name[4] = {1, 2, 1, 5} ;

		//const char name[10] = {'1', 2, 3, 4, 5, 6, 7 ,8 ,9,10} ;

		int written_size = write(fd_coo, &name, 3);
	
		printf("write coo file size %d\n", written_size);

		if (written_size == -1) {
			close(fd_coo);
			err(2, "error while writing");
		}

		//close(fd_coo);
		exit(0);


	}
	
	//parent
	int child_num_status;
	int child_coo_status;

	printf("children pids %d %d \n", pid_id, pid_id_file2);
	waitpid(pid_id, &child_num_status, NULL);

	waitpid(pid_id_file2, &child_coo_status, NULL);

	printf("patent waited;\nchild num status %d \nchild coo status %d", child_num_status, child_coo_status);

	if(WIFEXITED(child_num_status) && ( WEXITSTATUS(child_num_status) == 0)
		&& WIFEXITED(child_coo_status) && ( WEXITSTATUS(child_coo_status) == 0 )) {
	
		printf("yeyyy\n", child_num_status);
	} else {
		close(fd_numbers);
		close(fd_coo);
		errx(5,"Something went wrong");
	}

	int fd_res = open("./res", O_CREAT | O_TRUNC | O_WRONLY, S_IRWXU);
	if (fd_res == -1) {
		err(1, "Error while open file");
	}

	printf("fd_num: %d, fd_coo: %d, fd_res: %d\n", fd_numbers, fd_coo, fd_res);
	int x;
	int y;
	int number;
	int xy[2] ;
	int cnt = 0;
	int read_status;
	
	lseek(fd_coo,0, SEEK_SET);
	lseek(fd_numbers,0, SEEK_SET);

	int size = lseek(fd_numbers,0, SEEK_END);
    printf("size numbers > %d\n", size);

	//read_status = read(fd_coo, &number, 4);

	while( ( read_status = read(fd_coo, &number, 4)) > 0) {
		printf("read > %d\n", number);

		if(cnt == 0){
			xy[0] = number;
			cnt++;
		} else {
			xy[1] = number;
			cnt--;

			printf("< %d ; %d > \n", xy[0], xy[1]);
			lseek(fd_numbers, xy[0]*4, SEEK_SET);
			int  read_pos;
			int poscnt = 0;
			while( (read_pos = read(fd_numbers, &number, 4)) > 0 && poscnt < xy[1]) {
				printf("read position:  %d; cnt= %d\n", read_pos,poscnt);
				poscnt++;
				write(fd_res, &number, 4);
			}
			
		
			if(read_pos ==-1) {
				close(fd_numbers);
				close(fd_coo);
				close(fd_res);
				err(3, "error with reading fd_numbers");
			}

		}
		
		//printf("< %d ; %d > \n", xy[0], xy[0]);
	}
	printf("read status > %d\n", read_status);

	if(read_status == -1) {
		close(fd_numbers);
		close(fd_coo);
		
		err(7, "Error reading file");
	}
	
	exit(0);
}
