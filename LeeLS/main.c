#include<stdio.h> 
#include<stdlib.h> 
#include <sys/types.h>
#include <sys/stat.h> 
#include <dirent.h> 
#include <string.h>
#include <pwd.h>
#include <grp.h>

void printPermissions(struct stat fStats) {
	printf((S_ISDIR(fStats.st_mode)) ? "d" : "-");
    printf((fStats.st_mode & S_IRUSR) ? "r" : "-");
    printf((fStats.st_mode & S_IWUSR) ? "w" : "-");
    printf((fStats.st_mode & S_IXUSR) ? "x" : "-");
    printf((fStats.st_mode & S_IRGRP) ? "r" : "-");
    printf((fStats.st_mode & S_IWGRP) ? "w" : "-");
    printf((fStats.st_mode & S_IXGRP) ? "x" : "-");
    printf((fStats.st_mode & S_IROTH) ? "r" : "-");
    printf((fStats.st_mode & S_IWOTH) ? "w" : "-");
    printf((fStats.st_mode & S_IXOTH) ? "x" : "-");
}

/*
 * Possible options
 * -a : Lists every file including hidden ones
 * -1 : Lists one file per line
 * -U : Lists unordered
 * -Q : enclose entry names in double quotes
 * -s : print size of each file, in blocks
 * -m : fill width with a comma separated list of entries
 * -n : like -l, but list numeric UIDs and GIDs
 * -l : use a long listing format
 * -g : like -l, but do not list owner
 * -o : like -l, but do not list group information
 * -i : print index number of each file
 */
int main(int argc, char *argv[]) {
	DIR *dp = NULL; 
	struct dirent *ptr = NULL; 
	struct stat fStats;
	dp = opendir((const char*)getenv("PWD")); 
	if (argc == 2) {
		char* option = argv[1];
		if (option[1] == 'a') {			//List all
			while(NULL != (ptr = readdir(dp))) { 
				printf("%s  ",ptr->d_name); 
			}
		} else if(option[1] == '1') {	//List one per line
			while(NULL != (ptr = readdir(dp))) {
				if (strcmp(ptr->d_name, ".") != 0 && strcmp(ptr->d_name, "..") != 0) {
					printf("%s\n",ptr->d_name);
				}
			}
		} else if(option[1] == 'U') {	//List in directory order
			while(NULL != (ptr = readdir(dp))) {
				if (strcmp(ptr->d_name, ".") != 0 && strcmp(ptr->d_name, "..") != 0) {
					printf("%s  ",ptr->d_name);
				}
			}
		} else if (option[1] == 'Q') {
			while(NULL != (ptr = readdir(dp))) {
				if (strcmp(ptr->d_name, ".") != 0 && strcmp(ptr->d_name, "..") != 0) {
					printf("\"%s\"  ",ptr->d_name);
				}
			}
		} else if (option[1] == 's') {
			while(NULL != (ptr = readdir(dp))) {
				if (strcmp(ptr->d_name, ".") != 0 && strcmp(ptr->d_name, "..") != 0) {
					stat(ptr->d_name, &fStats);
					printf("%d %s  ", fStats.st_blocks, ptr->d_name);
				}
			}
		} else if (option[1] == 'm') {
			while(NULL != (ptr = readdir(dp))) {
				if (strcmp(ptr->d_name, ".") != 0 && strcmp(ptr->d_name, "..") != 0) {
					printf("%s,  ", ptr->d_name);
				}
			}
		} else if (option[1] == 'n') {
			while(NULL != (ptr = readdir(dp))) {
				if (strcmp(ptr->d_name, ".") != 0 && strcmp(ptr->d_name, "..") != 0) {
					stat(ptr->d_name, &fStats);
					printPermissions(fStats);
					char buffer[20];
					strftime(buffer, sizeof(buffer), "%b %d %H:%M", localtime(&(fStats.st_mtime)));
					printf(" %d %d %d %5d %s %s\n", fStats.st_nlink, fStats.st_uid, fStats.st_gid, fStats.st_size, buffer, ptr->d_name);
				}
			}
		} else if (option[1] == 'l') {
			while(NULL != (ptr = readdir(dp))) {
				if (strcmp(ptr->d_name, ".") != 0 && strcmp(ptr->d_name, "..") != 0) {
					stat(ptr->d_name, &fStats);
					printPermissions(fStats);
					char buffer[20];
					struct passwd * temp_pw;
					temp_pw = getpwuid (fStats.st_uid);
					strftime(buffer, sizeof(buffer), "%b %d %H:%M", localtime(&(fStats.st_mtime)));
					printf(" %d %s %s %5d %s %s\n", fStats.st_nlink, temp_pw->pw_name, getgrgid(temp_pw->pw_gid)->gr_name, fStats.st_size, buffer, ptr->d_name);
				}
			}
		}else if (option[1] == 'g') {
			while(NULL != (ptr = readdir(dp))) {
				if (strcmp(ptr->d_name, ".") != 0 && strcmp(ptr->d_name, "..") != 0) {
					stat(ptr->d_name, &fStats);
					printPermissions(fStats);
					char buffer[20];
					struct passwd * temp_pw;
					temp_pw = getpwuid (fStats.st_uid);
					strftime(buffer, sizeof(buffer), "%b %d %H:%M", localtime(&(fStats.st_mtime)));
					printf(" %d %s %5d %s %s\n", fStats.st_nlink, getgrgid(temp_pw->pw_gid)->gr_name, fStats.st_size, buffer, ptr->d_name);
				}
			}
		} else if (option[1] == 'o') {
			while(NULL != (ptr = readdir(dp))) {
				if (strcmp(ptr->d_name, ".") != 0 && strcmp(ptr->d_name, "..") != 0) {
					stat(ptr->d_name, &fStats);
					printPermissions(fStats);
					char buffer[20];
					struct passwd * temp_pw;
					temp_pw = getpwuid (fStats.st_uid);
					strftime(buffer, sizeof(buffer), "%b %d %H:%M", localtime(&(fStats.st_mtime)));
					printf(" %d %s %5d %s %s\n", fStats.st_nlink, temp_pw->pw_name, fStats.st_size, buffer, ptr->d_name);
				}
			}
		} else if (option[1] == 'i') {
			while(NULL != (ptr = readdir(dp))) {
				if (strcmp(ptr->d_name, ".") != 0 && strcmp(ptr->d_name, "..") != 0) {
					stat(ptr->d_name, &fStats);
					printf("%lld  %s  ", ptr->d_ino, ptr->d_name);
				}
			}
		}
	} else {
		while(NULL != (ptr = readdir(dp))) {
			if (strcmp(ptr->d_name, ".") != 0 && strcmp(ptr->d_name, "..") != 0) {
				printf("%s  ",ptr->d_name);
			}
		}
	}
}