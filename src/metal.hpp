#ifndef METAL_HPP
#define METAL_HPP

#include "material.hpp"


class metal : public material {
    public:
        metal(){}
        metal(color a) : a_color(a) {}

        virtual bool scatter(const ray& r_in, data_hit& data, color& attenuation, ray& scattered) const

    public:
        color a_color;
};

bool metal::scatter(const ray& r_in, data_hit& data, color& attenuation, ray& scattered) const  {
            vec reflected = reflection(unit_vect(r_in.get_dir()), data.normal_hit);
            scattered = ray(data.point_hit, reflected);
            attenuation = a_color;
            return (dot(scattered.get_dir(), data.normal_hit)>0);
        }

#endif