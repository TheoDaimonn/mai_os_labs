#include <gtest/gtest.h>
#include "node_routine.hpp"
#include "socket_routine.hpp"
#include <unistd.h>
#include <string>

// Тест для проверки создания узлов
TEST(NodeTests, CreateNode) {
    Node root(-1);
    ASSERT_EQ(root.id, -1);
    auto result = root.Create(1, getenv("PROGRAM_PATH"));
    ASSERT_NE(result.find("Ok:"), std::string::npos);
}

// Тест для проверки отправки команды start таймера
TEST(NodeTests, TimerStart) {
    Node root(-1);
    root.Create(1, getenv("PROGRAM_PATH"));
    auto response = root.Send("exec 1 start", 1);
    ASSERT_NE(response.find("Ok:"), std::string::npos);
}

// Тест для проверки команды time таймера
TEST(NodeTests, TimerTime) {
    Node root(-1);
    root.Create(1, getenv("PROGRAM_PATH"));
    root.Send("exec 1 start", 1);
    sleep(2); // Wait for 2 seconds
    auto response = root.Send("exec 1 time", 1);
    size_t pos = response.find("Ok: ");
    std::string timeStr = response.substr(pos + 4);
    int time = std::stoi(timeStr);
    std::cout << time << std::endl;
    ASSERT_NE(time, 2000); 

}

TEST(NodeTests, KillNode) {
    Node root(-1);
    root.Create(1, getenv("PROGRAM_PATH"));
    auto response = root.Send("kill", 1);
    ASSERT_EQ(response.find("Ok"), std::string::npos); 
}

// Тест для обработки неизвестной команды
TEST(NodeTests, UnknownCommand) {
    Node root(-1);
    auto response = root.Send("unknown_command", -1);
    ASSERT_EQ(response, "Error: Not found");
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}