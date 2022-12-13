#ifndef SPHERE_HPP
#define SPHERE_HPP

#include "vector.hpp"

class sphere{
    private:
        Point center;
        double r; // rayon
        color the_color;

    public:
        sphere(Point the_point, double the_rayon, color _color):center(the_point), r(the_rayon), the_color(_color){}
        Point get_center(){return center;}
        double get_rayon(){return r;}
};

#endif