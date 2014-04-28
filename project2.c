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


int main() {

}
