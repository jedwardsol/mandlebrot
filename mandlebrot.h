#pragma once
#include <complex>

using Point = std::complex<long double>;

Point  fromPixel(int row, int column);
void go(Point const &topLeft,Point const &bottomRight);
void stop();
