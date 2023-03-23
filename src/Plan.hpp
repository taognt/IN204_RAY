#ifndef PLAN_HPP
#define PLAN_HPP

#include "vector.hpp"
#include "hittalbe.hpp"
#include "ray.hpp"
#include "list_hittable.hpp"

class plan : public hittable{
    private:
        vec normal; 
        Point origin;
        color the_color;

    public:
        plan(){}
        plan(vec norm, Point origin_, color _color):normal(norm),origin(origin_), the_color(_color){}
        plan(vec norm, Point origin_):normal(norm), origin(origin_){}
        
        Point get_origin(){return origin;}
        vec get_normal(){return normal;}
        color get_color(){return the_color;}

        virtual bool hit(ray& r, double t_min, double t_max, data_hit& data) const override;
};

bool plan::hit(ray& the_ray, double t_min, double t_max, data_hit& data) const{
    data_hit data_buffer;
    double t;

    //Set the color:
    data_buffer.the_color = the_color;

    //ray
    vec dir = the_ray.get_dir();                  
    Point origin_ray = the_ray.get_origine();


    double a = normal.getx();
    double b = normal.gety();
    double c = normal.getz();

    double x = dir.getx();
    double y = dir.gety(); 
    double z = dir.getz();
    double ox = origin.getx();
    double oy =  origin.gety();
    double oz =  origin.getz();

    auto diff_or = a*(origin_ray.getx()-ox)+b*(origin_ray.gety()-oy)+c*(origin_ray.getz()-oz);
    auto prod = a*x+b*y+c*z;

    if(prod!=0){
        t = -diff_or/prod;
        if(t<t_min || t_max <t){
            return false;
        }
    }
    else{
        if(diff_or==0){
            t=0;
            return true;
        }
    }

    data_buffer.t = t;
    data_buffer.point_hit = the_ray.at(t);
    data_buffer.normal_hit = normal;
    data_buffer.front_face = true;
    data = data_buffer;

    return true;
    
}

#endif