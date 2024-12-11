#ifndef GCF_H
#define GCF_H

#ifdef _WIN32
#define EXPORT __declspec(dllexport)
#else
#define EXPORT
#endif

EXPORT int GCF_Euclid(int A, int B);
EXPORT int GCF_Naive(int A, int B);

#endif // GCF_H
