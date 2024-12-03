#include <iostream>
#include "gcf.hpp"
#include "square.hpp"

int main() {
    int choice;
    while (true) {
        std::cout << "Введите команду: ";
        std::cin >> choice;

        if (choice == 1) {
            int A, B;
            std::cin >> A >> B;
            std::cout << "НОД (алгоритм Евклида): " << GCF_Euclid(A, B) << std::endl;
        } else if (choice == 2) {
            float A, B;
            std::cin >> A >> B;
            std::cout << "Площадь прямоугольника: " << Square_Rectangle(A, B) << std::endl;
        } else {
            std::cout << "Неверная команда!" << std::endl;
        }
    }
    return 0;
}