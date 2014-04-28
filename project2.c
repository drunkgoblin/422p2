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
    struct queue *next;
};

int main() {
    struct queue micky;
    struct queue rudolph;
    struct queue boris;
    micky.data.id = 3;
    rudolph.data.id = 4;
    boris.data.id = 5;
    printf("%d\n",micky.data.id);
    printf("%d\n",rudolph.data.id);
    printf("%d\n",boris.data.id);
}
