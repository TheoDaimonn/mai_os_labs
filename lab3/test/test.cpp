#include <gtest/gtest.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>
#include <cstring>
#include <csignal>
#include <stdexcept>
#include <iostream>

#include "../include/parent.hpp"
#include "../include/utils.hpp"


class SharedMemoryTest : public ::testing::Test {
protected:
    int shm_fd;
    char* shared_memory;

    void SetUp() override {
        // Создаем или открываем разделяемую память
        shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
        ASSERT_NE(shm_fd, -1) << "Не удалось открыть разделяемую память";

        // Задаем размер разделяемой памяти
        ASSERT_EQ(ftruncate(shm_fd, SHARED_MEM_SIZE), 0) << "Не удалось установить размер разделяемой памяти";

        // Отображаем разделяемую память в адресное пространство процесса
        shared_memory = static_cast<char*>(mmap(0, SHARED_MEM_SIZE, PROT_WRITE | PROT_READ, MAP_SHARED, shm_fd, 0));
        ASSERT_NE(shared_memory, MAP_FAILED) << "Не удалось отобразить разделяемую память";
    }

    void TearDown() override {
        // Очистка разделяемой памяти
        munmap(shared_memory, SHARED_MEM_SIZE);
        shm_unlink(SHM_NAME);
        close(shm_fd);
    }
};

TEST_F(SharedMemoryTest, ChildProcessesTransformData) {
    // Исходные данные для теста
    const std::string input = "Hello World";
    
    // Проверяем, что данные были корректно записаны в память
    strncpy(shared_memory, input.c_str(), SHARED_MEM_SIZE);
    EXPECT_STREQ(shared_memory, input.c_str());

    // Запуск родительского процесса с путями к child1 и child2 и исходным вводом
    const char* pathToChild1 = "./child1";
    const char* pathToChild2 = "./child2";

    try {
        ParentProcess(pathToChild1, pathToChild2, input);
    } catch (const std::runtime_error& e) {
        FAIL() << "Ошибка в ParentProcess: " << e.what();
    }

    // Проверяем результат после выполнения всех преобразований
    EXPECT_STREQ(shared_memory, "HELLO_WORLD");
}

// Функция main для запуска всех тестов
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

