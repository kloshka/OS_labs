#include "GCF.h"

int GCF_Naive(int A, int B) {
    int gcf = 1;
    for (int i = 2; i <= (A < B ? A : B); ++i) {
        if (A % i == 0 && B % i == 0) {
            gcf = i;
        }
    }
    return gcf;
}
