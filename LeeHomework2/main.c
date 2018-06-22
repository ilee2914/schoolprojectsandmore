#define _POSIX_SOURCE
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#undef _POSIX_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int findEndOfRowByte(int rowStartByte, int num_bytes, char buf[]) {
	for (int i = rowStartByte; i < num_bytes; i++) {
		if (buf[i] == '\n') {
			return i;
		}
	}
	return -1;
}

int findSpaceByte(int rowStartByte, int num_bytes, char buf[]) {
	for (int i = rowStartByte; i < num_bytes; i++) {
		if (buf[i] == ' ') {
			return i;
		}
	}
	return -1;
}

void replaceElement(int startByte, int spaceByte, int endByte, int num_bytes, int fd, char buf[]) {
	char ans[1];
	char newTitle[20];
	char newAuthor[20];
	char newBuf[200];
	
	printf("Would you like to edit the title, author, or both(insert new line)? (T | A | B) ");
	scanf("%s", ans);
	
	if (strcmp("B", ans) == 0 || strcmp("T", ans) == 0) {
		printf("What is the new title? ");
		scanf("%s", newTitle);
		strcat(newTitle, " ");
	}
	if (strcmp("B", ans) == 0 || strcmp("A", ans) == 0) {
		printf("Who is the new author? ");
		scanf("%s", newAuthor);
		strcat(newAuthor, "\n");
	}
	
	if (strcmp("B", ans) == 0) {				// Handles Insertion
		strcpy(newBuf, &buf[startByte]);
		lseek(fd, startByte, SEEK_SET);
		write(fd, newTitle, strlen(newTitle));
		write(fd, newAuthor, strlen(newAuthor));
		write(fd, newBuf, strlen(newBuf));
		printf("Inserted new line!\n");
	} else if (strcmp("T", ans) == 0) {			// Handles replacing title
		strcpy(newBuf, &buf[spaceByte+1]);
		lseek(fd, startByte, SEEK_SET);
		ftruncate(fd, startByte);
		write(fd, newTitle, strlen(newTitle));
		write(fd, newBuf, strlen(newBuf));
		printf("Replaced title!\n");
	} else if (strcmp("A", ans) == 0) {			// Handles replacing authors
		if (endByte > -1)
			strcpy(newBuf, &buf[endByte + 1]);
		lseek(fd, spaceByte + 1, SEEK_SET);
		ftruncate(fd, spaceByte + 1);
		write(fd, newAuthor, strlen(newAuthor));
		if (endByte > -1) 
			write(fd, newBuf, strlen(newBuf));
		printf("Replaced author!\n");
	}
}

//Does the whole shibang
int replaceAtRow(int rowToFind, int fd) {
	char buf[200];
	int currRow = 0;
	int i;
	ssize_t num_bytes=lseek(fd, 0, SEEK_END);
	lseek(fd, 0, SEEK_SET);
	read(fd, buf, num_bytes);
	for (i = 0; i < num_bytes; i++) {
		if (currRow >= rowToFind) {			// Found byte where row starts
			int spaceByte = findSpaceByte(i, num_bytes, buf);
			int endByte = findEndOfRowByte(spaceByte, num_bytes, buf);
			replaceElement(i, spaceByte, endByte, num_bytes, fd, buf);
			break;
		}
		if (buf[i] == '\n')
			currRow++;
	}
	return -1;
}

int main() {
	char* filename = "books.txt";
	int fd = open(filename, O_RDWR);
	char rN[2];
	int rowNumber = 0;
	
	printf("What row number would you like to modify? ");
	scanf("%s", rN);
	rowNumber = atoi(rN);
	
	replaceAtRow(rowNumber, fd);	
}