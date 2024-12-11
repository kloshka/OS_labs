#ifndef PI_H
#define PI_H

#ifdef _WIN32
#define EXPORT __declspec(dllexport)
#else
#define EXPORT
#endif

EXPORT float Pi_Leibniz(int K);
EXPORT float Pi_Wallis(int K);

#endif // PI_H
