#ifndef ZMQ_UTILS_HPP
#define ZMQ_UTILS_HPP

#include <zmq.hpp>
#include <string>

void sendMessage(zmq::socket_t& socket, const std::string& message);
std::string receiveMessage(zmq::socket_t& socket);

#endif // ZMQ_UTILS_HPP
