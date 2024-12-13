#include "zmq_utils.hpp"

void sendMessage(zmq::socket_t& socket, const std::string& message) {
    zmq::message_t msg(message.size());
    memcpy(msg.data(), message.data(), message.size());
    socket.send(msg, zmq::send_flags::none);
}

std::string receiveMessage(zmq::socket_t& socket) {
    zmq::message_t msg;
    socket.recv(msg, zmq::recv_flags::none);
    return std::string(static_cast<char*>(msg.data()), msg.size());
}
