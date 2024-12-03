#include "gcf.hpp"

int GCF_Naive(int A, int B) {
    int min = (A < B) ? A : B;
    for (int i = min; i > 0; --i) {
        if (A % i == 0 && B % i == 0) {
            return i;
        }
    }
    return 1;
}