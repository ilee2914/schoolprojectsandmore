#include <stdio.h> /* for printf() */
#include <signal.h> /* for signal() */
#include <string.h> /* for strlen() */
#include <stdlib.h> /* for calloc() */
#include <unistd.h> /* for sleep() */
#include <fcntl.h> /* for open() */

sigset_t *oldset, *newset, *waitset;

int fd;

void handler(int signo);
void PrintSigset(sigset_t *set);
void CriticalCode(void);

int main(int argc, char **argv) {
	fd=open("file.txt", O_CREAT|O_RDWR, S_IRUSR|S_IWUSR);
	newset=(sigset_t *)malloc(sizeof(sigset_t));
	oldset=(sigset_t *)malloc(sizeof(sigset_t));
	waitset=(sigset_t *)malloc(sizeof(sigset_t));
	
	sigemptyset(newset);
	sigemptyset(oldset);
	sigemptyset(waitset);
	
	sigaddset(oldset, SIGINT);
	sigaddset(newset, SIGABRT);
	sigaddset(newset, SIGALRM);
	
	signal(SIGINT, handler);
	signal(SIGABRT, handler);
	signal(SIGALRM, handler);
	signal(SIGHUP, handler);
	
	sigprocmask(SIG_SETMASK, newset, NULL);
	sigprocmask(SIG_BLOCK, oldset, newset);
	
	CriticalCode();
	/* this unblocks SIGINT */
	sigsuspend(newset);	
	printf("Finished. Check file.txt for results.");
	return 0;
}

void CriticalCode(void) {
	raise(SIGINT);
	sigpending(waitset);
	write(fd, "Wait Set: \n", 11);
	PrintSigset(waitset);
}

void PrintSigset(sigset_t *set) {
	write(fd, "-----------------------------\n", 30);
	char buf[100];
	for (int i=1; i<32; i++) {
		if (sigismember(set, i)) {
			sprintf(buf, "Signal %d: %s\n", i, strsignal(i));
			write(fd, buf, strlen(buf));
		}
	}
	write(fd, "-----------------------------\n", 30);
}

void handler(int signo) {
	char *string=(char *)malloc(20*sizeof(char));
	string="ACTION: Received SIGINT.\n";
	switch (signo) {
		case SIGINT:
			write(fd, "In handler()...\n", 16);
			break;
		case SIGALRM:
			write(fd, "In handler()...\n", 16);
			string="ACTION: Received SIGALRM.\n";
			break;
		case SIGHUP:
			write(fd, "In handler()...\n", 16);
			string="ACTION: Received SIGHUP.\n";
			break;
		case SIGABRT:
			write(fd, "In handler()...\n", 16);
			string="ACTION: Received SIGABRT.\n";
			break;
	}
	write(fd, string, strlen(string));
}