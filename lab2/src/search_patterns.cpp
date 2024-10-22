#include "search_patterns.hpp"
#include <algorithm>
#include <iostream>

struct SearchArgs {
    std::string text;
    std::string pattern;
    int start;
    int end;
    std::vector<int> *foundPositions;
    pthread_mutex_t *mutex;
};

void *searchPatternTask(void *input) {
    const auto &args = *(reinterpret_cast<SearchArgs *>(input));
    int n = args.text.length();
    int m = args.pattern.length();

    std::vector<int> localPositions;

    for (int i = args.start; i <= args.end - m; ++i) {
        int j;
        for (j = 0; j < m; ++j) {
            if (args.text[i + j] != args.pattern[j]) {
                break;
            }
        }
        if (j == m) {
            localPositions.push_back(i);
        }
    }

    // Защита добавления найденных позиций с использованием мьютекса
    pthread_mutex_lock(args.mutex);
    args.foundPositions->insert(args.foundPositions->end(), localPositions.begin(), localPositions.end());
    pthread_mutex_unlock(args.mutex);

    return nullptr;
}

std::vector<int> searchPattern(size_t threadQuantity, const std::string &text, const std::string &pattern) {
    std::vector<int> foundPositions;

    // Проверка на пустой текст или шаблон
    if (text.empty() || pattern.empty()) {
        return foundPositions;
    }

    if (threadQuantity >= 1 && text.length() >= pattern.length()) {
        const size_t actualThreadQuantity = std::min(threadQuantity, text.length());
        std::vector<pthread_t> threads(actualThreadQuantity);

        const size_t chunkSize = text.length() / actualThreadQuantity;
        const size_t remainder = text.length() % actualThreadQuantity;

        std::vector<SearchArgs> argsForThread(actualThreadQuantity);
        pthread_mutex_t mutex;
        pthread_mutex_init(&mutex, nullptr);

        for (size_t i = 0; i < actualThreadQuantity; ++i) {
            argsForThread[i].text = text;
            argsForThread[i].pattern = pattern;
            argsForThread[i].start = i * chunkSize;
            argsForThread[i].end = (i + 1) * chunkSize + pattern.length() - 1;
            argsForThread[i].foundPositions = &foundPositions;
            argsForThread[i].mutex = &mutex;

            if (i == actualThreadQuantity - 1) {
                argsForThread[i].end += remainder;
            }

            pthread_create(&threads[i], nullptr, searchPatternTask, reinterpret_cast<void *>(&argsForThread[i]));
        }

        for (auto &thread : threads) {
            pthread_join(thread, nullptr);
        }

        pthread_mutex_destroy(&mutex);
    } else {
        int n = text.length();
        int m = pattern.length();

        for (int i = 0; i <= n - m; ++i) {
            int j;
            for (j = 0; j < m; ++j) {
                if (text[i + j] != pattern[j]) {
                    break;
                }
            }
            if (j == m) {
                foundPositions.push_back(i);
            }
        }
    }

    return foundPositions;
}
