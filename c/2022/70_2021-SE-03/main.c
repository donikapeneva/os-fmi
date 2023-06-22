#include <err.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>


/*
 * err exit number
 * 1 : args validation
 * 2 : opening files
 * 3 : validating content
 * 4 : lseek operation
 * 5 : reading
 * 6 : writing
 *
 */


int main(int argc, char * argv[]) {

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

	if(argc != 3 ) {
		errx(1, "Wrong arg count");
	}

	/* endianness
 	 * 1: Little endian
	 * 0: Big endian
	uint16_t word = 0x0001;
	char * b = (char * ) &word;
	printf("b[0] = %d\n", b[0]);
	*/

	//open input file
	int input_fd = open(argv[1], O_RDONLY);
	if(input_fd == -1) {
		err(2, "Error while opening input file");
	}

	int lenght_bytes = lseek(input_fd, 0, SEEK_END);

	if(lenght_bytes == -1) {
		close(input_fd);
		err(4, "Error with lseek end of file");
	}

	//debug
	printf(">>> lseek position, arrN %d\n", lenght_bytes);
	
	uint32_t elements = lenght_bytes / sizeof(uint16_t);
	
	//debug
	printf(">>>elements, %d\n", elements);

	if( elements > 524288) {
		close(input_fd);
		errx(3, "Too many elements in input file");
	}

	int s_status = lseek(input_fd, 0, SEEK_SET);
	if(s_status == -1) {
		close(input_fd);
		err(4, "Error with lseek start of file");
	}


	//create output file
	int output_fd = open(argv[2], O_CREAT | O_TRUNC | O_WRONLY, S_IRWXU);
	if(output_fd == -1) {
		close(input_fd);
		err(2, "Error while opening output file");
	}

	int w_status = write(output_fd, "const uint16_t arr[] = {", 24);
	if(w_status == -1) {
		close(input_fd);
		close(output_fd);
		err(6, "Error while writing in output file");
	}

	uint16_t element;
	int r_status;
	uint32_t element_count = 0;

	while((r_status = read(input_fd, &element, sizeof(uint16_t))) == sizeof(uint16_t)) {
		element_count++;
		printf(">>> element %d\n", element);

//>>>>>>>>> DPRINTF ---- 
		dprintf(output_fd, "%d", element);
//w_status = write(output_fd, &element, sizeof(uint8_t));
		if(element_count < elements) {
			w_status = write(output_fd, ",", 1);

			if(w_status == -1){
				close(input_fd);
				close(output_fd);
				err(6, "Error while writing elements in output file");
			}
		}

	}

	if(r_status == -1) {
		close(input_fd);
		err(5, "Error while reading elements from input file");
	}	

	w_status = write(output_fd, "};\n uint32_t arrN=", 18);
	if(w_status == -1){
		close(input_fd);
		close(output_fd);
		err(6, "Error while writing end of arra in output file");
	}

	
	dprintf(output_fd, "%d", elements);
/*
	w_status = write(output_fd, &elements, sizeof(uint32_t));
	if(w_status == -1){
		close(input_fd);
		close(output_fd);
		err(6, "Error while writing end of arra in output file");
	}
	*/

	w_status = write(output_fd, ";\n", 2);
	if(w_status == -1){
		close(input_fd);
		close(output_fd);
		err(6, "Error while writing end of output file in output file");
	}

	
	//todo check if elements are correct
	
	close(input_fd);
	close(output_fd);
	exit(0);
	

}
