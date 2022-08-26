#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <err.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>




/* custom error exit number
 * 1 : args validation
 * 2 : open files
 * 3 : read operations
 * 4 : write operations
 *
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



int main(int argc, char * argv[]){
	if(argc != 3) {
		errx(1, "Wrong args number");
	}


	int data_fd = open(argv[1], O_RDWR);
	if(-1 == data_fd) {
		err(2, "Error opening data file"); 
	}


	int comp_fd = open(argv[2], O_RDWR);
	if(-1 == comp_fd) {
		close(data_fd);
		err(2, "Error opening compt file"); 
	}

	//read data header file  -------------- 
	int r_status;
	uint32_t count_data;
	uint32_t magic_data;
	uint32_t magic_data_cmp = 0x21796F4A;

	r_status = read(data_fd, &magic_data, sizeof(uint32_t));
	
	if(r_status == -1){
		close(data_fd);
		close(comp_fd);
		err(3, "Error reading from data");
	}

	//printf(">>> magic data : %d magic data cmp :%d\n",magic_data, magic_data_cmp); 
	if(magic_data != magic_data_cmp) {
		close(data_fd);
		close(comp_fd);
		errx(1, "Data file is not in expected format");
	}
	
	r_status = read(data_fd, &count_data, sizeof(uint32_t));
	
	if(r_status == -1){
		close(data_fd);
		close(comp_fd);
		err(3, "Error reading from data");
	}

	printf(">>>> data header --\ncount: %d\n", count_data);  

	//get current position 
	int header_end_data = lseek(data_fd, 0, SEEK_CUR);
	printf(">>> data header end position: %d\n", header_end_data);

	//read comp header file  -------------- 
	uint32_t magic_1;
	uint16_t magic_2;

	uint32_t magic_1_secret = 0xAFBC7A37;
	uint16_t magic_2_secret = 0x1C27;

	r_status = read(comp_fd, &magic_1, sizeof(uint32_t));
	r_status = read(comp_fd, &magic_2, sizeof(uint16_t));

	if(r_status == -1){
		close(data_fd);
		close(comp_fd);
		err(3, "Error reading from comparator");
	}


	if(magic_1 != magic_1_secret || magic_2 != magic_2_secret) {
		close(data_fd);
		close(comp_fd);
		errx(1, "Comparator file is not in expected format");
	}


	//comparator header

	uint64_t count_cmp;

	int s_status;

	s_status = lseek(comp_fd, sizeof(uint16_t), SEEK_CUR);

	if(s_status == -1) {
		close(data_fd);
		close(comp_fd);
		err(3, "Error seek from comparator");
	}

	r_status = read(comp_fd, &count_cmp, sizeof(uint64_t));
	
	if(r_status == -1){
		close(data_fd);
		close(comp_fd);
		err(3, "Error reading from data");
	}


	printf(">>>> comp header --\ncount: %ld\n", count_cmp); 



	//THE REAL DEAL
	
	// 0 - <
	// 1 - >
	uint16_t type;
	uint32_t offset1;
	uint32_t offset2;

	uint64_t el1;
	uint64_t el2;

	int w_status;

	while( count_cmp > 0 ) {
		count_cmp--;

		//todo check r_status; s_sttaus
		r_status = read(comp_fd, &type, sizeof(uint16_t));
		//skip reserved
		s_status = lseek(comp_fd, sizeof(uint16_t) * 3, SEEK_CUR);
		r_status = read(comp_fd, &offset1, sizeof(uint32_t));
		r_status = read(comp_fd, &offset2, sizeof(uint32_t));


		//todo check if offset > count elements in data -> error
		if(offset1 > count_data || offset2 > count_data) {
			close(data_fd);
			close(comp_fd);
			errx(1, "Incorrect offset value");
		}


		//debug
		printf("<type, off1, off2> <%d, %d, %d> \n",type, offset1, offset2); 

		//get el1
		//not sure about this: 
		//since we have header - we should skip it, and go to the data els0
		
		// offset = offset1*64bits + magic data + count
		s_status = lseek(data_fd, offset1*sizeof(uint64_t) + header_end_data, SEEK_SET);
		r_status = read(data_fd, &el1, sizeof(uint64_t));

		//get el2
		s_status = lseek(data_fd, offset2*sizeof(uint64_t) + header_end_data , SEEK_SET);
		r_status = read(data_fd, &el2, sizeof(uint64_t));

		//debug
		printf("<el1, el2> <%ld, %ld> \n",el1, el2); 


		//compare el1 ? el2
		

		if(el1 == el2) {
			continue;
		}

		if(type == 0 && el1 < el2) {
			//all good
			printf("all good\n");
			continue;
		} else if (type == 1 && el1 > el2) {
			printf("all good\n");
			continue;
		}


		printf("not  good----- \n");

		//reset pointer in data
		//else -> restart fd pointer (lseek offset1; write 64 = el2)
   		//	resttart fd ponter (lseek offset2; write 64 = el1)	
		
		s_status = lseek(data_fd, offset1*sizeof(uint64_t) + header_end_data, SEEK_SET);
		w_status = write(data_fd, &el2, sizeof(uint64_t));

		s_status = lseek(data_fd, offset2*sizeof(uint64_t) + header_end_data , SEEK_SET);
		w_status = write(data_fd, &el1, sizeof(uint64_t));

	}







	close(data_fd);
	close(comp_fd);


	exit(0);
}

