#include "Pi.h"

float Pi_Leibniz(int K) {
    float pi = 0.0f;
    int sign = 1;
    for (int i = 0; i < K; ++i) {
        pi += sign * 1.0f / (2 * i + 1);
        sign = -sign;
    }
    return pi * 4.0f;
}
