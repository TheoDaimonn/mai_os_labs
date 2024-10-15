#include "search_patterns.hpp"

struct SearchArgs {
    std::string text;
    std::string pattern;
    int start;
    int end;
    std::vector<int> *foundPositions;
};

void *searchPatternTask(void *input) {
    const auto &args = *(reinterpret_cast<SearchArgs *>(input));
    int n = args.text.length();
    int m = args.pattern.length();

    for (int i = args.start; i <= args.end - m; ++i) {
        int j;
        for (j = 0; j < m; ++j) {
            if (args.text[i + j] != args.pattern[j]) {
                break;
            }
        }
        if (j == m) {
            args.foundPositions->push_back(i);
        }
    }
    return nullptr;
}

std::vector<int> searchPattern(size_t threadQuantity, const std::string &text, const std::string &pattern) {
    std::vector<int> foundPositions;

    if (threadQuantity >= 1) {
        const size_t actualThreadQuantity = std::min(threadQuantity, text.length());
        std::vector<pthread_t> threads(actualThreadQuantity);

        const size_t chunkSize = text.length() / actualThreadQuantity;
        const size_t remainder = text.length() % actualThreadQuantity;

        std::vector<SearchArgs> argsForThread(actualThreadQuantity);

        for (size_t i = 0; i < actualThreadQuantity; ++i) {
            argsForThread[i].text = text;
            argsForThread[i].pattern = pattern;
            argsForThread[i].start = i * chunkSize;
            argsForThread[i].end = (i + 1) * chunkSize + pattern.length() - 1;
            argsForThread[i].foundPositions = &foundPositions;

            if (i == actualThreadQuantity - 1) {
                argsForThread[i].end += remainder;
            }

            pthread_create(&threads[i], nullptr, searchPatternTask, reinterpret_cast<void *>(&argsForThread[i]));
        }

        for (auto &thread : threads) {
            pthread_join(thread, nullptr);
        }
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