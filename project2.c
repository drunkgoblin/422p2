#include <stdio.h>
#include <pthread.h>

struct stuff {
    int id;
    int phases;
    int current_phase;
    // Phase types: 1 = CPU phase; 2 = IO phase
    int phase_and_dur[phases][phases];
    int completed;
};

struct queue {
    stuff *data;
    queue *next;
};

int main() {
    queue micky;
    micky.data.id = 3;
    print(micky.data.id);
}
