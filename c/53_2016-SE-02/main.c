#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <err.h>
#include <unistd.h>
#include <stdint.h>
#include <stdio.h>


int main() {

	///*
	//generate test content -------------	
	int coordinates_fd = open("./f1", O_RDWR | O_TRUNC);
	if(coordinates_fd == -1) {
		err(1, "Error while opening file 1");
	}

	//generate coordinates test content
	uint32_t coordinates [2][2] = {{1, 10}, {5, 25}};
	int write_status = write(coordinates_fd, coordinates, 4*sizeof(uint32_t)); 

	if(write_status == -1) {
		close(coordinates_fd);

		err(1, "Error while writing coo");

	}

	int numbers_fd = open("./f2", O_RDWR | O_TRUNC);
	if(numbers_fd == -1) {
		close(coordinates_fd);
		err(1, "Error while opening file 2");
	}

	uint32_t numbers = 1;
	while(numbers < 21){ 
		int write_status_n = write(numbers_fd, &numbers, sizeof(uint32_t)); 
		numbers++;
		if(write_status_n == -1) {
		close(coordinates_fd);
		close(numbers_fd);

		err(1, "Error while writing coo");

	}
	}


	lseek(coordinates_fd, 0, SEEK_SET);
	lseek(numbers_fd, 0, SEEK_SET);

	//--------------------------------------
	//*/

	/*
	int coordinates_fd = open("./f1", O_RDONLY);
	if(coordinates_fd == -1) {
		err(1, "Error while opening file 1");
	}

	
	int numbers_fd = open("./f2", O_RDONLY);
	if(numbers_fd == -1) {
		close(coordinates_fd);
		err(1, "Error while opening file 2");
	}
	*/
			
	int res_fd = open("./f3", O_CREAT | O_WRONLY, S_IRWXU);
	if(res_fd == -1) {
		close(coordinates_fd);
		close(numbers_fd);
		err(1, "Error while opening file 3");
	}

	uint32_t number; 
	uint32_t start_position;
	uint32_t count_numbers;

	int read_status;
	int read_status_2;
	int write_status_2;


	int cntr = 0;

	while( (read_status = read(coordinates_fd, &number, sizeof(uint32_t))) > 0 ) {
		 
		
		if(cntr == 0 ) {
			start_position = number*sizeof(uint32_t);
			cntr++;
			continue;
		}

		if(cntr == 1) { 
			count_numbers = number;
			cntr--;
		}

		printf("start position : %d\nlenght : %d\n", start_position, count_numbers); 

		lseek(numbers_fd, start_position, SEEK_SET);

		while ((read_status_2 = read(numbers_fd, &number, sizeof(uint32_t))) > 0 
				&& count_numbers > 0) { 
			
			printf("reading: %d\n", number);	
			write_status_2 = write(res_fd, &number,  sizeof(uint32_t));


			if(write_status_2 == -1) {
				err(3, "Error while writing");
			}

			count_numbers--;
		}

		if(read_status_2 == -1) {
			err(2, "Error while reading numbers file");
		}



	}
	printf("debug : %d\n", read_status); 
	if(read_status == -1) {
		err(2, "Error while reading coordinates file");
	}


	close(coordinates_fd);
	close(numbers_fd);	
	close(res_fd);

}
