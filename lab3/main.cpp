#include "include/parent.hpp"
#include "include/utils.hpp"
#include <cstdlib>
#include <iostream>
#include <stdexcept>

int main() {
    // Retrieve paths to child executables from environment variables
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

    // Prompt user for input
    std::string userInput;
    std::cout << "Enter a string to transform: ";
    std::getline(std::cin, userInput);

    try {
        ParentProcess(pathToChild1, pathToChild2, userInput);
    } catch (const std::runtime_error& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
