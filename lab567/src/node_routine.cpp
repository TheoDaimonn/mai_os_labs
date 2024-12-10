#include "node_routine.hpp"
#include <unordered_map>
#include <chrono>
#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <sys/wait.h>

std::unordered_map<int, std::chrono::time_point<std::chrono::high_resolution_clock>> timers;
std::unordered_map<int, std::chrono::milliseconds> elapsedTimes;

std::string Node::Ping(int _id) {
    std::string ans = "Ok: 0";
    if (_id == id) {
        ans = "Ok: 1";
        return ans;
    } else if (auto it = children.find(_id); it != children.end()) {
        std::string msg = "ping " + std::to_string(_id);
        SendMessage(it->second.get(), msg);
        if (auto msg = ReceiveMessage(children[_id].get()); msg.has_value() && *msg == "Ok: 1") {
            ans = *msg;
        }
        return ans;
    }
    return ans;
}

std::string Node::Create(int idChild, const std::string &programPath) {
    std::string programName = programPath.substr(programPath.find_last_of("/") + 1);
    children[idChild] = std::make_unique<zmq::socket_t>(context, ZMQ_REQ);

    int newPort = Bind(children[idChild].get(), idChild);
    childrenPort[idChild] = newPort;
    int pid = fork();

    if (pid == -1) { 
        perror("Fork failed");
        return "Error: fork failed";
    } else if (pid == 0) { 
        execl(programPath.c_str(), programName.c_str(), std::to_string(idChild).c_str(), std::to_string(newPort).c_str(), nullptr);
        perror("execl failed");
        return "Error: execl failed";
    } else { 
        std::string pidChild = "Error: couldn't connect to child";
        children[idChild]->set(zmq::sockopt::sndtimeo, 3000); // Use new API
        SendMessage(children[idChild].get(), "pid");
        if (auto msg = ReceiveMessage(children[idChild].get()); msg.has_value()) {
            pidChild = *msg;
        }
        return "Ok: " + pidChild;
    }
}

std::string Node::Pid() {
    return std::to_string(getpid());
}

std::string Node::Send(const std::string &str, int id) {
    if (children.size() == 0) {
        return "Error: Not found";
    } else if (auto it = children.find(id); it != children.end()) {
        if (SendMessage(it->second.get(), str)) {
            std::string ans = "Error: Not found";
            if (auto msg = ReceiveMessage(children[id].get()); msg.has_value()) {
                ans = *msg;
            }
            return ans;
        }
    } else {
        std::string ans = "Error: Not found";
        for (auto &child : children) {
            std::string msg = "send " + std::to_string(id) + " " + str;
            if (SendMessage(child.second.get(), msg)) {
                if (auto msg = ReceiveMessage(child.second.get()); msg.has_value()) {
                    ans = *msg;
                }
            }
        }
        return ans;
    }
    return "Error: Not found";
}
std::string Node::Kill() {
    std::string ans;
    for (auto &child : children) {
        std::string msg = "kill";
        if (SendMessage(child.second.get(), msg)) {
            if (auto tmp = ReceiveMessage(child.second.get()); tmp.has_value()) {
                msg = *tmp;
            }
            if (ans.size() > 0) {
                ans = ans + " " + msg;
            } else {
                ans = msg;
            }
        }
        Unbind(child.second.get(), childrenPort[child.first]);
        child.second->close();
    }
    children.clear();
    childrenPort.clear();
    return ans;
}