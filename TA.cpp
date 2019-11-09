

#include <pthread.h>		//Create POSIX threads.
#include <time.h>			//Wait for a random time.
#include <unistd.h>			//Thread calls sleep for specified number of seconds.
#include <semaphore.h>		//To create semaphores
#include <stdlib.h>
#include <stdio.h>			//Input Output

pthread_t *Students;		//N threads running as Students.
pthread_t TA;				//Separate Thread for TA.

int ChairsCount = 0;
int CurrentIndex = 0;

//Declaration of Semaphores and Mutex Lock.

//Semaphores used:
sem_t s1; //A semaphore to signal and wait TA's sleep.
sem_t s2;
sem_t s3;
sem_t arr[s1, s2, s3]; //An array of 3 semaphores to signal and wait chair to wait for the TA.
sem_t s4; //A semaphore to signal and wait for TA's next student.
sem_t sleepyTA;

//Mutex Lock used:
//To lock and unlock variable ChairsCount to increment and decrement its value.
pthread_mutex_t mutexLock;

//Declared Functions
void *TA_Activity();
void *Student_Activity(void *threadID);

int main(int argc, char* argv[])
{
	int number_of_students;		//a variable taken from the user to create student threads.	Default is 5 student threads.
	int id;
	srand(time(NULL));

	//Initializing Mutex Lock and Semaphores
  sem_init(&arr, 0, 1); //Contains 3 semaphores
  sem_init(&s4, 0, 1);
  pthread_mutex_init(&mutexLock, NULL);


	if(argc<2)
	{
		printf("Number of Students not specified. Using default (5) students.\n");
		number_of_students = 5;
	}
	else
	{
		printf("Number of Students specified. Creating %d threads.\n", number_of_students);
		number_of_students = atoi(argv[1]);
	}

	//Allocate memory for Students
	Students = (pthread_t*) malloc(sizeof(pthread_t)*number_of_students);

	//Creating one TA thread and N Student threads.
     //hint: use pthread_create
  pthread_create(&Students, NULL, &Student_Activity, NULL);
  pthread_create(&TA, NULL, &TA_Activity, NULL);
	//Waiting for TA thread and N Student threads.
  pthread_join(&Students, NULL);
  pthread_join(&TA, NULL);
	//Free allocated memory
	free(Students);
	return 0;
}

void *TA_Activity()
{
     TODO
	//TA is currently sleeping.
        sem_wait(&sleepyTA);
	printf("TA is currently sleeping. ");
	while(true){
    // lock
    //https://www.geeksforgeeks.org/mutex-lock-for-linux-thread-synchronization/ 
        pthread_mutex_lock(&mutexLock);
	if(ChairsCount == 0)
	{
    //if chairs are empty, break the loop.
		pthread_mutex_unlock(&mutexLock);
		break;
        }
	//TA gets next student on chair.
    sem_post(&arr[CurrentIndex]);
    ChairsCount--;
    printf("Student left their chair. Now the remaining chairs %d\n", 3-ChairsCount);
    CurrentIndex = (CurrentIndex + 1) % 3;
    pthread_mutex_unlock(&mutexLock);
    //unlock

	//TA is currently helping the student

     //hint: use sem_wait(); sem_post(); pthread_mutex_lock(); pthread_mutex_unlock()
}

}

void *Student_Activity(void *threadID)
{
    //TODO

	//Student  needs help from the TA
	//Student tried to sit on a chair.
	//wake up the TA.
    // lock

	// unlock
    //Student leaves his/her chair.
    sem_wait(&arr[CurrentIndex]);
	//Student  is getting help from the TA
	//http://www.cplusplus.com/reference/cstdio/printf/ 
	printf("Student %ld is getting help from the TA", (long)threadID);
	//Student waits to go next.
	sem_wait(&s4);
	//Student left TA room
	printf("Student %ld left TA room. ", (long)threadID);

    //If student didn't find any chair to sit on.
    //Student will return at another time

     //hint: use sem_wait(); sem_post(); pthread_mutex_lock(); pthread_mutex_unlock()

	
}
