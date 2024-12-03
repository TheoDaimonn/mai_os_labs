#include <gtest/gtest.h>
#include <dlfcn.h>
#include <iostream>

// Прототипы функций
typedef int (*GCF_Func)(int, int);
typedef float (*Square_Func)(float, float);

// Тесты для функции GCF
TEST(GCFTest, DynamicLoading) {
    void* handle_gcf = dlopen("./libgcf.so", RTLD_LAZY);
    ASSERT_TRUE(handle_gcf != nullptr) << "Ошибка загрузки libgcf.so: " << dlerror();

    GCF_Func GCF_Euclid = (GCF_Func)dlsym(handle_gcf, "GCF_Euclid");
    ASSERT_TRUE(GCF_Euclid != nullptr) << "Ошибка поиска символа GCF_Euclid: " << dlerror();

    GCF_Func GCF_Naive = (GCF_Func)dlsym(handle_gcf, "GCF_Naive");
    ASSERT_TRUE(GCF_Naive != nullptr) << "Ошибка поиска символа GCF_Naive: " << dlerror();

    // Тесты для GCF_Euclid
    EXPECT_EQ(GCF_Euclid(10, 5), 5);
    EXPECT_EQ(GCF_Euclid(25, 15), 5);
    EXPECT_EQ(GCF_Euclid(14, 21), 7);
    EXPECT_EQ(GCF_Euclid(100, 10), 10);

    // Тесты для GCF_Naive
    EXPECT_EQ(GCF_Naive(10, 5), 5);
    EXPECT_EQ(GCF_Naive(25, 15), 5);
    EXPECT_EQ(GCF_Naive(14, 21), 7);
    EXPECT_EQ(GCF_Naive(100, 10), 10);

    // Переключение реализации
    GCF_Func current_gcf = GCF_Naive;
    EXPECT_EQ(current_gcf(10, 5), 5);
    EXPECT_EQ(current_gcf(25, 15), 5);
    EXPECT_EQ(current_gcf(14, 21), 7);
    EXPECT_EQ(current_gcf(100, 10), 10);

    current_gcf = GCF_Euclid;
    EXPECT_EQ(current_gcf(10, 5), 5);
    EXPECT_EQ(current_gcf(25, 15), 5);
    EXPECT_EQ(current_gcf(14, 21), 7);
    EXPECT_EQ(current_gcf(100, 10), 10);

    dlclose(handle_gcf);
}

// Тесты для функции Square
TEST(SquareTest, DynamicLoading) {
    void* handle_square = dlopen("./libsquare.so", RTLD_LAZY);
    ASSERT_TRUE(handle_square != nullptr) << "Ошибка загрузки libsquare.so: " << dlerror();

    Square_Func Square_Rectangle = (Square_Func)dlsym(handle_square, "Square_Rectangle");
    ASSERT_TRUE(Square_Rectangle != nullptr) << "Ошибка поиска символа Square_Rectangle: " << dlerror();

    Square_Func Square_Triangle = (Square_Func)dlsym(handle_square, "Square_Triangle");
    ASSERT_TRUE(Square_Triangle != nullptr) << "Ошибка поиска символа Square_Triangle: " << dlerror();

    // Тесты для Square_Rectangle
    EXPECT_FLOAT_EQ(Square_Rectangle(10, 5), 50);
    EXPECT_FLOAT_EQ(Square_Rectangle(25, 15), 375);
    EXPECT_FLOAT_EQ(Square_Rectangle(14, 21), 294);
    EXPECT_FLOAT_EQ(Square_Rectangle(100, 10), 1000);

    // Тесты для Square_Triangle
    EXPECT_FLOAT_EQ(Square_Triangle(10, 5), 25);
    EXPECT_FLOAT_EQ(Square_Triangle(25, 15), 187.5);
    EXPECT_FLOAT_EQ(Square_Triangle(14, 21), 147);
    EXPECT_FLOAT_EQ(Square_Triangle(100, 10), 500);

    // Переключение реализации
    Square_Func current_square = Square_Triangle;
    EXPECT_FLOAT_EQ(current_square(10, 5), 25);
    EXPECT_FLOAT_EQ(current_square(25, 15), 187.5);
    EXPECT_FLOAT_EQ(current_square(14, 21), 147);
    EXPECT_FLOAT_EQ(current_square(100, 10), 500);

    current_square = Square_Rectangle;
    EXPECT_FLOAT_EQ(current_square(10, 5), 50);
    EXPECT_FLOAT_EQ(current_square(25, 15), 375);
    EXPECT_FLOAT_EQ(current_square(14, 21), 294);
    EXPECT_FLOAT_EQ(current_square(100, 10), 1000);

    dlclose(handle_square);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}