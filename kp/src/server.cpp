#include <zmq.hpp>
#include <iostream>
#include <thread>
#include <vector>
#include "game.hpp"
#include "zmq_utils.hpp"

void handleClient(zmq::context_t& context, Game& game) {
    zmq::socket_t socket(context, ZMQ_REP);
    socket.connect("inproc://workers");

    while (true) {
        std::string guess = receiveMessage(socket);

        if (guess == "exit") {
            sendMessage(socket, "You have exited the game.");
            break;
        }

        auto [bulls, cows] = game.countBullsAndCows(guess);
        std::string response = (game.isWordGuessed(guess)) ?
            "Congratulations! You have guessed the word." :
            "Bulls: " + std::to_string(bulls) + ", Cows: " + std::to_string(cows);

        sendMessage(socket, response);
    }
}

int main() {
    zmq::context_t context(1);
    zmq::socket_t clients(context, ZMQ_ROUTER);
    zmq::socket_t workers(context, ZMQ_DEALER);
    clients.bind("tcp://*:5555");
    workers.bind("inproc://workers");

    std::string secretWord = "mewmew";
    Game game(secretWord);

    int numPlayers;
    std::cout << "Enter the number of players: ";
    std::cin >> numPlayers;

    std::vector<std::thread> threads;
    for (int i = 0; i < numPlayers; ++i) {
        threads.emplace_back(handleClient, std::ref(context), std::ref(game));
    }

    zmq::proxy(clients, workers, nullptr);

    for (auto& thread : threads) {
        thread.join();
    }
    return 0;
}
