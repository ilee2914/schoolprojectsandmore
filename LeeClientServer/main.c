#define _POSIX_SOURCE
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#undef _POSIX_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#define CLIENT_THREADS 4
#define READ_REQUEST 1
#define WRITE_REQUEST 2
#define QUIT_REQUEST 3

/* a per-request struct, one per client thread */

typedef struct request_tag {
	struct request_tag *next; /* pointer to next request */
	int operation; /* one of read/write/quit request */
	int synchronous; /* whether request is synchronous */
	int done_flag; /* predicate this request done */
	pthread_cond_t done;/* wait for this thread to finish */
	char prompt[32]; /* prompt server message to client */
	char text[28]; /* read/write text */
} request_t;

/* a server struct, organized as a client request queue */

typedef struct server_tag {
	request_t *first; /* first request in the queue */
	request_t *last; /* last request in the queue */
	int running; /* predicate this server running */
	pthread_mutex_t mutex; /* lock around server data */
	pthread_cond_t request; /* wait for a request */
} server_t;

server_t server={NULL, NULL, 0, PTHREAD_MUTEX_INITIALIZER,PTHREAD_COND_INITIALIZER};

int client_threads; /* client has 4 threads */
pthread_mutex_t client_mutex=PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t clients_done=PTHREAD_COND_INITIALIZER;

void *server_routine(void *arg);
void server_request(int operation, int sync, const char *prompt, char *string);
void *client_routine(void *arg);

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

void search(char name[], int fd) {
	ssize_t num_bytes=lseek(fd, 0, SEEK_END);
	lseek(fd, 0, SEEK_SET);
	char buf[1000];
	char * toks;
	read(fd, buf, num_bytes);
	toks = strtok(buf, " \n");
	while (toks != NULL) {
		if (strcmp(name, toks) == 0) {
			printf("Token exists in DB!\n");
		}
		toks = strtok(NULL, " \n");
	}
	printf("Token does not exist in DB!\n");
}

int main(int argc, char **argv) {
	pthread_t thread;
	int count;
	int err;
	client_threads=CLIENT_THREADS; /* #defined to 4 */
	for (count=0; count<client_threads; count++) {
		printf("Creating client #%d...\n", count);
		err=pthread_create(&thread, NULL, client_routine,
		(void *)count);
	}

	err=pthread_mutex_lock(&client_mutex);
	while (client_threads>0)
		err=pthread_cond_wait(&clients_done, &client_mutex);
	err=pthread_mutex_unlock(&client_mutex);
	printf("All clients done.\n");
	server_request(QUIT_REQUEST, 1, NULL, NULL);
	return 0;
}

void server_request(int operation, int sync, const char *prompt, char *string) {
	request_t *request;
	int err;

	/* lock the server data */
	err=pthread_mutex_lock(&server.mutex);

	/* create the server thread */
	if (!server.running) {
		pthread_t thread;
		/* server thread is created as a detached thread */
		pthread_attr_t detached_attr;
		err=pthread_attr_init(&detached_attr);
		err=pthread_attr_setdetachstate(&detached_attr, PTHREAD_CREATE_DETACHED);
		server.running=1;
		err=pthread_create(&thread, &detached_attr, server_routine, NULL);
		pthread_attr_destroy(&detached_attr);
	}

	/* create the request */
	request=(request_t *)malloc(sizeof(request_t));
	request->next=NULL; /* this is tail */
	/* these are going to be passed-in arguments */
	request->operation=operation;
	request->synchronous=sync;

	if (sync) {
		/* set the done predicate to 0 */
		request->done_flag=0;
		/* initialize the pthread_cond_t variable "done" */
		err=pthread_cond_init(&request->done, NULL);
	}

	/* all requests get a prompt */
	if (prompt!=NULL)
		strncpy(request->prompt, prompt, 32);
	else
		request->prompt[0]='\0';

	/* only the write requests get to write */
	if (operation==WRITE_REQUEST && string!=NULL)
		strncpy(request->text, string, 128);
	else
		request->text[0]='\0';
	/* enqueue the request */

	if (server.first==NULL) {/* if queue is empty */
		server.first=request; /* head=tail=request */
		server.last=request;
	} else {/* if queue is non-empty */
		/* make next of current tail point to request */
		(server.last)->next=request;
		/* make tail equal to request */
		server.last=request;
	}

	/* tell server that there is a request made */
	err=pthread_cond_signal(&server.request);
	/* if request was synchronous, wait for a reply */
	if (sync) {
		while (!request->done_flag)
			err=pthread_cond_wait(&request->done, &server.mutex);
		if (operation==READ_REQUEST)
		strcpy(string, request->text);
		err=pthread_cond_destroy(&request->done);
	}
	err=pthread_mutex_unlock(&server.mutex);
}

void *client_routine(void *arg) {
	printf("In client_routine()...\n");
	int client_number=(int )arg;
	char prompt[32];
	char string[128], formatted[128];
	int err;
	sprintf(prompt, "Client %d>", client_number);

	//puts(prompt);
	while (1){
		server_request(READ_REQUEST, 1, prompt, string);
		if (strlen(string)==0)
			break;
		sprintf(formatted, "(%d) %s", client_number, string);
		server_request(WRITE_REQUEST, 0, NULL, formatted);
	}

	err=pthread_mutex_lock(&client_mutex);
	client_threads--;

	if (client_threads<=0)
		err=pthread_cond_signal(&clients_done);
	err=pthread_mutex_unlock(&client_mutex);
	printf("Exiting client_routine()...\n");
	return NULL;
}

void *server_routine(void *arg) {
	char* filename = "books.txt";
	int fd = open(filename, O_RDWR);

	printf("In server_routine()...\n");
	static pthread_mutex_t prompt_mutex=PTHREAD_MUTEX_INITIALIZER;
	request_t *request;
	int operation;
	int err;
	
	while (1) {
		/* lock the server mutex, to protect server-modifying
		data */
		err=pthread_mutex_lock(&server.mutex);
		while (server.first==NULL) {
			/* wait for request */
			err=pthread_cond_wait(&server.request, &server.mutex);
		}
		/* dequeue 1st request in the queue */
		request=server.first;
		/* advance the head of the queue */
		server.first=request->next;
		if (server.first==NULL)
		/* if queue is empty, reset tail to NULL */
			server.last=NULL;
		err=pthread_mutex_unlock(&server.mutex);
		/* read what the requested op was: read/write/quit */
		operation=request->operation;
		char rN[2];
		int rowNumber = 0;
		char name[20];
		switch (operation) {
			case QUIT_REQUEST:
				break;
			case READ_REQUEST:
				if (strlen(request->prompt)>0)
					printf("--->%s", request->prompt);
				/* strip the newline character */
				if (fgets(request->text, 128, stdin)==0)
					request->text[0]='\0';
				request->text[strlen(request->text)-1]='\0';
				if (strcmp("DB_SEARCH", request->text) == 0) {
					printf("What do you want to search for?");
					scanf("%s", name);
					search(name, fd);
				} else if (strcmp("DB_INSERT", request->text) == 0) {
					printf("What row number would you like to modify? ");
					scanf("%s", rN);
					rowNumber = atoi(rN);
					
					replaceAtRow(rowNumber, fd);	
				} else if (strcmp("DB_REPLACE", request->text) == 0) {
					printf("What row number would you like to modify? ");
					scanf("%s", rN);
					rowNumber = atoi(rN);
					
					replaceAtRow(rowNumber, fd);	
				}
				break;
			case WRITE_REQUEST:
				
				break;
			default:
				break;
		}
		/* 0=non-synchronous, 1=synchronous */
		if (request->synchronous==1) {
			err=pthread_mutex_lock(&server.mutex);
			/* set the done flag to 1, will be the predicate to
			client to synchronize with other */
			request->done_flag=1;
			/* signal that the request is done processing */
			pthread_cond_signal(&request->done);
			err=pthread_mutex_unlock(&server.mutex);
		}
		if (operation==QUIT_REQUEST)
			break; /* break from the while(1) loop */
	}
	return NULL;
}