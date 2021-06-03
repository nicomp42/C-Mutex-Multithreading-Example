/*
* johnAndMary.c
* Bill Nicholson
* nicholdw@ucmail.uc.edu
*/
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>


pthread_mutex_t myMutex = PTHREAD_MUTEX_INITIALIZER;
int balance = 0;
int tmp;
int loopCounter = 0;
void *myThreadDeposit(void *vargp) {
	int deposit;
	deposit = *((int *)vargp);
	//printf("\nDepositing %d", deposit);
	//int tmp;
	//pthread_mutex_lock(&myMutex);
	// critical section
	//puts("critical section");
	//sleep(5);
	tmp = balance;
	if (loopCounter % 2 == 1) {usleep(500000);}
	tmp += deposit;
	if (loopCounter % 2 == 0) {sleep(1);}
	balance = tmp;
	//pthread_mutex_unlock(&myMutex);

	return NULL;
}

int main(int argc, char **argv) {
	pthread_t john, mary;
	int i;
	// 10 tests causes about 5 race condition failures. 
	int timesToTest = 10;
	int errorCount = 0;
	for (i = 0; i < timesToTest; i++) {
		loopCounter++;
		balance = 0;
		int johnDeposit;
		johnDeposit = 100;
		pthread_create(&john, NULL, myThreadDeposit, &johnDeposit);
		int maryDeposit;
		maryDeposit = 500;
		pthread_create(&mary, NULL, myThreadDeposit, &maryDeposit);

		pthread_join(john, NULL);
		pthread_join(mary, NULL);
		if (balance != 600) {
			errorCount++;
			printf("\n balance = %d", balance);
		}
	}
	printf("\n error count = %d out of %d tests.\n", errorCount, timesToTest);
}
