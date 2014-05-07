#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdarg.h>
#define NUM_PHASES 10
#define MAX_JOBS 5
pthread_mutex_t gimp;
//void *add(struct queue **tail, struct job **newJob);
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
//pointers to heads of queues.
struct queue * readrunning;
struct queue * waitingio;
struct queue * finished;

/**
* Remove the first item in the queue
**/
struct job *pop(struct queue **the_queue) {
    struct job *next_job;
    if (*the_queue != NULL) {
        struct queue *current = *the_queue;
        next_job = current->data;
        current = current->next;
        free(*the_queue);
        *the_queue = current;
    } else {
        next_job = NULL;
    }
    return next_job;
}

/**
pass in the end pointer for the queue.
**/
void *add(struct queue **tail, struct job **newJob) {
    struct queue *temp = *tail;
    if (!temp) { //queue is empty
        temp = malloc(sizeof(struct queue));
        *tail = temp;
        temp->data = *newJob;
        temp->next = NULL;
    }else {
        while (temp->next) {
            temp = temp->next;
        }
        temp->next = malloc(sizeof(struct queue));
        temp->next->data = *newJob;
        temp = temp->next;
    }
    printf("job id number: %d added to queue\n", temp->data->id);
    return;
}

void *iostuff() {
    printf("io thread bitch\n");
    pthread_mutex_lock(&gimp);
    while(completed_jobs < MAX_JOBS) {
        //printf("io thread bitch yo\n");
        //pthread_mutex_lock(&gimp);
        struct job * the_job = pop(&waitingio);
        //printf("io thread bitch ass yo, poppin'\n");
        pthread_mutex_unlock(&gimp);
        if (the_job != NULL) {
            //struct job * jobptr = the_job;
            printf("shut this io bitch up: %d\n",the_job->id);
            sleep(the_job->Dur[the_job->current_phase] - 1);
            the_job->current_phase++;
            if(the_job->current_phase > the_job->phases)
            {
                printf("io thread bitch ass yo, bust a nut!\n");
                the_job->completed = 1;
                pthread_mutex_lock(&gimp);
                completed_jobs++;
                pthread_mutex_unlock(&gimp);
            }
            if(the_job->completed == 1)
            {
                printf("in the butt, 5 5 5\n");
                pthread_mutex_lock(&gimp);
                add(&finished, &the_job);
                //pthread_mutex_unlock(&gimp);
            }else {
                pthread_mutex_lock(&gimp);
                add(&readrunning, &the_job);
                //pthread_mutex_unlock(&gimp);
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
        struct job * the_job = pop(&readrunning);
        //printf("cpu Thread yo, poppin'\n");
        pthread_mutex_unlock(&gimp);
        if (the_job != NULL) {
            printf("this fucking whore is working yo %d\n",the_job->id);
            //struct job * jobptr = the_job;
            sleep(the_job->Dur[the_job->current_phase] - 1);
            the_job->current_phase++;
            if(the_job->current_phase > the_job->phases)
            {
                printf("cpu Thread yo, done yo!\n");
                the_job->completed = 1;
                pthread_mutex_lock(&gimp);
                completed_jobs++;
                pthread_mutex_unlock(&gimp);
            }
            if(the_job->completed == 1)
            {
                printf("stick it in your ear\n");
                pthread_mutex_lock(&gimp);
                add(&finished, &the_job);
                //pthread_mutex_unlock(&gimp);
            }else {
                pthread_mutex_lock(&gimp);
                add(&waitingio, &the_job);
                //pthread_mutex_unlock(&gimp);
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
    newly_created_job->phases = 3;
    //jobs start from cpu phase
    newly_created_job->current_phase = 1;
    newly_created_job->completed = 0; //not completed yet
    int i = 0;
    for(i;i<5;i++)
    {
    newly_created_job-> Dur[i] = 1;
    }

    printf("The newly created job id is %d\n", newly_created_job->id);
    pthread_mutex_lock(&gimp);
    add(&readrunning, &newly_created_job);
    //pthread_mutex_unlock(&gimp);
    }
    pthread_mutex_unlock(&gimp);
    return;

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
