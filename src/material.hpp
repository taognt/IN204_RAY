#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include "ray.hpp"
#include "hittalbe.hpp"

using std::shared_ptr; //As a result : many objects can share the same ptr (being a texture for example)
using std::make_shared;

struct data_hit;

class material {
    public:
        virtual bool scatter(const ray ray_in, data_hit& the_data_hit, color& attenuation, ray& ray_out) const = 0;
};

class metal : public material {
    public:
        metal(color a) : a_color(a) {}

        virtual bool scatter(const ray ray_in, data_hit& the_data_hit, color& attenuation, ray& ray_out) const override;

    public:
        color a_color;
};

bool metal::scatter(const ray ray_in, data_hit& the_data_hit, color& attenuation, ray& ray_out) const{
    vec reflected = reflection(unit_vect(ray_in.get_dir()), the_data_hit.normal_hit);
    ray_out = ray(the_data_hit.point_hit, reflected);
    attenuation = a_color;
    return (dot(ray_out.get_dir(), the_data_hit.normal_hit)>0);
}

class diffuse : public material{
    public:
        diffuse(const color& a_color) : the_color(a_color){}

    virtual bool scatter(const ray ray_in, data_hit& the_data_hit, color& attenuation, ray& ray_out) const override;

    public:
        color the_color;
};

bool diffuse::scatter(const ray ray_in, data_hit& the_data_hit, color& attenuation, ray& ray_out) const{
        auto scatter_direction = the_data_hit.normal_hit +random_vec_unit();

        if(scatter_direction.near_null()){
            scatter_direction = the_data_hit.normal_hit;
        }

        ray_out = ray(the_data_hit.point_hit, scatter_direction);
        attenuation = the_color;
        return true;
    }

#endif 