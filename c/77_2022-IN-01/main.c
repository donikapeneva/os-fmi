#include<fcntl.h>
#include<err.h>
#include<unistd.h>
#include<stdint.h>
#include<stdio.h>

	// open files
	// get element from list.bin, 
	//		get its offset
	//			by 1) lseek? -> offset = (current byte(=lseek) - header(size)) / sizeof(el))
	// 			   2) counter
	//		get its value
	//			read el sizeof list
	// get element from data.bin 
	//		get the element on offset (+data header)
	//			read size of data el
	// write to out.bin
	//	lseek to position value (+ out header)
	//	write the element from data.bin
#define LIST 1
#define DATA 2
#define OUT 3

struct header {
	uint16_t magic;
	uint16_t filetype;
	uint32_t count;
};

int main (int argc, char ** argv) {
	
	if	(argc != 4 ) {
		errx(1, "expected <list.bin> <data.bin> <out.bin>");
	}

	int list_fd = open(argv[1], O_RDONLY);
		

	// --- validate list file
	// todo: check header read bytes
	// todo: count of items ?
	if(list_fd == -1) {
		err(2, "error open list file");
	}

	struct header list_header;
	int read_size_header;
	if((read_size_header = read(list_fd, &list_header, sizeof(struct header))) == -1) {
		err(2, "error read list header");
	}
	if(read_size_header != sizeof(struct header)) {
		errx(2, "wrong list file header");
	}

	//todo: WHY
	if(list_header.magic != 0x5a4d){
		errx(2, "sorry, cannot unlock the file list with your magic");
	}

	//better with fstat :)
	uint16_t list_end = lseek(list_fd, 0, SEEK_END);
	uint16_t list_actual_elements = (list_end - sizeof(struct header)) / sizeof(uint16_t);
	if(list_actual_elements != list_header.count) {
		errx(2, "Actual elements count is not matchign");
	}

	// ---- validate data file
	int data_fd = open(argv[2], O_RDONLY);

	if(data_fd == -1) {
		err(3, "err open list");
	}

	struct header data_header;
	if(read(data_fd, &data_header, sizeof(struct header)) == -1) {
		err(3, "error reading  data file");
	}

	if(data_header.magic != 0x5a4d){
		errx(3, "sorry, cannot unlock the file data with your magic");
	}
	
	// ---- validate out file
	int out_fd = open(argv[3], O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);

	if(out_fd == -1) {
		err(4, "err open list");
	}

	struct header out_header;
	if(read(out_fd, &out_header, sizeof(struct header)) == -1) {
		err(4, "error reading  out file");
	}

	if(out_header.magic != 0x5a4d){
		errx(4, "sorry, cannot unlock the file out with your magic");
	}
	

	if (data_header.count != list_header.count) {
		errx(8, "Emelements count missmatch: data file and list file");
	}

	// ---- action ----
	int offset_list=0;
	uint16_t list_el;
	int read_result;

	while ((read_result = read(list_fd, &list_el, sizeof(uint16_t)) )) {

		if (read_result == -1) {
			err(5, "Error while reading list file");
		}
		offset_list++;

		int offset_data = offset_list*sizeof(uint32_t) + sizeof(struct header) - sizeof(uint32_t);
		printf("offset data is %d\n", offset_data);
		uint32_t data_el;
		lseek(data_fd, offset_data, SEEK_SET);

		if( read(data_fd, &data_el, sizeof(uint32_t)) != sizeof(uint32_t)) {
			err(6, "Wrong read size of data");
		}

		uint64_t el_to_write = (uint64_t) data_el;

		int offset_out = list_el * sizeof(uint64_t) + sizeof(struct header) - sizeof(uint64_t);
		lseek(out_fd, offset_out, SEEK_SET);
		if( write(out_fd, &el_to_write, sizeof(uint64_t)) != sizeof(uint64_t)) {
			errx(7, "Wrong write size in out file");
		}
	}

	
	close(list_fd);
	close(data_fd);
	close(out_fd);

	return 0;

}
