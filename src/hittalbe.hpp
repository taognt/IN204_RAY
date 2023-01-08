#ifndef HITTABLE_HPP
#define HITTABLE_HPP

#include "vector.hpp"
#include "ray.hpp"

//We store the data of hit points
struct data_hit{
    Point point_hit;
    vec normal_hit;
    double t;

    bool front_face; //True if the object's face crossed is the front one (the ray comes front the outside of the object)
    inline void set_normal(ray the_ray, const vec& out_normal){ //The normal_hit always points outside the object
        front_face = dot(the_ray.get_dir(), out_normal);
        normal_hit = front_face ? out_normal : - out_normal;
    }
};

class hittable{
    public:
        virtual bool hit(ray r, double t_min, double t_max, data_hit& data) const = 0 ;
};


#endif