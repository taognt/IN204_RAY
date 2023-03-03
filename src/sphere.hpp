#ifndef SPHERE_HPP
#define SPHERE_HPP

#include "vector.hpp"
#include "hittalbe.hpp"
#include "ray.hpp"
#include "list_hittable.hpp"

class sphere : public hittable{
    private:
        Point center;
        double r; // radius
        color the_color;

    public:
        sphere(){}
        sphere(Point the_point, double the_rayon, color _color):center(the_point), r(the_rayon), the_color(_color){}
        sphere(Point the_center, double the_rayon):center(the_center), r(the_rayon){}
        
        Point get_center(){return center;}
        double get_rayon(){return r;}

        virtual bool hit(ray& r, double t_min, double t_max, data_hit& data) const override;
};

bool sphere::hit(ray& the_ray, double t_min, double t_max, data_hit& data) const{
    data_hit data_buffer;

    vec dir = the_ray.get_dir();                  
    Point origin = the_ray.get_origine();
    vec or_cent = origin-center; // direction - center

    //P(t) = dir*t + origin
    double a = dot(dir,dir);
    double b = 2*dot(or_cent,dir);
    double c = dot(or_cent, or_cent) - r*r;
    double delta = b*b - 4*a*c;



    /* data.t = root;
    data.point_hit = the_ray.at(data.t);
    vec out_normal = (data.point_hit - center)/r;
    data.set_normal(the_ray, out_normal);  */

    if(delta<0){
        return false;
    }

    else{

        //find the nearest root
        auto root = (-b-sqrt(delta))/2*a;
        
        //std::cerr<<"delta : "<<delta<<"\nroot : "<<root<<"\nt_max : "<<t_max<<std::endl;
        if(root<t_min || t_max <root){
            root = (-b+sqrt(delta))/2*a;
            if(root<t_min || t_max <root){
                return false;
            }
        }
        data_buffer.t = root;
        data_buffer.point_hit = the_ray.at(root); // NON
        vec out_normal = (data_buffer.point_hit - center)/r;
        data_buffer.set_normal(the_ray, out_normal); //OK??
        data = data_buffer;


        // cesdeux valeurs doivent être égales :
        std::cerr<<"r : "<<r<<std::endl;
        std::cerr<<"distance P(t) - centre :\n"<<std::endl;
        std::cerr<< (data.point_hit-center).lenght()<<"\n"<<std::endl;
    }

    

    
    
   

   
}

#endif