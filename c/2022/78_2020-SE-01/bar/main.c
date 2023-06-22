#include <err.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/wait.h>

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

/* open fifo
 *
 * dup stdin -> fifo
 * exec -> command argv1 
 *
 * BAR
 *
 */


int main (int argc, char * argv[]) {

	if(argc != 2) {
		errx(1, "Invalid args count");
	}

	int fifo_fd = open("../shared-fifo", O_RDONLY);
	int dup_status = dup2( fifo_fd, 0);
	
	printf(">>>> child will exec comand %s\n", argv[1]);
	int exec_status = execl( argv[1], argv[1], (char*) NULL);
	
	if(exec_status == -1) {
		close(fifo_fd);
		err(3, "Exec failed");
	}

}
