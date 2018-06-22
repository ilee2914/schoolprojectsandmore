#include <pthread.h>
#include <stdio.h> /* for printf() */
#include <unistd.h> /* for read(), write(), sleep() */
#include <stdlib.h> /* for calloc() */
#include <time.h>

#define S 2100	//

int A[S][S], B[S][S], C[S][S];
int T;

void fillArray(int toFill[S][S]) {
	for (int i = 0; i < S; i++) {
		for (int j = 0; j < S; j++) {
			toFill[i][j] = rand()%9 + 1;
		}
	}
}

void printArray(int toPrint[S][S]) {
	for (int i = 0; i < S; i++) {
		for (int j = 0; j < S; j++) {
			printf("%4d", toPrint[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}

int findThreadCount() {
	if (S%7 == 0) return 7;
	else if (S%6 == 0) return 6;
	else if (S%5 == 0) return 5;
	else if (S%4 == 0) return 4;
	else if (S%3 == 0) return 3;
	else if (S%2 == 0) return 2;
	else return 7;
}

// The function the threads will run...
void *multiply(void* sec) {
	int sect = (int)sec;
	int start = sect * (S/T);	//Gets starting row
	int end = (sect+1) * (S/T);	//Gets ending row
	
	for (int i = start; i < end; i++) {
		for (int j = 0; j < S; j++) {
			int total = 0;
			for (int k = 0; k < S; k++) {
				total += A[i][k] * B[k][j];
			}
			C[i][j] = total;
		}
	}
	return 0;
	
}

int main() {
	srand(time(NULL));
	clock_t start= clock();
	T = findThreadCount(); // Finds a decent number of threads to use
	printf("Using %d threads...\n", T);
	pthread_t* threads = (pthread_t*)malloc(T*sizeof(pthread_t));
	fillArray(A);
	fillArray(B);
	for (int i = 0; i < T; i++) {
		if (pthread_create(&threads[i], NULL, multiply, (void*)i)) {
			printf("Error");
			free(threads);
			exit(0);
		}
	}
	for (int i = 0; i < T; i++)
		pthread_join(threads[i], NULL);
	
	double endTime = clock();
	double totalTime = ((double)(clock() - start))/CLOCKS_PER_SEC;
	
	free(threads);
	
	
	printf("Time took: %f\n", totalTime);
	printf("start time: %f\n", start);
	printf("end time: %f\n", endTime);
	
	
}