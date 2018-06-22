#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>

int main() {	
	char *filename = "time.txt";
	if (access(filename, R_OK) == -1) {
		printf("time.txt does not exist");
		return 0;
	}
	
	pid_t pid = 0;
	pid = fork();
	if (pid == 0) {
		int fd = open(filename, O_RDWR);
		lseek(fd, 0, SEEK_END);
		time_t prevTime = 0;
		time_t currSec = 0;
		struct tm* timeInfo;
		char buffer[100];
		while (1==1) {
			currSec = time(NULL);
			if (currSec - prevTime >= 60) {		//Hour has passed or new program started
				prevTime = currSec;
				timeInfo = localtime(&currSec);
				strftime(buffer, 26, "%Y-%m-%d %H:%M:%S", timeInfo);
				write(fd, buffer, strlen(buffer));
				write(fd, "\n", 1);
			}	
			sleep(60);
		}
	}
	return 0;
}