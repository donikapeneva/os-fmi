
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <fcntl.h>




int main () {
	
	int fd = open("testres", O_CREAT | O_TRUNC | O_RDWR, S_IRWXU);

	write(fd, "FFAB", 4);

	uint8_t byteA = 0x01;
	uint8_t byteB = 0x00;
	write(fd, &byteA, 1);
	write(fd, &byteB, 1);

		write(fd, "FFBA", 4);

	
	write(fd, &byteB, 1);

	write(fd, &byteA, 1);

write(fd, "FFTN", 4);


	uint16_t test_number = 0x0010;
	printf(">> number %d\n", test_number); 
	write(fd, &test_number, sizeof(uint16_t));

write(fd, "FFAA", 4);

	
	uint8_t first_byte = (uint8_t)test_number;
	printf(">> byte 1 %d\n", first_byte); 
	write(fd, &first_byte, sizeof(uint8_t));

write(fd, "FFBB", 4);

	uint8_t second_byte = ((uint8_t)test_number) >> 8;
	printf(">> byte 2 %d\n", second_byte); 
	write(fd, &second_byte, sizeof(uint16_t));


	exit(0);

}
