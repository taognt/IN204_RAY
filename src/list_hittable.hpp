#ifndef LIST_HITTABLE_HPP
#define LIST_HITTABLE_HPP

#include "hittalbe.hpp"

#include <memory>
#include <vector>
#include <stdio.h>

using std::shared_ptr; //As a result : many objects can share the same ptr (being a texture for example)
using std::make_shared;

class list_hittable : public hittable {
    public:
        list_hittable(){}
        list_hittable(shared_ptr<hittable> object){add(object);}

        void clear(){objects.clear();}
        void add(shared_ptr<hittable> object){
            objects.push_back(object);
        }


        virtual bool hit(ray& r, double t_min, double t_max, data_hit& data) const override;
        

    public:
        std::vector<shared_ptr<hittable>> objects;
        std::vector<color> colors;
};


// Return true if any object is hit by the ray between t_min and t_max
bool list_hittable::hit(ray& r, double t_min, double t_max, data_hit& data) const {
    data_hit data_buffer;
    auto closest_t = t_max;
    bool hit_any = false;

    for(int i=0; i<objects.size();++i){
        auto object = objects[i];
        if(object->hit(r, t_min, closest_t, data_buffer)==true){
            if(data_buffer.t<closest_t){
                closest_t = data_buffer.t;
                data = data_buffer;
                hit_any = true;
            }
        }
    }
    return hit_any;
}


#endif
