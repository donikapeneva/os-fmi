#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <stdint.h>
#include <stdio.h>
#include <err.h>
#include <math.h>



/*
  
  read binary file
  - bit 1 -> out 10
  - bit 0 -> out 01

  1011 0110 = 0xB6
  => 1001 1010 0110 1001 = 0x9A69

	
goal		  	 1011 0110


read 1 byte		 1001 1010
helper byte 	 0000 0001



for int i=0; i < 8; i++) 

	if read_byte && helper byte = 2^i;
	
		bit is 1;
	
	else bit is 0;

	  
 */

int powered(int x, int y){
	if( y == 0) {
		return 1;
	}
	int res  = 1;
	for(int i = 0; i < y; i++) {
		res *= x;
	}

	return res;
}

int main (int argc, char * argv[]) {
	/* TEST DATA 
	int decoder_fd = open("decoded-message", O_CREAT | O_TRUNC | O_WRONLY, S_IRUSR | S_IWUSR);
	uint8_t test = 0xB6;
	write(decoder_fd, &test, 1);
	 test = 0x7d;
	write(decoder_fd, &test, 1);
	 test = 0x69;
	write(decoder_fd, &test, 1);
	 test = 0x9a;
	write(decoder_fd, &test, 1);
	err(1, "WFT");
	close(decoder_fd);
	*/
	
	if(argc != 3) {
		errx(1, "Wrong number of args");
	}

	int encoded_fd = open(argv[1], O_RDONLY);
	if(encoded_fd == -1) {
		err(2, "Error opening encoded)");
	}

	int decoded_fd = open(argv[2],  O_CREAT | O_TRUNC | O_WRONLY, S_IRUSR | S_IWUSR);
	if(decoded_fd == -1) {
		close(encoded_fd);
		err(2, "Error opening edecoded)");
	}


	uint16_t bytes;
	uint16_t helper = 2;

	int decoded_bytes[8];


	int decimal_power = 1;



	
	int r_status;

	while( (r_status = read(encoded_fd, &bytes, 2)) > 0){
		
		uint16_t res = 0;
		int stepen_na_2 = 0;
		int counter_power_2 = 0;

		printf("---- reading------------: %x res=%d \n", bytes, res);
	
		for(int i=0; i < 17; i=i+2) {
			
			//cant make pow() wokr
			// because start = 10 bitwising << twice				 
			//printf(">>> counter : %d\n);
			stepen_na_2 = powered(2, counter_power_2);	
						
			printf(">>> i=%d\t| stepen na 2: %d\t| helper %d\n", i, stepen_na_2, helper<<i);			
			printf(">>> bytes & helper %d \n", bytes&(helper<<i));
			
			int digit_value;
			if((bytes&(helper<<i)) == (helper<<i)) {
				digit_value = 1;
							
			}else {
				digit_value = 0;
				
			}

			res += (digit_value * stepen_na_2);
			printf(">>>> res: %d digit: %d * %d\n", res, digit_value, stepen_na_2);


			//helper = (helper<<2);
			counter_power_2 ++;

		}
		printf(">>>> res: %d\n", res);
		write(decoded_fd, &res, sizeof(uint8_t));
	}
	
	close(encoded_fd);

}
