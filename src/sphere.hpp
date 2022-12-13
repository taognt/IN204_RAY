#ifndef SPHERE_HPP
#define SPHERE_HPP

#include "vector.hpp"

class sphere{
    private:
        Point center;
        double r; // rayon

    public:
        Point get_center(){return center;}
        double get_rayon(){return r;}
};

#endif