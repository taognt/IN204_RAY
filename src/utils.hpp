#ifndef UTILS_HPP
#define UTILS_HPP

#include <cmath>
#include <stdio.h>
#include <limits>
#include <memory>
#include <random>

#include "ray.hpp"
#include "vector.hpp"

// Definition of some useful functions or global variables/const

// Using

// Constants

const double pi = 3.141592653589793238462643;
const double infinity = std::numeric_limits<double>::infinity();
const double epsilon = 1e-4;

// Functions
double deg_to_rad(double deg);



#endif