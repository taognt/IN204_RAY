#ifndef CAMERA_H
#define CAMERA_H

#include "vector.hpp"
#include "ray.hpp"

class camera{
    public:
        //Constructors
        camera(){
        const auto ratio = 16.0/9.0;
        auto view_height = 2.0;
        auto view_width = ratio*view_height;
        auto focal = 1.0;

        origin = Point(0.0,0.0,0.0);
        hor = vec(view_width,0,0);
        vert = vec(0,view_height,0);
        lower_left_corner = origin - hor/2 - vert/2 - vec(0, 0, focal);
        }

        camera(float ratio_1, float ratio_2, float view_height, float focal){ //default origin at 0 0 0
        auto ratio = ratio_1/ratio_2;
        auto view_width = ratio*view_height;

        origin = Point(0.0,0.0,0.0);
        hor = vec(view_width,0,0);
        vert = vec(0,view_height,0);
        lower_left_corner = origin - hor/2 - vert/2 - vec(0, 0, focal);
        }

        camera(vec origin_, float ratio, float view_height, float focal){
        auto view_width = ratio*view_height;
        origin = origin_;
        hor = vec(view_width,0,0);
        vert = vec(0,view_height,0);
        lower_left_corner = origin - hor/2 - vert/2 - vec(0, 0, focal);
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