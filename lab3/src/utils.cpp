#include "utils.hpp"
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <iostream>

volatile sig_atomic_t child1_ready = 0;
volatile sig_atomic_t child2_ready = 0;

void handle_child1_ready(int) { child1_ready = 1; }
void handle_child2_ready(int) { child2_ready = 1; }

void wait_for_children() {
    signal(SIGUSR1, handle_child1_ready);
    signal(SIGUSR2, handle_child2_ready);

    while (!child1_ready || !child2_ready) {
        pause();
    }
}