#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include "ray.hpp"
#include "hittalbe.hpp"

using std::shared_ptr; //As a result : many objects can share the same ptr (being a texture for example)
using std::make_shared;

struct data_hit;

class material {
    public:
        virtual bool scatter(ray ray_in, data_hit& the_data_hit, color& attenuation, ray& ray_out) const = 0;
};

class metal : public material {
    public:
        metal(color a) : a_color(a) {}

        virtual bool scatter(const ray& r_in, data_hit& data, color& attenuation, ray& scattered) const override ;

    public:
        color a_color;
};

bool metal::scatter(const ray& r_in, data_hit& data, color& attenuation, ray& scattered) const{
    vec reflected = reflection(unit_vect(r_in.get_dir()), data.normal_hit);
    scattered = ray(data.point_hit, reflected);
    attenuation = a_color;
    return (dot(scattered.get_dir(), data.normal_hit)>0);
}

// class diffuse : public material{
//     public:
//         diffuse(const color& a_color) : the_color(a_color){}

//     virtual bool scatter(const ray& r_in, data_hit &data, color& attenuation, ray& scattered) const override{
//         auto scatter_direction = data.normal_hit +random_vec_unit();

//         if(scatter_direction.near_null()){
//             scatter_direction = data.normal_hit;
//         }

//         scattered = ray(data.point_hit, scatter_direction);
//         attenuation = the_color;
//         return true;
//     }

//     public:
//         color the_color;
// };

#endif 