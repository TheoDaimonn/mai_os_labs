#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include "search_patterns.hpp"

int main(int argc, char const *argv[]) {
    if (argc != 4) {
        std::cerr << "Использование: " << argv[0] << " <сколько потоков> <текст> <образец>" << std::endl;
        return 1;
    }

    unsigned long threadQuantity = std::atol(argv[1]);
    std::string text = argv[2];
    std::string pattern = argv[3];

    auto start = std::chrono::high_resolution_clock::now();

    std::vector<int> foundPositions = searchPattern(threadQuantity, text, pattern);

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    std::cout << "Время выполнения: " << elapsed.count() << " секунд" << std::endl;

    std::cout << "Найденные позиции: ";
    for (int pos : foundPositions) {
        std::cout << pos << " ";
    }
    std::cout << std::endl;

    return 0;
}