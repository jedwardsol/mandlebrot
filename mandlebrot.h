#pragma once
#include <complex>
#include <boost/multiprecision/cpp_complex.hpp>


//using Point = std::complex<double>;
//using Point = boost::multiprecision::cpp_complex_double;
using Point = boost::multiprecision::cpp_complex_quad;

Point  fromPixel(int row, int column);
void go(Point const &topLeft,Point const &bottomRight);
void stop();
