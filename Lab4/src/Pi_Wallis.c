#include "Pi.h"

float Pi_Wallis(int K) {
    float pi = 1.0f;
    for (int i = 1; i <= K; ++i) {
        pi *= (2.0f * i) / (2 * i - 1);
        pi *= (2.0f * i) / (2 * i + 1);
    }
    return pi * 2.0f;
}
