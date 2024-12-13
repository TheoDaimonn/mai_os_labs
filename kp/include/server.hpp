#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>
#include <vector>
#include <zmq.hpp>
#include "game.hpp"

class Server {
public:
    Server(int port);
    void run();

private:
    void handle_client_message(const std::string& message);
    void handle_create_command(const std::string& message);
    void handle_join_command(const std::string& message);
    void create_game(const std::string& game_name, int num_players);
    void join_game(const std::string& game_name, const std::string& player_name);
    void start_game(Game& game);
    void handle_guess_command(const std::string& message);

    int port_;
    zmq::context_t context_;
    zmq::socket_t socket_;
    std::vector<Game> games_;
};

#endif // SERVER_HPP