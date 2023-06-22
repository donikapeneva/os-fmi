#include <stdint.h>
#include <err.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#define LESS 0
#define MORE 1

struct comp_header {
	uint32_t magic1;
	uint16_t magic2;
	uint16_t reserved;
	uint64_t count;
};

struct comp_data {
	uint16_t type;
	uint16_t reserved[3];
	uint32_t offset1;
	uint32_t offset2;
};

struct data_header {
	uint32_t magic;
	uint32_t count;
};

void replace(int fd, uint32_t off1, uint32_t off2);
void replace(int fd, uint32_t off1, uint32_t off2, uint64_t e1, uint64_t e2) {
	if(lseek(fd, off1, SEEK_SET) == -1) {
		err(17, "could not lseek");
	}
	if(write(fd, &e2, sizeof(e2)) == -1) {
		err(18, "could not write");
	}
	if(lseek(fd, off2, SEEK_SET) == -1) {
		err(19, "could not lseek");
	}
	if(write(fd, &e1, sizeof(e1)) == -1) {
		err(20, "could not write");
	}
}

int main(int argc, char** argv) {

	if(argc != 3) {
		errx(1, "arg count");
	}

	int data_fd = open(argv[1], O_RDWR);
	if(data_fd == -1) {
		err(2, "could not open data.bin");
	}

	struct data_header buff;
	if(read(data_fd, &buff, sizeof(buff)) == -1) {
		err(3, "could not read header");	
	}
	
	if(buff.magic != 0x4A6F7921) {
		errx(4, "wrong magic");
	}

	int comp_fd = open(argv[2], O_RDONLY);
	if(comp_fd == -1) {
		err(5, "could not open comparator");
	}

	struct stat s;
	if(fstat(data_fd, &s) == -1) {
		err(10, "could not stat");
	}

	if((s.st_size - sizeof(struct data_header) / sizeof(uint64_t)) != buff.count) {
		errx(11, "count does not match reality");
	}

	struct comp_header comp_buff;
	if(read(comp_fd, &comp_buff, sizeof(comp_buff)) == -1) {
		err(6, "could not read comp header");	
	}
	//0xAFBC7A37
	if(comp_buff.magic1 != 0x377ABCAF) {
		errx(4, "wrong magic");
	}

	if(comp_buff.magic2 != 0x271C) {
		errx(4, "wrong magic");
	}

	struct stat s1;
	if(fstat(comp_fd, &s1) == -1) {
		err(10, "could not stat");
	}

	if((s1.st_size - sizeof(struct comp_header) / sizeof(struct comp_data)) != comp_header.count) {
		errx(12, "count does not match reality");
	}

	struct comp_data buff1;
	int read_result;
	while((read_result = read(comp_fd, &buff1, sizeof(buff1)) ) ) {

		if(read_result == -1) {
			err(20, "could not read");
		}
		if(read_result != sizeof(buff1)) {
			errx(21, "read less that expected");
		}

		uint64_t e1;	
		uint64_t e2;	

		if(buff1.offset1 > buff.count || buff1.offset2 > buff.count) {
			errx(15, "offset out of file");
		}

		// Napravi proverki
		lseek(data_fd, buff1.offset1*sizeof(uint64_t)+sizeof(data_header), SEEK_SET);
		read(data_fd, &e1, sizeof(e1));
		lseek(data_fd, buff1.offset2*sizeof(uint64_t)+sizeof(data_header), SEEK_SET);

		read(data_fd, &e2, sizeof(e2));

		if(buff1.type == LESS && e1 > e2) {
			replace(data_fd, buff1.offset1, buff1.offset2, e1, e2);		
		} else if(buff1.type == MORE && e1 < e2) {
			replace(data_fd, buff1.offset1, buff1.offset2, e1, e2);		
		} else if(buff1.type != MORE || buff1.type != LESS) {
			errx(16, "unknown comparator type");
		}
	}

	close(data_fd);
	close(comp_fd);
			
	return 0;
}

