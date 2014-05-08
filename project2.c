#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdarg.h>
#define NUM_PHASES 4
#define MAX_JOBS 5
pthread_mutex_t gimp;

int completed_jobs;
int job_id;
struct job {
    int id;
    int phases;
    int current_phase;
    // Phase types: 1 = CPU phase; 2 = IO phase
    int Dur[5];

    int completed;
};

struct queue {
    struct job * data;
    struct queue * next;
};
//pointers to heads and tails of queues.
struct queue * readrunning;
struct queue * readrunningEnd;
struct queue * waitingio;
struct queue * waitingioEnd;
struct queue * finished;
struct queue * finishedEnd;

/**
* Remove the first item in the queue
**/
struct queue *pop(struct queue **the_queue) {
   struct queue *current = *the_queue;
    if (*the_queue != NULL) {
        *the_queue = current->next;      
    } else {
       current= NULL;
    }
    return current;
}

/**
pass in the end pointer for the queue.
**/
void *add(struct job **newJob) {
    if (!readrunning) { //queue is empty
        readrunning = malloc(sizeof(struct queue));
        readrunningEnd = readrunning;
        readrunning->data = *newJob;
        readrunning->next = NULL;
    }else {
        readrunningEnd->next = malloc(sizeof(struct queue));
        readrunningEnd->next->data = *newJob;
        readrunningEnd = readrunningEnd->next;
    }
    return;
}

void *addIO(struct queue **newJob) {
    if (!waitingio) { //queue is empty
        waitingio = *newJob;
        waitingioEnd = waitingio;
        waitingio->next = NULL;
    }else {
        waitingioEnd->next = *newJob;
        waitingioEnd = waitingioEnd->next;
    }
    return;
}

void *addRR(struct queue **newJob) {
    if (!readrunning) { //queue is empty
        readrunning = *newJob;
        readrunningEnd = readrunning;
        readrunning->next = NULL;
    }else {
        readrunningEnd->next = *newJob;
        readrunningEnd = readrunningEnd->next;
    }
    return;
}

void *addF(struct queue **newJob) {
    if (!finished) { //queue is empty
        finished = *newJob;
        finishedEnd = finished;
        finished->next = NULL;
    }else {
        finishedEnd->next = *newJob;
        finishedEnd = finishedEnd->next;
    }
    return;
}

void *iostuff() {
    
    pthread_mutex_lock(&gimp);
    while(completed_jobs < MAX_JOBS) {
        //printf("io thread bitch yo\n");
        //pthread_mutex_lock(&gimp);
        struct queue * the_job = pop(&waitingio);
        //printf("io thread bitch ass yo, poppin'\n");
        pthread_mutex_unlock(&gimp);
        if (the_job != NULL) {
            //struct job * jobptr = the_job;
             printf("Job %d is running on IO\n",the_job->data->id);
            sleep(the_job->data->Dur[the_job->data->current_phase] - 1);
            the_job->data->current_phase++;
            if(the_job->data->current_phase > the_job->data->phases)
            {
                
                the_job->data->completed = 1;
                pthread_mutex_lock(&gimp);
                completed_jobs++;
                pthread_mutex_unlock(&gimp);
            }
            if(the_job->data->completed == 1)
            {
                printf("Job %d is done, putting on finished queue\n",the_job->data->id);
                pthread_mutex_lock(&gimp);
                addF(&the_job);
            }else {
printf("Job %d is moving to CPU queue\n",the_job->data->id);
                pthread_mutex_lock(&gimp);
                addRR(&the_job);
            }
        
        }
    }
    pthread_mutex_unlock(&gimp);
    return;
}

void *cpu() {

    pthread_mutex_lock(&gimp);
    while(completed_jobs < MAX_JOBS) {
        //printf("cpu Thread yo\n");
        //pthread_mutex_lock(&gimp);
        struct queue * the_job = pop(&readrunning);
        //printf("cpu Thread yo, poppin'\n");
        pthread_mutex_unlock(&gimp);
        if (the_job != NULL) {
            printf("Job %d is running on CPU\n",the_job->data->id);
            //struct job * jobptr = the_job;
            sleep(the_job->data->Dur[the_job->data->current_phase] - 1);
            the_job->data->current_phase++;
            if(the_job->data->current_phase > the_job->data->phases)
            {
                the_job->data->completed = 1;
                pthread_mutex_lock(&gimp);
                completed_jobs++;
                pthread_mutex_unlock(&gimp);
            }
            if(the_job->data->completed == 1)
            {
                printf("Job %d is done, putting in finished queue\n",the_job->data->id);
                pthread_mutex_lock(&gimp);
                addF(&the_job);
            }else {
printf("Job %d is moving to IO queue\n",the_job->data->id);
                pthread_mutex_lock(&gimp);
                addIO(&the_job);
            }
        }
    }
    pthread_mutex_unlock(&gimp);
    return;
}

void *job() {
    
    //loop be here ar!
    int id = 0;
    pthread_mutex_lock(&gimp);
    while(job_id < MAX_JOBS)
    {
    //pthread_mutex_lock(&gimp);
    //if (job_id < MAX_JOBS) {
    id = job_id;
    job_id++;
    pthread_mutex_unlock(&gimp);
    sleep(2);
    struct job * newly_created_job = (struct job *) malloc(sizeof(struct job));
    newly_created_job->id = id;
    //Number of phases is 2 (cpu and IO)
    newly_created_job->phases = NUM_PHASES;
    //jobs start from cpu phase
    newly_created_job->current_phase = 1;
    newly_created_job->completed = 0; //not completed yet
    int i = 0;
    for(i;i<=NUM_PHASES;i++)
    {
    newly_created_job-> Dur[i] = 1;
    }

    printf("Job %d has been created\n", newly_created_job->id);
    pthread_mutex_lock(&gimp);
    add(&newly_created_job);
    //pthread_mutex_unlock(&gimp);
    }
    pthread_mutex_unlock(&gimp);
    return;

}

int main() {


//initialize
//newly_created_job = (struct job *) malloc(sizeof(struct job));
completed_jobs = 0;
job_id = 0;
pthread_mutex_init(&gimp, NULL);
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
pthread_create(&threadz[i],NULL,job, NULL);
}
printf("yar\n");
for(i=0; i <= 15; i++)
{
pthread_join(threadz[i],NULL);
}
printf("oops!\n");
pthread_exit(NULL);
/*
job(1); //create a job
//readrunning = (struct queue *) malloc(sizeof(struct queue));
add(&readrunning, &newly_created_job);
job(2);
//finished = (struct queue *) malloc(sizeof(struct queue));
add(&readrunning, &newly_created_job);
printf("First test %d\n", readrunning->data->id);
printf("second test %d\n", readrunning->next->data->id);
printf("Removed job is = %d\n", pop(&readrunning)->id);
printf("Removed job is = %d\n", pop(&readrunning)->id);
*/
}
