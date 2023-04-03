#ifndef SPHERE_HPP
#define SPHERE_HPP

#include "vector.hpp"
#include "hittalbe.hpp"
#include "ray.hpp"
#include "list_hittable.hpp"
#include "utils.hpp"

class sphere : public hittable{
    private:
        Point center;
        double r; // radius
        shared_ptr<material> the_material; //Color is inside

    public:
        sphere(){};
        sphere(Point the_center, double the_rayon, shared_ptr<material> mat):center(the_center), r(the_rayon), the_material(mat){};
        
        Point get_center(){return center;}
        double get_rayon(){return r;}

        virtual bool hit(ray& r, double t_min, double t_max, data_hit& data) const override;
};

bool sphere::hit(ray& the_ray, double t_min, double t_max, data_hit& data) const{
    bool test_1 = true; // test if root is a root of the equation
    bool test_2 = false; // make sure the hit point is on the sphere
    data_hit data_buffer;
    double result;

    //temporary


    //Set the color:
    //data_buffer.the_color = the_color;

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

    
    if(delta<epsilon){
        return false;
        std::cerr<<"delta < 0"<<std::endl;
    }
    else{

        //find the nearest root
        auto root = (-b-sqrt(delta))/(2*a);
        
        //std::cerr<<"delta : "<<delta<<"\nroot : "<<root<<"\nt_max : "<<t_max<<std::endl;
        if(root<t_min || t_max <root){
            root = (-b+sqrt(delta))/(2*a);
            if(root<t_min || t_max <root){
                return false;
            }
        }

        // test if root is a root of the equation
        if (test_1){
            result = a*root*root+b*root+c;
            if(result <epsilon){
            }
            else{
                std::cerr<<"ERROR. Root is not a root"<<std::endl;
                std::cerr<<"at2+bt+c = "<<(result)<<std::endl;
            }
        }

        data_buffer.t = root;
        data_buffer.point_hit = the_ray.at(root); //OK
        vec out_normal = (data_buffer.point_hit - center)/r;
        data_buffer.set_normal(the_ray, out_normal); //OK
        data_buffer.mat_ptr = the_material;
        data = data_buffer;

        // ces deux valeurs doivent être égales :
        if(test_2){
            std::cerr<<"rayon sphere : "<<r<<std::endl;
            std::cerr<<"distance P(t) - centre : ";
            std::cerr<< (data.point_hit-center).lenght()<<"\n"<<std::endl; 
        }
        return true;
    }
    //return false;
}

#endif