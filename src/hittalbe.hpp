#ifndef HITTABLE_HPP
#define HITTABLE_HPP

#include "vector.hpp"
#include "ray.hpp"
#include <memory>
#include <vector>
#include <stdio.h>


using std::shared_ptr;
using std::make_shared;

class material;
//We store the data of hit points
struct data_hit{
    public:
        Point point_hit; // P(t) = origin + t*direction
        vec normal_hit; // Normal to the object in the hit point
        double t;
        bool front_face; //True if the object's face crossed is the front one (the ray comes front the outside of the object)
        color the_color;
        shared_ptr<material> mat_ptr; //Shared pointer of the material



        inline void set_normal(ray& the_ray, const vec& out_normal){ //The normal_hit always points outside the object
            //front_face = dot(the_ray.get_dir(), out_normal)<0;
            //normal_hit = front_face ? out_normal : - out_normal;
            if(dot(the_ray.get_dir(), out_normal)>0){
                normal_hit = -out_normal;
                front_face = false;
            }
            else{
                normal_hit = out_normal;
                front_face = true;
            }
        }

};

class hittable{
    public:   
        virtual bool hit(ray& r, double t_min, double t_max, data_hit& data) const = 0;
};


#endif