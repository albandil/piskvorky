///////////////////////////////////////////////
// "Piškvorky"
// Jakub Benda (c) 2009

#ifndef _UTILS_H_
#define _UTILS_H_

#include "vec3.h"

// convenient function for square of a number
inline float _sqr(float x) { return x*x; }

// boundary check
template <typename T> bool isin(T x, T a, T b) { return a < x && x < b; }

#endif
