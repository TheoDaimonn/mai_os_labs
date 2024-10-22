#include <gtest/gtest.h>
#include "search_patterns.hpp"
#include <string>
#include <vector>
#include <chrono>

// Тест для проверки корректности однопоточного варианта
TEST(PatternSearchTests, SingleThreadTest) {
    std::string text = "ababababa";
    std::string pattern = "aba";
    std::vector<int> expected = {0, 2, 4, 6};

    // Запускаем поиск с одним потоком
    std::vector<int> result = searchPattern(1, text, pattern);
    
    // Проверяем, что результаты совпадают
    ASSERT_EQ(result.size(), expected.size());
    EXPECT_EQ(result, expected);
}

// Тест для проверки, что результаты однопоточного и многопоточного вариантов совпадают
TEST(PatternSearchTests, MultiThreadConsistencyTest) {
    std::string text = "ababababa";
    std::string pattern = "aba";

    // Результат для однопоточного поиска
    std::vector<int> singleThreadResult = searchPattern(1, text, pattern);
    
    // Результат для многопоточного поиска
    std::vector<int> multiThreadResult = searchPattern(4, text, pattern);

    // Результаты должны совпадать
    ASSERT_EQ(singleThreadResult.size(), multiThreadResult.size());
    EXPECT_EQ(singleThreadResult, multiThreadResult);
}

// Тест для проверки, что многопоточный вариант быстрее однопоточного
TEST(PatternSearchTests, MultiThreadPerformanceTest) {
    std::string text(1e6, 'a');  // Длинный текст с 1,000,000 символов 'a'
    std::string pattern = "aaa";  // Шаблон для поиска

    // Измеряем время выполнения однопоточного поиска
    auto startSingle = std::chrono::high_resolution_clock::now();
    searchPattern(1, text, pattern);
    auto endSingle = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> singleElapsed = endSingle - startSingle;

    // Измеряем время выполнения многопоточного поиска
    auto startMulti = std::chrono::high_resolution_clock::now();
    searchPattern(4, text, pattern);  // Используем 4 потока
    auto endMulti = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> multiElapsed = endMulti - startMulti;

    // Проверяем, что многопоточный вариант быстрее (в общем случае)
    EXPECT_LT(multiElapsed.count(), singleElapsed.count());
}

// Тест для случая, когда шаблон длиннее текста (должен вернуть пустой результат)
TEST(PatternSearchTests, PatternLongerThanText) {
    std::string text = "short";
    std::string pattern = "this is a longer pattern";

    std::vector<int> result = searchPattern(2, text, pattern);
    
    // Результат должен быть пустым
    EXPECT_TRUE(result.empty());
}

// Тест для случая, когда текст пустой (должен вернуть пустой результат)
TEST(PatternSearchTests, EmptyText) {
    std::string text = "";
    std::string pattern = "non-empty";

    std::vector<int> result = searchPattern(2, text, pattern);
    
    // Результат должен быть пустым
    EXPECT_TRUE(result.empty());
}

// Тест для случая, когда шаблон пустой (должен вернуть пустой результат)
TEST(PatternSearchTests, EmptyPattern) {
    std::string text = "non-empty";
    std::string pattern = "";

    std::vector<int> result = searchPattern(2, text, pattern);
    
    // Результат должен быть пустым
    EXPECT_TRUE(result.empty());
}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
