#include <gtest/gtest.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>
#include <cstring>
#include <csignal>
#include <stdexcept>
#include <iostream>
#include <sstream>
#include <chrono>
#include <thread>

#include "../include/parent.hpp"
#include "../include/utils.hpp"

class SharedMemoryTest : public ::testing::Test {
protected:
    int shm_fd;
    char* shared_memory;

    void SetUp() override {
        shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
        ASSERT_NE(shm_fd, -1) << "Не удалось открыть разделяемую память";
        ASSERT_EQ(ftruncate(shm_fd, SHARED_MEM_SIZE), 0) << "Не удалось установить размер разделяемой памяти";
        shared_memory = static_cast<char*>(mmap(0, SHARED_MEM_SIZE, PROT_WRITE | PROT_READ, MAP_SHARED, shm_fd, 0));
        ASSERT_NE(shared_memory, MAP_FAILED) << "Не удалось отобразить разделяемую память";
    }

    void TearDown() override {
        munmap(shared_memory, SHARED_MEM_SIZE);
        shm_unlink(SHM_NAME);
        close(shm_fd);
    }
};

TEST_F(SharedMemoryTest, ChildProcessesTransformMultipleInputs) {
    const char* pathToChild1 = std::getenv("PATH_TO_CHILD1");
    const char* pathToChild2 = std::getenv("PATH_TO_CHILD2");

    std::vector<std::pair<std::string, std::string>> test_cases = {
        {"Hello World", "Transformed result: HELLO_WORLD\n"},
        {"Testing Shared Memory", "Transformed result: TESTING_SHARED_MEMORY\n"},
        {"multiple words test", "Transformed result: MULTIPLE_WORDS_TEST\n"}
    };

    for (const auto& [input, expected_output] : test_cases) {
        memset(shared_memory, 0, SHARED_MEM_SIZE);
        strncpy(shared_memory, input.c_str(), SHARED_MEM_SIZE);

        std::stringstream input_stream;
        std::stringstream output_stream;

        input_stream << input << std::endl;

        int original_stdin = dup(STDIN_FILENO);
        int original_stdout = dup(STDOUT_FILENO);

        int input_pipe[2];
        int output_pipe[2];

        ASSERT_EQ(pipe(input_pipe), 0);
        ASSERT_EQ(pipe(output_pipe), 0);

        dup2(input_pipe[0], STDIN_FILENO);
        dup2(output_pipe[1], STDOUT_FILENO);

        close(input_pipe[0]);
        close(output_pipe[1]);

        write(input_pipe[1], input.c_str(), input.size());
        close(input_pipe[1]);

        pid_t pid = fork();
        ASSERT_NE(pid, -1) << "Не удалось создать процесс";

        if (pid == 0) {
            ParentProcess(pathToChild1, pathToChild2);
            exit(0); 
        } else {
            std::this_thread::sleep_for(std::chrono::seconds(1)); 

            char buffer[1024];
            ssize_t bytes_read = read(output_pipe[0], buffer, sizeof(buffer) - 1);
            ASSERT_GT(bytes_read, 0);
            buffer[bytes_read] = '\0';

            std::string result(buffer);
            EXPECT_EQ(result, expected_output) << "transmutation error" << input << "'";

            int status;
            waitpid(pid, &status, 0);
            ASSERT_TRUE(WIFEXITED(status)) << "child process error";
        }

        dup2(original_stdin, STDIN_FILENO);
        dup2(original_stdout, STDOUT_FILENO);

        close(original_stdin);
        close(original_stdout);

        close(input_pipe[0]);
        close(output_pipe[0]);
    }
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
