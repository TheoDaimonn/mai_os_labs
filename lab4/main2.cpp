#include <iostream>
#include <dlfcn.h>

typedef int (*GCF_Func)(int, int);
typedef float (*Square_Func)(float, float);

int main() {
    void* handle_gcf = dlopen("./libgcf.so", RTLD_LAZY);
    void* handle_square = dlopen("./libsquare.so", RTLD_LAZY);

    if (!handle_gcf || !handle_square) {
        std::cerr << "Ошибка загрузки библиотек: " << dlerror() << std::endl;
        return 1;
    }

    GCF_Func GCF = (GCF_Func)dlsym(handle_gcf, "GCF_Euclid");
    Square_Func Square = (Square_Func)dlsym(handle_square, "Square_Rectangle");

    if (!GCF || !Square) {
        std::cerr << "Ошибка поиска символов: " << dlerror() << std::endl;
        return 1;
    }

    int choice;
    while (true) {
        std::cout << "Введите команду: ";
        std::cin >> choice;
        int flag = 1;
        if (choice == 0) {
            if (flag == 1){
            GCF = (GCF_Func)dlsym(handle_gcf, "GCF_Naive");
            Square = (Square_Func)dlsym(handle_square, "Square_Triangle");
            }
            else{
            GCF = (GCF_Func)dlsym(handle_gcf, "GCF_Euclid");
            Square = (Square_Func)dlsym(handle_square, "Square_Rectangle");
            }
            std::cout << "Реализация переключена." << std::endl;
        } else if (choice == 1) {
            int A, B;
            std::cin >> A >> B;
            std::cout << "НОД: " << GCF(A, B) << std::endl;
        } else if (choice == 2) {
            float A, B;
            std::cin >> A >> B;
            std::cout << "Площадь: " << Square(A, B) << std::endl;
        } else {
            std::cout << "Неверная команда!" << std::endl;
        }
    }

    dlclose(handle_gcf);
    dlclose(handle_square);
    return 0;
}