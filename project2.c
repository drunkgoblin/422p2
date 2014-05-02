#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdarg.h>

struct job {
    int id;
    int phases;
    int current_phase;
    // Phase types: 1 = CPU phase; 2 = IO phase
    int phase_and_dur[10][10];
    int completed;
};

struct queue {
    struct job data;
    struct queue * next;
};

struct queue * readrunning;
struct queue * waitingio;
struct queue * finished;

//A newly created job
struct job *newly_created_job;

void *job(int job_id) {
printf("I've got a job for you whore\n");
struct job current_job;
newly_created_job = &current_job;
newly_created_job->id = job_id;
printf("The newly created job id is %d\n", newly_created_job->id);
	
}

void *cpu() {
printf("cpu Thread yo\n");
//update the job data structure with the completed phase

//add jobs to the run queue when thier phase is runnable

//add jobs to the I/O queue if CPU phase is completed and next phase is I/O  bound

//If a job has finished all its phases, put it to the finished queue

}

void *iostuff() {
printf("io thread bitch\n");
}

int pop() {

}

int main() {
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
	pthread_create(&threadz[i],NULL,job(3), NULL);
    }
    printf("yar\n");
    for(i=0; i < 15; i++)
	{
	pthread_join(threadz[i],NULL);
	}

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
*/
}
