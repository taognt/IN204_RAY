#ifndef CAMERA_H
#define CAMERA_H

#include "vector.hpp"
#include "ray.hpp"

class camera{
    public:
        camera(){
        const auto ratio = 16.0/9.0;
        auto view_height = 2.0;
        auto view_width = ratio*view_height;
        auto focal = 1.0;

        Point origin = Point(0.0,0.0,0.0);
        auto hor = vec(view_width,0,0);
        auto vert = vec(0,view_height,0);
        vec lower_left_corner = origin - hor/2 - vert/2 - vec(0, 0, focal);
        }

        ray get_ray(double u, double v) const{
            ray r(origin, lower_left_corner + u*hor+v*vert);
            return r;
        }

        Point get_origin(){return origin;}

    private:
        Point origin;
        Point lower_left_corner;
        vec hor;
        vec vert;
    


};

#endif