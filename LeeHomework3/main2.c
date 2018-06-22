#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

void convertToBinary(int val) {
	char * conversion[] = {"000", "001", "010", "011", "100", "101", "110", "111"};
	char value[20];
	char binary[100] = "";
	int size = sprintf(value, "%o", val);
	int i = 0;
	//Handles first input
	int temp = value[0] - '0';
	char * tempHold = conversion[temp];
	if (temp <= 3) {
		if (temp <= 1)
			strncpy(binary, tempHold+2, 3);
		else
			strncpy(binary, tempHold+1, 3);
		i++;
	}
	
	for (; i < size; i++) {
		int temp = value[i] - '0';
		strcat(binary, conversion[temp]);
	}
	
	printf("%s\n", binary);
}

void permissionFlags(int mode) {
	char userFlag[3] = "---";
	char groupFlag[3] = "---";
	char otherFlag[3] = "---";
	if ( mode & S_IRUSR ) userFlag[0] = 'r';   
	if ( mode & S_IWUSR ) userFlag[1] = 'w';
	if ( mode & S_IXUSR ) userFlag[2] = 'x';
	
	if ( mode & S_IRGRP ) groupFlag[0] = 'r';   
	if ( mode & S_IWGRP ) groupFlag[1] = 'w';
	if ( mode & S_IXGRP ) groupFlag[2] = 'x';
	
	if ( mode & S_IROTH ) otherFlag[0] = 'r';  
	if ( mode & S_IWOTH ) otherFlag[1] = 'w';
	if ( mode & S_IXOTH ) otherFlag[2] = 'x';
	printf("User permissions : \t\t%s\n", userFlag);
	printf("Group permissions : \t\t%s\n", groupFlag);
	printf("Other permissions : \t\t%s\n", otherFlag);
}

void printLine() {
	printf("\n\n----------------------------------------------------------\n\n");
}

int main (int argc, char * argv[]) {
	if (argc != 2) {
		printf("Please check arguments. Closing.");
		return 0;
	}

	struct stat buf;
	stat (argv[1], &buf);
	printf ("File name: \t%s", argv[1]);
	
	printLine();
	printf("st_mode: \t"); convertToBinary(buf.st_mode);
	printf("st_ino: \t"); convertToBinary(buf.st_ino);
	printf("st_dev: \t"); convertToBinary(buf.st_dev);
	printf("st_rdev: \t"); convertToBinary(buf.st_rdev);
	printf("st_nlink: \t"); convertToBinary(buf.st_nlink);
	printf("st_uid: \t"); convertToBinary(buf.st_uid);
	printf("st_gid: \t"); convertToBinary(buf.st_gid);
	printf("st_size: \t"); convertToBinary(buf.st_size);
	printf("st_atime: \t"); convertToBinary(buf.st_atime);
	printf("st_mtime: \t"); convertToBinary(buf.st_mtime);
	printf("st_ctime: \t"); convertToBinary(buf.st_ctime);
	printf("st_blksize: \t"); convertToBinary(buf.st_blksize);
	printf("st_blocks: \t"); convertToBinary(buf.st_blocks);
	
	printLine();
	printf("st_mode breakdown\n");
	printf("st_mode: \t\t"); convertToBinary(buf.st_mode);
	printf("Set User ID: \t\t"); convertToBinary((buf.st_mode & 0x8000) >> 15);		// 15th bit
	printf("Set Group ID: \t\t"); convertToBinary((buf.st_mode & 0x4000) >> 14);	// 14th bit
	printf("Sticky Bit: \t\t"); convertToBinary((buf.st_mode & 0x2000) >> 13);		// 13th bit
	printf("File Type: \t\t"); convertToBinary((buf.st_mode & 0x1E00) >> 9);		// 12th-9th bit
	printLine();
	
	printf("Access Permissions \n"); permissionFlags(buf.st_mode);		
	
	printLine();
	return 0;
}