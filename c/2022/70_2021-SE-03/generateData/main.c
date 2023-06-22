#include <err.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main() {

	//args: input file --> uint16_t  (created on little endian)
	//						<= 524288 elements
	// 		output file --> c header file 
	// 						arr[] := all elements form input;
	// 								no size
	// 								const
	// 						arrN : uint32_t -> size of arr
	//

	/*
 		validate input file -> 
			1 int = 4 byte = 32 bit
			arrN = lseek - last position / sizeof uint16
		arr_count = 0;
	  	lseek - set 0
		malloc? arr[] -->  or directly write to header file :) 

		while read element (uint16)
			arr push element --> write to file
			arr_count++

		additional check for arr count == arrN
		
		create new file (header file )
	   	write > declaration... 
		
		for element in arr[] - write element
			
	
	
	*/

	int fd = open ("testData", O_CREAT | O_TRUNC | O_RDWR, S_IRUSR | S_IWUSR );

	if(fd == -1) {
		err(1, "Error opening testData");
	}

	uint16_t elements = 50;

	while (elements > 0) {
		int w_status = write(fd, &elements, sizeof(uint16_t));
		if (w_status == -1) {
			close(fd);
			errx(2, "Error writing in data");
		}

		elements--;
	}

	printf(">>> test data generated.\n");

	close(fd);
	exit(0);

}
