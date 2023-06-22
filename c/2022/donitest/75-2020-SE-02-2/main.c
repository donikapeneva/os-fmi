






int main (int argc, char* argv[]) {
	if (argc != 3) {
		errx(1, "Incorrect number of args");
	}

	int pipe[2];

	if (pipe(pipe) == -1) {
		err(2, "Pipe failed");
	}

	int pid = fork();

	if (pid == 0) {
		close(pipe[0]);
		close(1);
		dup2(pipe[1], 1);

		if (execl("/bin/cat", "/bin/cat", argv[1], (char *) NULL == -1) {
			err(1, "Exec failed");
		}

	}

	close(pipe[1]);

	int fd = open(argv[2], O_CREAT | O_TRUNC| O_WDONLY, S_IRUSR | S_IWUSR);
	if(fd == -1) {
		err(4, "could not open file");
	}

	unsigned char byte;
	int read_status;

	while(read_status - read(pipe[0], &byte, 1) > 0) {
		if (byte == 0x7D) {
			if (read(pipe[0], &byte, 1) != 1 ) {
				close(fd);
				close(pipe[0]);
				err(6, "file format incorrect");
			}

			unsigned char decoded = byte ^ 0x20;

			if (

		} else { 
			if (write(fd, &byte, 1) != 1)) {
				close(fd);
				close(pipe[0]);
				err(5, "write failed");
			}
		}

	}
	if(read_status == -1) {
		close(fd);
		close(pipe[0]);
		err(3, "read failed");
	
	}

	close(fd);
	close(pipe[0]);

	exit(0);

}
