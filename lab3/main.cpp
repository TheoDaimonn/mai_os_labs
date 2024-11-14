
#include "include/parent.hpp"
#include "include/utils.hpp"
#include <cstdlib>
#include <iostream>
#include <stdexcept>

int main() {
    // export PATH_TO_CHILD1="/home/ivzarru/os_labs/lab3/build/child1"
    // export PATH_TO_CHILD2="/home/ivzarru/os_labs/lab3/build/child2"
    const char* pathToChild1 = std::getenv("PATH_TO_CHILD1");
    const char* pathToChild2 = std::getenv("PATH_TO_CHILD2");

    if (!pathToChild1) {
        std::cerr << "Error: Environment variable PATH_TO_CHILD1 is not set." << std::endl;
        return EXIT_FAILURE;
    }
    if (!pathToChild2) {
        std::cerr << "Error: Environment variable PATH_TO_CHILD2 is not set." << std::endl;
        return EXIT_FAILURE;
    }

    try {
        ParentProcess(pathToChild1, pathToChild2);
    } catch (const std::runtime_error& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}