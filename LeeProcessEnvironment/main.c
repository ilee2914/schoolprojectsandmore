#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <setjmp.h>
#include <string.h>

void writeName(int fd) {
	printf("Please write my name: ");
	char name[10];
	scanf("%s", name);
	write(fd, name, strlen(name));
}

void correct(int fd, jmp_buf save) {
	char buf[10];
	lseek(fd, 0, SEEK_SET);
	read(fd, buf, sizeof(char));
	if (strcmp(buf, "Ira")==0) {
		printf("Correct! Here are some bananas!! )))");
		close(fd);
	} else {
		printf("Incorrect! Try again.\n\n");
		close(fd);
		remove("test.txt");
		longjmp(save, 1);
	}
}

int main() {
	char* filename = "test.txt";
	jmp_buf save;
	int val = setjmp(save);
	
	int fd = open(filename, O_RDWR | O_CREAT);
	writeName(fd);
	correct(fd, save);
	return 0;
}