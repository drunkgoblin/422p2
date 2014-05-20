#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>

#define NUM_PHASES 2
#define MAX_JOBS 8
pthread_mutex_t rr_lock;
pthread_mutex_t io_lock;
pthread_mutex_t fin_lock;
pthread_mutex_t id_lock;
pthread_mutex_t completed_lock;

int completed_jobs;
int job_id;
struct job {
    int cpuId;
    int id;
    int phases;
    int current_phase;
    int Dur[NUM_PHASES];
    int completed;
};

struct queue {
    struct job * data;
    struct queue * next;
};
//pointers to heads and tails of queues.
//struct queue * readrunning;
//struct queue * readrunningEnd;
struct queue * run[8][2];
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
        current->next = NULL;
    } else {
       current= NULL;
    }
    return current;
}

/**
pass in the end pointer for the queue.
**/
//are the adds above this one going away? yes
void *add(struct queue **head, struct queue **tail, struct queue **newJob) {
if (!*head) {
    *head = *newJob;
    *tail = *newJob;
} else {
    struct queue *temp = *tail;
    temp->next = *newJob;
    temp = temp->next;
    temp->next = NULL;
    *tail = *newJob;
}
return;
//should we leave all this in here when we turn in? awesome. hi shrew. die, mostly.
//I wonder why it isn’t changing colors now its your name and theres a shrew up there, on
//my surface i’m logged in as me or something I vote yes, what do shrews do? /dies
}
void *iostuff() {//ooh bag fries. I should try those. I always get BK onion rings hollaaaaaa
    
    pthread_mutex_lock(&completed_lock);
    while(completed_jobs < MAX_JOBS) {
        pthread_mutex_unlock(&completed_lock);
        pthread_mutex_lock(&io_lock);
        struct queue * the_job = pop(&waitingio);
        pthread_mutex_unlock(&io_lock);
        if (the_job != NULL) {
             printf("Job %d is running on IO\n",the_job->data->id);
            sleep(the_job->data->Dur[the_job->data->current_phase] );
            the_job->data->current_phase++;
            if(the_job->data->current_phase > the_job->data->phases - 1)
            {
                
                the_job->data->completed = 1;
                pthread_mutex_lock(&completed_lock);
                completed_jobs++;
                pthread_mutex_unlock(&completed_lock);
            }
            if(the_job->data->completed == 1)
            {
                printf("Job %d is done, putting on finished queue\n",the_job->data->id);
                pthread_mutex_lock(&fin_lock);
                add(&finished,&finishedEnd,&the_job);
                pthread_mutex_unlock(&fin_lock);
            }else {
                printf("Job %d is moving to CPU queue\n",the_job->data->id);
                pthread_mutex_lock(&rr_lock);
                add(&run[the_job->data->cpuId][0],&run[the_job->data->cpuId][1],&the_job);
                pthread_mutex_unlock(&rr_lock);
            }
        
        }
        pthread_mutex_lock(&completed_lock);
    }
    pthread_mutex_unlock(&completed_lock);
    return;
}

void *cpu(void * me) {
    int id = ((int )me);
    pthread_mutex_lock(&completed_lock);
    while(completed_jobs < MAX_JOBS) {
         pthread_mutex_unlock(&completed_lock);
        pthread_mutex_lock(&rr_lock);
        struct queue * the_job = pop(&run[id][0]);
        pthread_mutex_unlock(&rr_lock);
        if (the_job != NULL) {
            printf("Job %d is running on CPU\n",the_job->data->id);
            //struct job * jobptr = the_job;
            sleep(the_job->data->Dur[the_job->data->current_phase] );
            the_job->data->current_phase++;
            if(the_job->data->current_phase > the_job->data->phases - 1)
            {
                the_job->data->completed = 1;
                pthread_mutex_lock(&completed_lock);
                completed_jobs++;
                pthread_mutex_unlock(&completed_lock);
            }
            if(the_job->data->completed == 1)
            {
                printf("Job %d is done, putting in finished queue\n",the_job->data->id);
                pthread_mutex_lock(&fin_lock);
                add(&finished,&finishedEnd,&the_job);
                pthread_mutex_unlock(&fin_lock);
            }else {
                printf("Job %d is moving to IO queue\n",the_job->data->id);
                pthread_mutex_lock(&io_lock);
                add(&waitingio,&waitingioEnd,&the_job);
                pthread_mutex_unlock(&io_lock);
            }
        }
        pthread_mutex_lock(&completed_lock);
    }
    pthread_mutex_unlock(&completed_lock);
    return;
}

void *job() {
    
    //loop be here arrrrrrrrrrrrrrrrrrrrr!
    int cpuId = 0;
    int id = 0;
    srand(time(NULL));
    pthread_mutex_lock(&id_lock);
    while(job_id < MAX_JOBS)
    {
    id = job_id;
    job_id++;
    pthread_mutex_unlock(&id_lock);
    sleep(2);
    struct queue * newly_created_job = (struct queue *) malloc(sizeof(struct queue));
    newly_created_job->data = (struct job *) malloc(sizeof(struct job));
    newly_created_job->data->cpuId = cpuId;
    printf("Job number: %d created\n", cpuId);
    if (cpuId < 7) {
        cpuId++;
    } else {
        cpuId = 0;
    }
    newly_created_job->data->id = id;
    //Number of phases is 2 (cpu and IO)
    newly_created_job->data->phases = (rand() % NUM_PHASES) + 1;
    //jobs start from cpu phase
    newly_created_job->data->current_phase = 0;
    newly_created_job->data->completed = 0; //not completed yet
    newly_created_job->next = NULL;
    int i = 0;
    for(i;i<newly_created_job->data->phases;i++)
    {
    newly_created_job->data-> Dur[i] = rand() % 5;
    }

    printf("Job %d has been created\n", newly_created_job->data->id);
    pthread_mutex_lock(&rr_lock);
    add(&run[newly_created_job->data->cpuId][0],&run[newly_created_job->data->cpuId][1],&newly_created_job);
    pthread_mutex_unlock(&rr_lock);
    if (finished != NULL) {
        pthread_mutex_lock(&fin_lock);
        struct queue * goner = pop(&finished);
        if (finished == NULL) {
            finishedEnd = NULL;
        }
        pthread_mutex_unlock(&fin_lock);
        printf("Memory used by job %d is free!\n",goner->data->id);
            free(goner->data);
        free(goner);
    }
    pthread_mutex_lock(&id_lock);
    }
    pthread_mutex_unlock(&id_lock);
        pthread_mutex_lock(&completed_lock);
    while(completed_jobs < MAX_JOBS) {
    pthread_mutex_unlock(&completed_lock);
        if (finished != NULL) {
            pthread_mutex_lock(&fin_lock);
            struct queue * goner2 = pop(&finished);
            if (finished == NULL) {
                finishedEnd = NULL;
            }
            pthread_mutex_unlock(&fin_lock);
            printf("Memory used by job %d is free!\n",goner2->data->id);
            free(goner2->data);
            free(goner2);
        }
        pthread_mutex_lock(&completed_lock);
    }
    pthread_mutex_unlock(&completed_lock);
    return;

}

int main() {


//initialize
completed_jobs = 0;
job_id = 0;
pthread_mutex_init(&io_lock, NULL);
pthread_mutex_init(&rr_lock, NULL);
pthread_mutex_init(&fin_lock, NULL);
pthread_mutex_init(&id_lock, NULL);
pthread_mutex_init(&completed_lock, NULL);
pthread_t threadz[16];
int i;
for(i = 0; i <= 7; i++)
{
pthread_create(&threadz[i],NULL,cpu,(void *) i);
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
printf("Done bitches!!\n");
pthread_exit(NULL);
}
