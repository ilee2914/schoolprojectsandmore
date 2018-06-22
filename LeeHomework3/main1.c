#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>


int main (int argc, char * argv[]) {
	if (argc != 2) {
		printf("Please input a file name");
		return 0;
	}
	
	uid_t user = 123;
	gid_t group = 123;

	if (access(argv[1], R_OK) == -1) {
		printf("File does not exist...Creating file.\n");
		open (argv[1], O_CREAT);
	}
	chown (argv[1], user, group);
	printf("File user changed to %i in group %i.", user, group);

	return 0;
}