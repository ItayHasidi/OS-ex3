#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

static long sum;
static long primeCounter;
static int numberOfThreads;
pthread_mutex_t lock;
static int t_length;
static int t_length_end;

int isPrime(int num){
	int sqr = (int) sqrt (num);
	int i;
	if(num == 2 || num == 3){
		return 1;
	}
	if(num % 2 == 0 || num % 3 == 0 || num == 1 || num == 0){
		return 0;
	}
	for (i = 5; i <= sqr; i += 6){
		if (num % i == 0 || num % (i + 2) == 0){
			return 0;
		}
	}
	return 1;
}



void* main_help(void* id){
	int random, length = t_length;
	if(((unsigned int) id)+1 == numberOfThreads){
		length = t_length_end;
	}
	for (int i=0;i<length;i++){
		random = rand();
		if (isPrime(random))
		{
    		pthread_mutex_lock(&lock);
			sum = sum + random;
			primeCounter++;
	    	pthread_mutex_unlock(&lock);
		}	
	}
}

int main(int argc, char *argv[])
{
	if(argc != 3) {
		printf("Too few arguments ");
		printf("USAGE: ./primeCalc <prime pivot> <num of random numbers>");
		exit(0);
	}
	int randomPivot = atoi(argv[1]);
	int numOfRandomNumbers = atoi(argv[2]);	
	srand(randomPivot);
	sum = 0;
	primeCounter = 0;
	numberOfThreads = get_nprocs();
	t_length = numOfRandomNumbers / numberOfThreads;
	t_length_end = t_length + numOfRandomNumbers % numberOfThreads; //numOfRandomNumbers - ((numberOfThreads-1) * t_length);
	pthread_t threads[numberOfThreads];
	int rc;
	for(unsigned int i = 0; i < numberOfThreads; i++){
		rc = pthread_create(&threads[i], NULL, main_help, (void*)& i);
		if (rc){
			printf("ERROR; return code from pthread_create() is %d\n", rc);
			exit(-1);
		}
	}
	for(unsigned int i = 0; i < numberOfThreads; i++){
    	pthread_join(threads[i], NULL);
	}
	pthread_mutex_destroy(&lock);
	printf("%ld,%ld\n",sum,primeCounter);
	// pthread_exit(NULL);
    exit(0);
}