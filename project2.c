#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdarg.h>
#define NUM_PHASES 10

struct job {
    int id;
    int phases;
    int current_phase;
    // Phase types: 1 = CPU phase; 2 = IO phase
    int phase_and_dur[NUM_PHASES][NUM_PHASES];
    int completed;
};

struct queue {
    struct job data;
    struct queue * next;
};
//pointers to heads and tails of queues.
struct queue * readrunning;
struct queue * readrunningend;
struct queue * waitingio;
struct queue * waitingioend;
struct queue * finished;
struct queue * finishedend;

//A newly created job
struct job *newly_created_job = NULL;

void *job(int job_id) {
    printf("I've got a job for you whore\n");

    newly_created_job = (struct job *) malloc(sizeof(struct job));

    newly_created_job->id = job_id;
    //Number of phases is 2 (cpu and IO)
    newly_created_job->phases = 2;
    //jobs start from cpu phase
    newly_created_job->current_phase = 1;
    newly_created_job->completed = 0; //not completed yet
    newly_created_job-> phase_and_dur[0][0] = 1;

    printf("The newly created job id is %d\n", newly_created_job->id);

}

void *cpu() {

    printf("cpu Thread yo\n");
    //update the job data structure with the completed phase
    int i ;
    for(i = 0; i < NUM_PHASES; i++)
    {

    }
    //add jobs to the run queue when thier phase is runnable
    for(i = 0; i < NUM_PHASES; i++)
    {
        //add jobs from the waiting queue to the runnin queue
        /*while(waitingio != NULL)
        {
            struct *job job_iterator;
            job_iterator = waitingio;
         // readrunning ->next = wa
        }
        */

        //printf("Job %d has been added to the running queue.\n", newly_created_job->id);
    }

//add jobs to the I/O queue if CPU phase is completed and next phase is I/O bound

//If a job has finished all its phases, put it to the finished queue

}

void *iostuff() {
    printf("io thread bitch\n");
}
/**
* Remove the first item in the queue
**/
struct job *pop(struct queue *the_queue) {

    struct job *next_job = &the_queue->data;
    struct queue *current = the_queue;
    while(current != NULL)
    {
        next_job = &current->data;

        current = current->next;


    }
    return next_job;
}

/**
pass in the end pointer for the queue.
**/
void add(struct queue *tail, struct job *newJob) {
    struct queue *newQueue = (struct queue *) malloc(sizeof(struct queue));
    struct queue *temp = tail;
    if (!tail) { //queue is empty
        tail = newQueue;
        tail->data = *newJob;
        tail->next = NULL;
    }else {
        temp = tail->next;
        while (temp->next) {
            temp = temp->next;
        }
        newQueue->data = *newJob;
        temp = newQueue;;
    }
    printf("job id number: %d\n", tail->data.id);

}

/**
*Add item to the queue
* queue type 1 = readyrunning, 2 = waiting, 3 = finished
**/
/*
struct queue *add(int queue_type, struct job *the_job){
    struct queue *the_queue;

    switch(queue_type)
    {
    case 1:
        the_queue = readrunning;
        break;
    case 2:
        the_queue = waitingio;
        break;
    case 3:
        the_queue = finished;
        break;
    }
    if(the_queue->size == 0)//queue is empty
    {
        the_queue->data = *the_job;
        the_queue->next = NULL; 
    }
    else//queue is not empty, so add a job infront of it. FIFO
    {
       struct queue *new_queue =
             (struct queue *) malloc(sizeof(struct queue));
       new_queue->data = *the_job;
       new_queue->next = the_queue;
    }
    the_queue->size++;

    return the_queue;

}*/

int main() {
   // readrunningend = readrunning;
   // waitingioend = waitingio;
   // finishedend = finished;
/**
//initialize
newly_created_job = (struct job *) malloc(sizeof(struct job));


pthread_t threadz[16];
int i;
for(i = 0; i <= 7; i++)
{
pthread_create(&threadz[i],NULL,cpu,NULL);
}
for(i = 8; i <= 11; i++)
{
pthread_create(&threadz[i],NULL,iostuff,NULL);
}
for(i = 12; i <= 15; i++)
{
//job number starts at 0
pthread_create(&threadz[i],NULL,job(i-12), NULL);
}
printf("yar\n");
for(i=0; i < 15; i++)
{
pthread_join(threadz[i],NULL);
}
pthread_exit(NULL);

struct queue piggy;
readrunning = &piggy;
readrunning->data.id = 3;
struct queue phillipe;
readrunning->next = &phillipe;
readrunning->next->data.id = 17;
struct queue shit;
waitingio = &shit;
finished = malloc(sizeof(struct job));
waitingio->data.id = 4;
finished->data.id = 5;
printf("%d\n",readrunning->data.id);
printf("%d\n",readrunning->next->data.id);
printf("%d\n",waitingio->data.id);
printf("%d\n",finished->data.id);
*/

    job(1); //create a job
    //readrunning = (struct queue *) malloc(sizeof(struct queue));
    add(readrunning, newly_created_job);
    job(2);
    //finished = (struct queue *) malloc(sizeof(struct queue));
    add(readrunning, newly_created_job);
    printf("First test %d\n", readrunning->data.id);
    //printf("Removed job is = %d\n", pop(readrunning)->id);
    //printf("second test %d\n", finished->data.id);
    //printf("Removed job is = %d\n", pop(finished)->id);
}

