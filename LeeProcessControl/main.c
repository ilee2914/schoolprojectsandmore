#include <unistd.h> /* for read(), write() */
#include <fcntl.h> /* for open() */
#include <string.h> /* for strlen() */
#include <stdio.h> /* for printf() */
#include <stdlib.h> /* for calloc() */
#include <stdbool.h> /* for true */
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <setjmp.h>
#include <sys/resource.h>
#include <sys/wait.h>

#define BUFFSIZE 4096
#define FILESIZE 1474560

jmp_buf j;

void Input(int);
void PrintStatus(int status);
void sigHandler (int signal)
{
	switch(signal) {
		case SIGABRT:
			printf("Termination signal : SIGABRT\n");
			printf("Process abort signal.\n");
			break;
		case SIGFPE:
			printf("Termination signal : SIGFPE\n");
			printf("Erroneous arithmetic operation.\n");
			longjmp(j, signal);
			break;
		case SIGILL:
			printf("Termination signal : SIGILL\n");
			printf("Illegal instruction.\n");
			break;
		case SIGINT:
			printf("Termination signal : SIGINT\n");
			printf("Terminal interrupt signal.\n");
			break;
		case SIGSEGV:
			printf("Termination signal : SIGSEGV\n");
			printf("Invalid memory reference.\n");
			break;
		case SIGTERM:
			printf("Termination signal : SIGTERM\n");
			printf("Termination signal.\n");
			break;
	}
}

void PrintUsage(struct rusage usage) {
	printf("--------------------------\nRusage Data\n--------------------------\n");
	printf("User CPU time used : %d\n", usage.ru_utime.tv_sec);
	printf("System CPU time used : %d\n", usage.ru_stime.tv_sec);
	printf("Maximum resident set size : %ld\n", usage.ru_maxrss);
	printf("Integral shared memory size : %ld\n", usage.ru_ixrss);
	printf("Integral unshared data size : %ld\n", usage.ru_idrss);
	printf("Integral unshared stack size : %ld\n", usage.ru_isrss);
	printf("Page reclaims (soft page faults) : %ld\n", usage.ru_minflt);
	printf("Page faults (hard page faults) : %ld\n", usage.ru_majflt);
	printf("Swaps : %ld\n", usage.ru_nswap);
	printf("Block input operations : %ld\n", usage.ru_inblock);
	printf("Block output operations : %ld\n", usage.ru_oublock);
	printf("IPC messages sent : %ld\n", usage.ru_msgsnd);
	printf("IPC messages received : %ld\n", usage.ru_msgrcv);
	printf("Signals received : %ld\n", usage.ru_nsignals);
	printf("Voluntary context switches : %ld\n", usage.ru_nvcsw);
	printf("Involuntary context switches : %ld\n", usage.ru_nivcsw);
	printf("--------------------------\n\n");
}

int main(int argc, char *argv[])
{
	int fd=open("file5.txt", O_CREAT|O_RDWR|O_APPEND, S_IRUSR| S_IWUSR);
	int ret, status;
	
	signal(SIGABRT, sigHandler);
	signal(SIGFPE, sigHandler);
	signal(SIGILL, sigHandler);
	signal(SIGINT, sigHandler);
	signal(SIGSEGV, sigHandler);
	signal(SIGTERM, sigHandler);
	
	for (int n=0; n<3; n++)
	{
		if ((ret=fork())==0) /* child process */
		{
			Input(fd);
			printf("Child process, ret=%d.\n", ret);
			status=wait(&status);
			switch(n)
			{
			case 0:
				exit(7);
				break;
			case 1:
				abort();
				break;
			case 2:
				if (setjmp(j)) {
					printf("Returned to safe spot. Exiting process..\n");
					exit(0);
				} else {
					status=status/0;
				}
				break;
			}
		}
		else
		{
			printf("Parent process, child ret==%d.\n", ret);
			struct rusage usage;
			wait3(&status, 0, &usage);
			PrintStatus(status);
			PrintUsage(usage);
			
		}
	} /* end for */
	return 0;
}
void Input(int filedes)
{
	char **name=calloc(3, 20*sizeof(char));
	char **it;
	name[0]="Adriana Wise\n";
	name[1]="Richard Stevens\n";
	name[2]="Evi Nemeth\n";
	for (it=&name[0]; *it!=NULL; it++)
	{
		ssize_t num_bytes=write(filedes, *it, strlen(*it));
		sleep(2);
	}
}
void PrintStatus(int status)
{	
	int lower_8_bits=WIFEXITED(status);
	if (lower_8_bits==true)
	{
		printf("WIFEXITED(status)=%d\n", WIFEXITED(status));
		printf("Exit status for child=%d\n",
		WEXITSTATUS(status));
		printf("lower_8_bits=%x\n", status);
	}
	else if (WIFSIGNALED(status)==true)
	{
		printf("WIFSIGNALED(status)=%d\n",
		WIFSIGNALED(status));
		printf("Exit status for child=%d\n",
		WTERMSIG(status));
		printf("Exit status for child=%d\n",
		WCOREDUMP(status));
	}
	else if (WIFSTOPPED(status)==true)
	{
		printf("WIFSTOPPED(status)=%d\n", WIFSTOPPED(status));
		printf("Exit status for child=%d\n",
		WSTOPSIG(status));
	}
}