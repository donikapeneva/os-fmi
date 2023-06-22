#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<stdint.h>



int main(int argc, char * argv[]) {
	
	
	int fd = open("./number", O_CREAT | O_TRUNC | O_RDWR, S_IRWXU) ;
	//short c = 0xB6;
	uint8_t sth = 0xB6;
	write(fd, &sth, 1);
	//arr[8] = [0, 
	//1011 0110
	//0000 0010 <<
	//---------
	//000000000
		

	char comparator = 0x01;
	for( int i = 0; i < 8; i++) {
		comparator = comparator << 1;
		printf(">>>> %d %d = %d", sth, comparator, ((sth & comparator) !=0)); 
	}	

	exit(0);
}
