#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

struct fib_Params {
    int numInput;
    int* numArrayResult;
};

void* fibonacci(void* arg) {
    struct fib_Params* fibParams = (struct fib_Params*)arg;

    int number = fibParams->numInput;
    int* arrayInt = fibParams->numArrayResult;

    // Base step
	arrayInt[0] = 0;
	arrayInt[1] = 1;

    // Inductive step
    for(int i = 2; i < number; i++) {
        arrayInt[i] = arrayInt[i-1] + arrayInt[i-2];
    }

    return NULL;
}

int main(int argc, char* argv[]) {
    
    // Throws error if number of arguments is not 2
    if(argc != 2) {
        fprintf(stderr, "Fibonacci \n Command syntax: fibonacci <num>\n <num>: Number of Fibonacci numbers that the program is to generate.\n");
        return 1;
    }

    int numInput = atoi(argv[1]);

    // Performing some validations
    if(numInput < 1) {
        fprintf(stderr, "Please enter an integer equal or greater to 1 as argument.\n");
        return 1;
    }

    // Set Fib Parameters
    struct fib_Params fibParams;
    fibParams.numInput = numInput;
    fibParams.numArrayResult = malloc(sizeof(int) * numInput); // Allocate memory to create array

    // Create thread
    pthread_t thread;
    int result = pthread_create(&thread, NULL, fibonacci, &fibParams);
    if(result != 0) {
        fprintf(stderr, "Error creating thread.\n");
    }

    // Join thread
    result = pthread_join(thread, NULL);
    if(result != 0) {
        fprintf(stderr, "Error joining thread.\n");
    }

    printf("Sequence Generated:");
    for(int i = 0; i < fibParams.numInput; i++) {
        printf(" %d", fibParams.numArrayResult[i]);
    }
    printf("\n");

	// Deallocate memory from the previous malloc call
    free(fibParams.numArrayResult);

    return 0;
}
