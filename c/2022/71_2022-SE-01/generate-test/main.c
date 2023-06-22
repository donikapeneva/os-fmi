#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <err.h>

/* 
 * * GENERATE TEST DATA *
 */


/*
 * Thinking ..... 
 	data bin
	comparator bin


	validate file formats by the magic val ?
	
	data bin ---
	elemnts - uint64_t

	comparator bin---
	<uint16, 16, 		16			16, 	32, 	32>
	<type, reserved, reserved, reserved, offset1, offset2>
	type: 0 (<) | 1(>);
	---- 
	offset1 type offset2
	if not -> swap

	get all elements count; < ?
	
	read from comparator
	type = get 0|1 from comparator row
	el1 = get element on offset1 from data.bin (lseek, read 64) 
	el2 = .............  offset2 from data.bin (lseek (SET), read 64)
	compare
	el1 >?< el2
	if true -> continue
	else -> restart fd pointer (lseek offset1; write 64 = el2)
   			resttart fd ponter (lseek offset2; write 64 = el1)	
				

 */



int main(){

	//generate data 
	int data_fd = open("data.bin", O_CREAT | O_TRUNC | O_WRONLY, S_IRWXU);
	
	//header
	uint32_t magic_h = 0x21796F4A;
	uint32_t count = 10;
	int w_status;

	w_status = write(data_fd, &magic_h, sizeof(uint32_t));
	w_status = write(data_fd, &count, sizeof(uint32_t));


	if(w_status == -1){ 
			close(data_fd);
			err(1, "Error writing in data");
		}


	uint64_t numbers = 1;

	//data
	while(numbers <= 10) {

		 w_status = write(data_fd, &numbers, sizeof(uint64_t));
		if(w_status == -1){ 
			close(data_fd);
			err(1, "Error writing in data");
		}
		numbers++;
	}
	
	printf(">>>> data generated\n");

	//generate conparator
	
	int comp_fd = open("comparator.bin", O_CREAT | O_TRUNC | O_WRONLY, S_IRWXU);
	
	//header
	uint32_t magic1 = 0xAFBC7A37;
	uint16_t magic2 = 0x1C27;

	uint16_t reserved = 0;
	uint64_t count_c = 3;

	write(comp_fd, &magic1, sizeof(uint32_t));
	write(comp_fd, &magic2, sizeof(uint16_t));
	write(comp_fd, &reserved, sizeof(uint16_t));
	write(comp_fd, &count_c, sizeof(uint64_t));


	//content
	// <uint16, 16, 		16			16, 	32, 	32>
	// <type, reserved, reserved, reserved, offset1, offset2>
	uint16_t type_1 = 0;
	uint16_t reserved_1 = 0;
	uint32_t offset1_1 = 5;
	uint32_t offset2_1 = 3;

	write(comp_fd, &type_1, sizeof(uint16_t));
	write(comp_fd, &reserved_1, sizeof(uint16_t) * 3);
	write(comp_fd, &offset1_1, sizeof(uint32_t));
	write(comp_fd, &offset2_1, sizeof(uint32_t));

	uint16_t type_2 = 0;
	uint16_t reserved_2 = 0;
	uint32_t offset1_2 = 6;
	uint32_t offset2_2 = 8;

	write(comp_fd, &type_2, sizeof(uint16_t));
	write(comp_fd, &reserved_2, sizeof(uint16_t) * 3);
	write(comp_fd, &offset1_2, sizeof(uint32_t));
	write(comp_fd, &offset2_2, sizeof(uint32_t));

	uint16_t type_3 = 1;
	uint16_t reserved_3 = 0;
	uint32_t offset1_3 = 7;
	uint32_t offset2_3 = 9;

	write(comp_fd, &type_3, sizeof(uint16_t));
	write(comp_fd, &reserved_3, sizeof(uint16_t) * 3);
	write(comp_fd, &offset1_3, sizeof(uint32_t));
	write(comp_fd, &offset2_3, sizeof(uint32_t));


printf(">>>> comp generated\n");

close(data_fd);
close(comp_fd);



	exit(0);
}

