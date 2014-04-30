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

void *job() {

}

void *cpu() {

}

void *iostuff() {

}

int pop() {

}

int main() {
    pthread_t threadz[16];
    int i;
    for(i = 0; i <= 7; i ++)
    {
	pthread_create(&threadz[i],NULL,cpu,NULL);
    }
    for(i = 8; i <= 11; i ++)
    {
	pthread_create(&threadz[i],NULL,iostuff,NULL);
    }
    for(i = 12; i <= 15; i ++)
    {
	pthread_create(&threadz[i],NULL,job, NULL);
    }
    printf("yar\n");
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
