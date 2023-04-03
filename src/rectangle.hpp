#ifndef RECT_HPP
#define RECT_HPP

#include "vector.hpp"
#include "hittalbe.hpp"
#include "ray.hpp"
#include "list_hittable.hpp"

class rectangle : public hittable{
    private:
        vec plage_inf;
        vec plage_sup;
        Point corner; /// min of each plage 
        color the_color;

    public:
        rectangle(){}
        rectangle(vec plage_inf_,vec plage_sup_, Point corner_, color _color):plage_inf(plage_inf_),plage_sup(plage_sup_),corner(corner_), the_color(_color){}
        rectangle(vec plage_inf_,vec plage_sup_, Point corner_):plage_inf(plage_inf_),plage_sup(plage_inf_), corner(corner_){}
        
        Point get_corner(){return corner;}
        vec get_plage_sup(){return plage_sup;}
        vec get_plage_inf(){return plage_inf;}
        color get_color(){return the_color;}

        virtual bool hit(ray& r, double t_min, double t_max, data_hit& data) const override;
};

bool rectangle::hit(ray& the_ray, double t_min, double t_max, data_hit& data) const{
    data_hit data_buffer;
    double t;

    //Set the color:
    data_buffer.the_color = the_color;

    //ray
    vec dir = the_ray.get_dir();                  
    Point origine_ray = the_ray.get_origine();

    //Getting inf bounds
    double x_inf = plage_inf.getx();
    double y_inf= plage_inf.gety();
    double z_inf = plage_inf.getz();

    //Getting sup bounds
    double x_sup = plage_sup.getx();
    double y_sup= plage_sup.gety();
    double z_sup = plage_sup.getz();

    double x = dir.getx();
    double y = dir.gety(); 
    double z = dir.getz();
    double ox = corner.getx();
    double oy =  corner.gety();
    double oz =  corner.getz();

    t = (z_sup - origine_ray.getz())/(dir.getz()); // change the inf ant treats the orientation later
    //print the t
    //std::cerr<<"\n Box t : "<<t<<std::endl;

    if(the_ray.at(t).getx() < x_inf || the_ray.at(t).getx() > x_sup || the_ray.at(t).gety() < y_inf || the_ray.at(t).gety() > y_sup){
            //std::cerr<<"False : returnd for Box ray"<<std::endl;
            return false;
    }
    std::cerr<<"True : returnd for Box ray"<<std::endl;
    //set all the data
    data_buffer.t = t;
    data_buffer.point_hit = the_ray.at(t);
    //calculating the normal
    vec out_normal = vec(0,0,1);
    data_buffer.set_normal(the_ray, out_normal);
    data = data_buffer;

    return true;
    
}

#endif