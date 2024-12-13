#include <zmq.hpp>
#include <iostream>
#include "zmq_utils.hpp"

int main() {
    zmq::context_t context(1);
    zmq::socket_t socket(context, ZMQ_REQ);
    socket.connect("tcp://localhost:5555");

    while (true) {
        std::string guess;
        std::cout << "Enter your guess (or 'exit' to quit): ";
        std::cin >> guess;

        sendMessage(socket, guess);

        std::string response = receiveMessage(socket);
        std::cout << response << std::endl;

        if (response.find("Congratulations") != std::string::npos || response.find("exited") != std::string::npos) {
            break;
        }
    }
    return 0;
}
