
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <err.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

/*
  P - watcher on Q
 limit_time = 1-9
	

limit_exec_fail = 2;
limit_exec_success = 3;

//Q? if Q is aways successful - we will always be here. 
//for testing purposses - limit_time_success;

do {

	fork()

	exec Q

	wait status
	
	write to log file

	if( WIFSIGNALED ) {

		exit(129)
	}


	if(status != 0 && stop - start < limit_time) {
		limit_exec_fail --;
		continue;
	}
	
	limit_exec_fail = 0;
	limit_exec_success--;

} while (exec_limit > 0 && limit_exec_success > 0);

exit(0);
  
  
 */


void write_log_row(time_t start_time, time_t end_time, int exit_status);

int main (int argc, char * argv[]){ 

	if(argc < 3) {
		errx(1, "Invalid number of args");
	}
	
	printf(">>> 2) %s\n", (char*)(argv+2)[1]);
	char** argv_q_pointer = (char **) (argv + 2);

	printf(">>> 3) %s\n", argv_q[1]);
	printf(">>> 3) argc %d\n",argc);

	char argv_q [argc-3] ;
	for(int i = 3; i >= argc ; i++) {
		//argv_q[i] = argv
	}
	


	if(  (atoi(argv[1])) < 1 || (atoi(argv[1])) > 9) {
		errx(1, "Invalid limit time");

	}

	int limit_exec_fail = 2;
	int limit_exec_success = 3;

	

	do{ 


		printf(">>>> fail: %d, success: %d \n", limit_exec_fail, limit_exec_success); 




		//test Q - ding dong
		//
		time_t start = time(NULL);

		int pid_id = fork();

		if(pid_id == 0){
			//if(execl("../82_2022-IN-01/main", argv[2], "2", (char*) NULL) == -1){i
			//TODO ----- pass all args 
			if(execl(argv[2], argv[2], (char *) argv_q, (char*) NULL) == -1){ 
				err(2, "Error exec Q");
			}
		}
		
		int q_status;
		wait(&q_status);
		time_t end = time(NULL);
	/*		
		int log_fd = open("run.log", O_CREAT | O_APPEND | O_WRONLY, S_IRUSR | S_IWUSR);
		if(log_fd == -1) {
			err(3, "Error open run.log");
		}

		

		char* log_row = strcat(start, " ");
		log_row = strcat(log_row, end);
		log_row = strcat(log_row, " ");
		log_row = strcat(log_row, q_status);

		printf(">>>> log row %s\n", log_row);

		int w_status;
		w_status = write(log_fd, &log_row, sizeof(time_t)*2 + 6);
		if(w_status == -1) {
			close(log_fd);
			err(4, "Error wrinting in log file");
		}
*/
		if( WIFSIGNALED(q_status )) {
			write_log_row(start, end, 129);
			exit(129);
		}
		
		int exit_status = WEXITSTATUS(q_status);
		write_log_row(start, end, exit_status);

		if(exit_status != 0 && (end-start) < atoi(argv[1])) {
			limit_exec_fail --;
			continue;
		}	

		limit_exec_fail = 2;
		limit_exec_success--;


	} while(limit_exec_fail > 0 && limit_exec_success > 0);

	exit(0);

}



void write_log_row(time_t start_time, time_t end_time, int exit_status){

/*
	char* log_row = strcat(start_time, " ");
	log_row = strcat(log_row, end_time);
	log_row = strcat(log_row, " ");
	log_row = strcat(log_row, exit_status);

	printf(">>>> log row %s\n", log_row);

	int log_fd = open("run.log", O_CREAT | O_APPEND | O_WRONLY, S_IRUSR | S_IWUSR);
	if(log_fd == -1) {
		err(3, "Error open run.log");
	}

	int w_status;
	w_status = write(log_fd, &log_row, sizeof(time_t)*2 + 6);
	if(w_status == -1) {
		close(log_fd);
		err(4, "Error wrinting in log file");
	}
*/
	
	int log_fd = open("run.log", O_CREAT | O_APPEND | O_WRONLY, S_IRUSR | S_IWUSR);
	if(log_fd == -1) {
		err(3, "Error open run.log");
	}


	int w_status;
	//w_status = write(log_fd, &start_time, sizeof(time_t));
	dprintf(log_fd, "%ld", start_time);
	w_status = write(log_fd, " ", 1);
	//w_status = write(log_fd, &end_time, sizeof(time_t));
	dprintf(log_fd, "%ld", end_time);
	w_status = write(log_fd, " ", 1);

	//w_status = write(log_fd, &exit_status, 4);
	dprintf(log_fd, "%d", exit_status);
	w_status = write(log_fd, "\n", 1);

	if(w_status == -1) {
		close(log_fd);
		err(4, "Error wrinting in log file");
	}



	return;

}

