#pragma once

#include <string>
#include <vector>
#include <atomic>
#include <pthread.h>

void *searchPatternTask(void *input);

std::vector<int> searchPattern(size_t threadQuantity, const std::string &text, const std::string &pattern);