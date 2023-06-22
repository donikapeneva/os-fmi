#include <err.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/wait.h>

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

/* mkfifo
 *
 * dup stdout -> fifo
 * exec -> cat 
 *
 * FOO
 *
 */


int main (int argc, char * argv[]) {

	if(argc != 2) {
		errx(1, "Invalid args count");
	}

	int f_status = mkfifo("../shared-fifo", S_IRUSR | S_IWUSR);
	if(f_status == -1) {
		err(2, "Error creating fifo");
	}


	int fifo_fd = open("../shared-fifo", O_WRONLY);

	int dup_status = dup2(fifo_fd, 1);


	printf(">>>> child will exec\n");
	int exec_status = execl("/usr/bin/cat", "cat", argv[1], (char*) NULL);
	if(exec_status == -1) {
		close(fifo_fd);
		err(3, "Exec failed");
	}
}

