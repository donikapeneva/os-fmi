#include<fcntl.h>
#include<err.h>
#include<unistd.h>
#include<stdint.h>

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

	int list_fd = open(argv[1], O_CREAT | O_TRUNC | O_RDWR, S_IRUSR | S_IWUSR);
		
	int data_fd = open(argv[2],   O_CREAT | O_TRUNC | O_RDWR, S_IRUSR | S_IWUSR);
	int out_fd = open(argv[3], O_CREAT | O_TRUNC | O_RDWR, S_IRUSR | S_IWUSR);
	//todo: checks

	// --- list file --- 
	struct header list_header;
	list_header.magic = 0x5A4D;
	list_header.filetype = 1;
	list_header.count = 3;
	if(write(list_fd, &list_header, sizeof(struct header)) == -1) {
		errx(2, "error writing  listi file");
	}
	
	uint16_t n1[3] = { 1, 3, 1 };
	
	for (int i =0; i< 3; i++) {

		if(write(list_fd, &n1[i], sizeof(uint16_t)) == -1) {
			errx(2, "error writing data in listi file");
		}
	}

	// --- data file ---
	struct header data_header;
	data_header.magic = 0x5A4D;
	data_header.filetype = 2;
	data_header.count = 3;
	if(write(data_fd, &data_header, sizeof(struct header)) == -1) {
		errx(2, "error writing data file");
	}
	uint32_t n2[3] = { 1, 2, 3 };
	
	for (int i =0; i< 3; i++) {

		if(write(data_fd, &n2[i], sizeof(uint32_t)) == -1) {
			errx(2, "error writing data in data file");
		}
	}


// --- out file ---
struct header out_header;
	out_header.magic = 0x5A4D;
	out_header.filetype = 3;
	out_header.count = 3;
	if(write(out_fd, &out_header, sizeof(struct header)) == -1) {
		errx(2, "error writing out file");
	}




	
	close(list_fd);
	close(data_fd);

	close(out_fd);

	return 0;

}
