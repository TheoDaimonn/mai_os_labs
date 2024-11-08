#include "../include/parent.hpp"
#include "../include/utils.hpp"

void ParentProcess(const char* pathToChild1, const char* pathToChild2, const std::string& userInput) {
    int shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) {
        throw std::runtime_error("Failed to open shared memory");
    }

    if (ftruncate(shm_fd, SHARED_MEM_SIZE) == -1) {
        throw std::runtime_error("Failed to set shared memory size");
    }

    char* shared_memory = static_cast<char*>(mmap(0, SHARED_MEM_SIZE, PROT_WRITE | PROT_READ, MAP_SHARED, shm_fd, 0));
    if (shared_memory == MAP_FAILED) {
        throw std::runtime_error("Failed to map shared memory");
    }

    strncpy(shared_memory, userInput.c_str(), SHARED_MEM_SIZE);

    pid_t pid1 = fork();
    if (pid1 == -1) throw std::runtime_error("Failed to fork child 1");

    if (pid1 == 0) {
        execl(pathToChild1, pathToChild1, NULL);
        perror("Failed to exec child 1");
        exit(EXIT_FAILURE);
    }

    pid_t pid2 = fork();
    if (pid2 == -1) throw std::runtime_error("Failed to fork child 2");

    if (pid2 == 0) {
        execl(pathToChild2, pathToChild2, NULL);
        perror("Failed to exec child 2");
        exit(EXIT_FAILURE);
    }

    wait_for_children();

    std::cout << "Transformed result: " << shared_memory << std::endl;

    munmap(shared_memory, SHARED_MEM_SIZE);
    shm_unlink(SHM_NAME);

    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);
}