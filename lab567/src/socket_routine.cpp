#include "socket_routine.hpp"
#include <iostream>
#include <zmq.hpp>

int Bind(zmq::socket_t* socket, int id) {
    int port = 4040 + id;
    while (true) {
        try {
            socket->bind("tcp://127.0.0.1:" + std::to_string(port));
            break;
        } catch (...) {
            port++;
        }
    }
    return port;
}

void Unbind(zmq::socket_t* socket, int port) {
    socket->unbind("tcp://127.0.0.1:" + std::to_string(port));
}

void Connect(zmq::socket_t* socket, int port) {
    socket->connect("tcp://127.0.0.1:" + std::to_string(port));
}

void Disconnect(zmq::socket_t* socket, int port) {
    socket->disconnect("tcp://127.0.0.1:" + std::to_string(port));
}

bool SendMessage(zmq::socket_t* socket, const std::string& message) {
    zmq::message_t zmqMessage(message.begin(), message.end()); // Преобразуем std::string в zmq::message_t
    zmq::send_result_t result = socket->send(zmqMessage, zmq::send_flags::none);
    return result.has_value(); // Проверяем, было ли сообщение успешно отправлено
}

std::optional<std::string> ReceiveMessage(zmq::socket_t* socket) {
    zmq::message_t message;
    zmq::recv_result_t result = socket->recv(message, zmq::recv_flags::none);
    if (result.has_value()) {
        return message.to_string(); // Преобразуем сообщение в строку
    }
    return std::nullopt; // Возвращаем пустое значение в случае неудачи
}
