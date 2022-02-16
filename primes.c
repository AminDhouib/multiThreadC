#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h> 
#include <math.h>


/* Function for checking if number is a prime number
	Input: Integer (Number)
	Output: Boolean (True if prime; false if not)
 */
int isPrime(int num) {
  
	// 1 is not a prime number
	if (num == 1){
		return 0;	
	}

	// 2 is a prime number
	if (num == 2){
		return 1;	
	}

    // Iterate from 2 to (n/2)+1 
    for (int i = 2; i <= (num/2)+1; i++) { 
  
        // If n is divisible by any number between 
        // 2 and n/2, it is not prime 
        if (num % i == 0) { 
            return 0;
        } 
    } 
  
	return 1;
}

/* Thread that will print all prime numbers below and equal the entered number */
void* primes(void* arg) {

	// Casting argument to a number
    int num = *((int*)arg);

	printf("Prime numbers smaller or equal to %d:", num);
	for(int i = 1; i <= num; i++) {
	    if(isPrime(i)) {
	        printf(" %d", i);
	    }
	}

    printf("\n");

    return NULL;
}

/* Main function */
int main(int argc, char* argv[]) {

    // Throws error if number of arguments is not 2
    if(argc != 2) {
        fprintf(stderr, "Primes \n Command syntax: primes <num>\n <num>: Number in which all the prime numbers less than or equal to that number.\n");
        return 1;
    }

    int num = atoi(argv[1]);

	// Performing some validations
    if(num < 1) {
        fprintf(stderr, "Provide a number that is bigger than 1.\n");
        return 1;
    }

	if(num == 1) {
        fprintf(stderr, "1 is not a prime number.\n");
        return 1;
    }

    // Create thread
    pthread_t thread;
    int result = pthread_create(&thread, NULL, primes, &num);
    if(result != 0) {
        fprintf(stderr, "Error creating thread.\n");
        exit(EXIT_FAILURE);
    }

    // Join thread
    result = pthread_join(thread, NULL);
    if(result != 0) {
        fprintf(stderr, "Error joining thread.\n");
        exit(EXIT_FAILURE);
    }

    return 0;
}
