#ifndef SPHERE_HPP
#define SPHERE_HPP

#include "vector.hpp"
#include "hittalbe.hpp"
#include "ray.hpp"

class sphere : public hittable{
    private:
        Point center;
        double r; // rayon
        color the_color;

    public:
        sphere(){}
        sphere(Point the_point, double the_rayon, color _color):center(the_point), r(the_rayon), the_color(_color){}
        sphere(Point the_center, double the_rayon):center(the_center), r(the_rayon){}
        
        Point get_center(){return center;}
        double get_rayon(){return r;}

        virtual bool hit(ray r, double t_min, double t_max, data_hit& data) const override;
};

bool sphere::hit(ray the_ray, double t_min, double t_max, data_hit& data) const{
    Point orig = the_ray.get_origine();
    vec dir = the_ray.get_dir();

    vec oc = dir-center;
    double a = dot(orig, orig);
    double b = 2*dot(orig, oc);
    double c = dot(oc, oc)-r*r;

    auto delta = b*b-4*a*c;
    auto sqrtdelt = sqrt(delta);
    if(delta <0){return false;}

    auto root = (-b-sqrtdelt)/2*a;
    if(root<t_min || root>t_max){
        root = (-b+sqrtdelt)/2*a;
        if(root<t_min||root>t_max){
        return false;
        }
    }
    
    data.t = root;
    data.point_hit = the_ray.at(data.t);
    vec out_normal = (data.point_hit - center)/r;
    data.set_normal(the_ray, out_normal);

    return true;

   
}

#endif