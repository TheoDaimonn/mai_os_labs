#pragma once

#include <csignal>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <stdexcept>
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>

constexpr size_t SHARED_MEM_SIZE = 4096;
constexpr const char* SHM_NAME = "/my_shared_memory";

void wait_for_children();
void signal_parent(int signal);