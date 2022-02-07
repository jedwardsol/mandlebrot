#pragma once
#include <complex>

std::complex<double>  fromPixel(int row, int column);
void go(std::complex<double> const &topLeft,std::complex<double> const &bottomRight);
void stop();
