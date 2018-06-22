#define _POSIX_SOURCE
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#undef _POSIX_SOURCE
#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[]) {
	if (argc < 3) {
		printf("Please check arguments!");
		return 0;
	}
	
	char name[10];					// first name
	char *fn = argv[1];				// first file
	char *fn2 = argv[2];			// second file
	int fd;
	int fd2;
	int n = 0;
	char buf[10];
	
	printf("Please enter your name : ");
	scanf("%s", name);
	
	fd = creat(fn, S_IRUSR | S_IWUSR);
	write(fd, name, strlen(name));
	printf("Output has been created.\n");
	close(fd);
	
	fd = open(fn, O_RDWR);
	fd2 = creat(fn2, S_IRUSR | S_IWUSR);
	while ((n=read(fd, buf, sizeof(char)))>0) {
		write(fd2, buf, n);
	}
	printf("Copy has been created");
	
	close(fd);
	close(fd2);
	return(fd);
}