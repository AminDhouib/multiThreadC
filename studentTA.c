/**
 * How to run: gcc studentTA.c -o studentTA -lpthread
 * This program runs infinitely, Press Contorl + C to end the program 
 * */

#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>

// Global variables Holding Int Values
#define NUM_OF_STUDENTS 5
#define NUM_OF_CHAIRS 3

// Global Variables Holding Semaphores and Mutex 
pthread_mutex_t mutex;
sem_t student_sem;
sem_t ta_sem;

// Number of students that are waiting in the chairs 
int numberOfStudentsWaiting = 0;

// Initialize all semaphores and the mutex thread
void initSemaphoresMutex() {
    pthread_mutex_init(&mutex, NULL);
    sem_init(&student_sem, 0, 0);
    sem_init(&ta_sem, 0, 0);
}

// Generate random time
int generateRandomTime() {
	int randomSeconds = (rand() % 5) + 1;
    return randomSeconds;
}

// A Pseudo-Queue That Is Used To Keep Track Of Which Student is with the TA
// NOTE: This does not affect the program's actual work. This is only used to print
// to the user which student is being handled by the TA.
int waitingRoomChairs[3];
int nextTeachingPosition = 0;
int nextSeatingPosition = 0;

// Student Thread
void* stu_thread(void* student) {
    int studentID = *(int*) student;

    while (1) {
		// Get time student will take to program and sleep
        int programmingTime = generateRandomTime();
        printf("Student %d will be programing for %d seconds.\n", studentID, programmingTime);
        sleep(programmingTime);
		
		// Lock Mutex
        pthread_mutex_lock(&mutex);

		// Seat is available
        if (numberOfStudentsWaiting < NUM_OF_CHAIRS) {
			
			// Student takes a seat and now there is an additional student waiting  
			numberOfStudentsWaiting++;
            printf("\tStudent %d takes a seat. Now # of waiting students = %d\n", studentID, numberOfStudentsWaiting);
			
			// Update Pseudo-Queue to keep track of which student the TA will handle
			waitingRoomChairs[nextSeatingPosition] = studentID;         
			nextSeatingPosition = ( nextSeatingPosition + 1 ) % NUM_OF_CHAIRS;

			// Student tells TA that he has arrived
            sem_post(&student_sem);
			
			// Unlock Mutex (Other threads have access to the seats)			
			pthread_mutex_unlock(&mutex);

			// Student is receiving help at this point 
            sem_wait(&ta_sem);

		// No Seat is available        
		} else {
            pthread_mutex_unlock(&mutex);
            printf("\t\t\tSeats are full. Student %d will retry later and will continue programming\n", studentID);
        }
    }
}

// TA Thread
void *ta_thread() {
    while (1) {
		
		//Nap here 
		pthread_mutex_lock(&mutex);
		if (numberOfStudentsWaiting <= 0) {
			printf("No students need help the TA is napping\n");
		}
		pthread_mutex_unlock(&mutex);
		// TA waits for a student to arrive at his office
        sem_wait(&student_sem);
		
		// Get time the TA will take to help student 
        int helpTime = generateRandomTime();
		
		// TA will make sure that he/she is the only one that has 'control' over the seats
        pthread_mutex_lock(&mutex);
        
		// TA will take a student
		numberOfStudentsWaiting--;
		printf("\t\tTA is helping Student %d for %d seconds. Now # of waiting students = %d\n", waitingRoomChairs[nextTeachingPosition], helpTime, numberOfStudentsWaiting);
		
		// Update Pseudo-Queue to properly keep track of which student the TA will handle
		nextTeachingPosition = ( nextTeachingPosition + 1 ) % NUM_OF_CHAIRS;

		// TA will give control over the seats to another thread as it is done taking a student from there
		pthread_mutex_unlock(&mutex);
		
		// TA is helping a student
		sleep(helpTime);

		// TA is releasing the student when he/she is done helping
        sem_post(&ta_sem); 

    }
}

// *PROGRAM STARTS HERE*
int main() {		
	
	// Initalize Semaphores and Mutex
    initSemaphoresMutex();

	// Initalize Thread 'Group' For Student
    pthread_t stu[NUM_OF_STUDENTS];
	
	// Initalize Thread For TA
    pthread_t ta;

	// Array holding the ids of the students
	int student_ids[NUM_OF_STUDENTS];

	// Create Threads For Students
    for (int i = 0; i < NUM_OF_STUDENTS; i++) {
		student_ids[i] = i;
        pthread_create(&stu[i], NULL, stu_thread, (void*) &student_ids[i]);
	}

	// Create Threads For TA
    pthread_create(&ta, NULL, ta_thread, NULL);

	// Join Threads (should never get here since the program goes on 'forever')
    for (int i = 0; i < NUM_OF_STUDENTS; i++) {
        pthread_join(stu[i], NULL);
	}

    return 0;
}
