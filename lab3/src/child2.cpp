#include "../include/utils.hpp"
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <cstring>
int main() {
    int shm_fd = shm_open(SHM_NAME, O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("Failed to open shared memory");
        return EXIT_FAILURE;
    }

    char* shared_memory = static_cast<char*>(mmap(0, SHARED_MEM_SIZE, PROT_WRITE | PROT_READ, MAP_SHARED, shm_fd, 0));
    if (shared_memory == MAP_FAILED) {
        perror("Failed to map shared memory");
        return EXIT_FAILURE;
    }
    for (char* p = shared_memory; *p; ++p) {
        if (*p == ' ') *p = '_';
    }
    kill(getppid(), SIGUSR2);

    munmap(shared_memory, SHARED_MEM_SIZE);
    close(shm_fd);
    return EXIT_SUCCESS;
}