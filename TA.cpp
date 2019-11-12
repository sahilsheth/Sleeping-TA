

#include <pthread.h>		//Create POSIX threads.
#include <time.h>			//Wait for a random time.
#include <unistd.h>			//Thread calls sleep for specified number of seconds.
#include <semaphore.h>		//To create semaphores
#include <stdlib.h>
#include <stdio.h>			//Input Output
#include <iostream>
using namespace std;

pthread_t *Students;		//N threads running as Students.
pthread_t TA;				//Separate Thread for TA.

int ChairsCount = 0;
int CurrentIndex = 0;

//Declaration of Semaphores and Mutex Lock.

//Semaphores used:
sem_t sleepyTA;
sem_t s1, s2, s3; //A semaphore to signal and wait TA's sleep.
sem_t arr [3]; //An array of 3 semaphores to signal and wait chair to wait for the TA.
sem_t s4; //A semaphore to signal and wait for TA's next student.

//Mutex Lock used:
//To lock and unlock variable ChairsCount to increment and decrement its value.
pthread_mutex_t mutexLock;

//Declared Functions
void *TA_Activity(void *);
void *Student_Activity(void *threadID);

int main(int argc, char* argv[])
{
	int studentNum;		//a variable taken from the user to create student threads.	Default is 5 student threads.
	int id;
	srand(time(NULL));

	//Initializing Mutex Lock and Semaphores
  //sem_init(&arr[3], 0, 0); //Contains 3 semaphores
    sem_init(&sleepyTA, 0, 0); //Contains 3 semaphores
  sem_init(&s4, 0, 0);
  for(id = 0; id < 3; id++)
  {//Chairs array of 3 semaphores.
        sem_init(&arr[id], 0, 0);
  }
  pthread_mutex_init(&mutexLock, NULL);
  

	if(argc<2)
	{
		//printf("Number of Students not specified \n");
        cout << "Number of Students not specified " << endl;
		studentNum = 5;
	}
	else
	{
		printf("Number of Students is specified. Creating %d threads.\n", studentNum);
        //cout << "Number of Students is specified. Creating " << " " << studentNum << " " << threads << endl;
		studentNum = atoi(argv[1]);
	}

	//Allocate memory for Students
	Students = (pthread_t*) malloc(sizeof(pthread_t)*studentNum);

	//Creating one TA thread and N Student threads.
     //hint: use pthread_create

  pthread_create(&TA, NULL, TA_Activity, NULL);
 	for(id =0; id < studentNum; id++)
    {
  	  pthread_create(&Students[id], NULL, Student_Activity, (void*) (long) id);
    }

	//Waiting for TA thread and N Student threads.
  pthread_join(TA, NULL);
  for (id=0; id < studentNum; id++)
  {
      pthread_join(Students[id], NULL);
  }

  



	//Free allocated memory
	free(Students);
	return 0;
}

void *TA_Activity(void *)
{
     //TODO
    while(true)
    {
	//TA is currently sleeping.
        sem_wait(&sleepyTA);
	//printf("TA is currently sleeping. ");
        cout << "TA is currently sleeping" << endl;
	while(true)
    {
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
        //cout << " " << "Student left their chair. Now the remaining chair " << 3-ChairsCount << endl;
    CurrentIndex = (CurrentIndex + 1) % 3;
    pthread_mutex_unlock(&mutexLock);
    //unlock

	//TA is currently helping the student
	//printf("TA is currently helping the student");
        cout << "TA is current helping the student" << endl;
        sleep(8);
	sem_post(&s4);

     //hint: use sem_wait(); sem_post(); pthread_mutex_lock(); pthread_mutex_unlock()
     }
    }

}

void *Student_Activity(void *threadID)
{
   //TODO
    int time;
    while(true)
    {
        time = rand() % 10 + 1;
        sleep(time);
       //Student  needs help from the TA
        
        pthread_mutex_lock(&mutexLock);
       int countingChair= ChairsCount;
        pthread_mutex_unlock(&mutexLock);
       //Student tried to sit on a chair.
       if(countingChair<3)
       {
           //wake up the TA.

           if (countingChair==0)
           {
               sem_post(&sleepyTA);
           }
           else
           {
               printf("Student %ld is waiting for the TA. \n", (long)threadID);
               //cout << " " << Student " << (long)threadID << " is waiting for the TA. " << endl;
           }
           // lock

           pthread_mutex_lock(&mutexLock);
           int indexNum= (ChairsCount+CurrentIndex)%3;
           ChairsCount++;
           //unlock
           printf("Student sits on chair. The remaining chairs are: %d \n", 3-ChairsCount);
           pthread_mutex_unlock(&mutexLock);
           //Student leaves his/her chair.
           sem_wait(&arr[CurrentIndex]);
           //Student  is getting help from the TA
           //http://www.cplusplus.com/reference/cstdio/printf/
           printf("Student %ld is getting help from the TA. \n", (long)threadID);
           //cout << " " << "Student " << (long)threadID << " " << "is getting help from the TA" << endl;
           //Student waits to go next.
           sem_wait(&s4);
           //Student left TA room
           printf("Student %ld left TA room. \n", (long)threadID);
           //cout << " "<<"Student " << (long)threadID << " " <<" left TA room. " << endl;
       }

       
       else
       {
       //If student didn't find any chair to sit on.
       //Student will return at another time
       printf("Student %ld must come back later. \n",(long)threadID);
           //cout << " " << "Student " << (long)threadID << " " << " must come back later. " << endl;

        //hint: use sem_wait(); sem_post(); pthread_mutex_lock(); pthread_mutex_unlock()
       }
    }
}
