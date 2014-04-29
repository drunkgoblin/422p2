#include <stdio.h>
#include <pthread.h>

struct stuff {
    int id;
    int phases;
    int current_phase;
    // Phase types: 1 = CPU phase; 2 = IO phase
    int phase_and_dur[10][10];
    int completed;
};

struct queue {
    struct stuff data;
    struct queue * next;
}head;

struct queue * readrunning;
struct queue * waitingio;
struct queue * finished;

int job() {

}

int cpu() {

}

int iostuff() {

}

int pop() {

}

int main() {
    struct queue piggy;
    readrunning = &piggy;
    readrunning->data.id = 3;
    struct queue phillipe;
    readrunning->next = &phillipe;
    readrunning->next->data.id = 17;
    struct queue shit;
    waitingio = &shit;
    waitingio->data.id = 4;
    //finished->data->id = 5;
    printf("%d\n",readrunning->data.id);
    printf("%d\n",readrunning->next->data.id);
    printf("%d\n",waitingio->data.id);
    //printf("%d\n",finished->data->id);
}
