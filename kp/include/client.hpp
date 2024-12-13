#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include <zmq.hpp>

class Client {
public:
    Client(const std::string& server_address);
    void run();

private:
    void send_message(const std::string& message);
    std::string receive_message();

    std::string server_address_;
    zmq::context_t context_;
    zmq::socket_t socket_;
};

#endif // CLIENT_HPP