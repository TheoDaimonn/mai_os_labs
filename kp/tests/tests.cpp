#include "game.hpp"
#include <gtest/gtest.h>
#include <zmq.hpp>
#include <thread>
#include <string>

// Запуск сервера в отдельном потоке
void startServer() {
    system("./server &"); // Предполагается, что сервер уже скомпилирован и находится в текущем каталоге.
}

TEST(GameTest, CountBullsAndCows) {
    Game game("hello");

    auto result = game.countBullsAndCows("hallo");
    EXPECT_EQ(result.first, 4); // 4 bulls
    EXPECT_EQ(result.second, 0); // 0 cows
}

TEST(GameTest, IsWordGuessed) {
    Game game("world");
    EXPECT_TRUE(game.isWordGuessed("world"));
    EXPECT_FALSE(game.isWordGuessed("words"));
}

TEST(ServerTest, ProcessSingleGuess) {
    // Подготовка
    std::thread serverThread(startServer);
    std::this_thread::sleep_for(std::chrono::seconds(1)); // Даем время серверу запуститься

    zmq::context_t context(1);
    zmq::socket_t clientSocket(context, ZMQ_REQ);
    clientSocket.connect("tcp://localhost:5555");

    // Отправка предположения
    std::string guess = "hallo";
    zmq::message_t request(guess.size());
    memcpy(request.data(), guess.data(), guess.size());
    clientSocket.send(request, zmq::send_flags::none);

    // Получение ответа
    zmq::message_t reply;
    clientSocket.recv(reply, zmq::recv_flags::none);
    std::string response(static_cast<char*>(reply.data()), reply.size());

    // Проверка ответа
    EXPECT_TRUE(response.find("Bulls") != std::string::npos);
    EXPECT_TRUE(response.find("Cows") != std::string::npos);

    // Завершение сервера
    clientSocket.send(zmq::message_t("exit", 4), zmq::send_flags::none);
    serverThread.join();
}