#include "vector.hpp"
#include "color.hpp"
#include "ray.hpp"
#include "sphere.hpp"
#include "hittalbe.hpp"
#include "list_hittable.hpp"
#include "Plan.hpp"
#include "camera.hpp"
#include <iostream>
#include <chrono>
#include <fstream>
#include "material.hpp"
#include<omp.h>
#include "metal.hpp"


#include "utils.hpp"

color BG_color(1.0,1.0,1.0);  // Color of the background


double is_hit(ray the_ray, sphere the_sphere){     // the_ray = a+t*b
    vec dir = the_ray.get_dir();                  
    Point origin = the_ray.get_origine();
    Point center = the_sphere.get_center();
    vec or_cent = origin-center; // origin - center
    double a = dot(dir,dir);
    double b = 2*dot(or_cent,dir);
    double c = dot(or_cent, or_cent) - the_sphere.get_rayon()*the_sphere.get_rayon();
    double delta = b*b - 4*a*c;

    //return(delta >=0);
    auto root = (-b-sqrt(delta))/(2*a);
    if(delta>=0){
        return(root); 
    }
    else{
        return(-1);
    }
}




// FONCTION MODIFÉE
color ray_color(ray& the_ray, const list_hittable& Shapes, int depth){

    if(depth<=0){
        return color(0.0,0.0,0.0);
    } 

    data_hit rec;
    if(Shapes.hit(the_ray, 0.001, infinity, rec)){
        ray scattered;
        color attenuation;
        if(rec.mat_ptr->scatter(the_ray, rec, attenuation, scattered)){
            return attenuation*ray_color(scattered, Shapes, depth-1);
        }

        else{
            return BG_color;
        }
    }
    return BG_color;
}


// Creation d'une image au format PPM, exemple
int main(){
    //stream definition :
    std::ofstream myfile;
    myfile.open("../images/image_720_2.ppm");


    const auto ratio = 16.0/9.0;
    const int width = 1920;
    const int height = static_cast<int>(width/ratio);
    std::cerr<<"\nHeight: "<<height<<"\n"<<std::flush;

    //camera
    camera the_camera;
    auto view_height = 2.0;
    auto view_width = ratio*view_height;
    std::cerr<<"\nView_width : "<<view_width<<"\n"<<std::flush;
    auto focal = 1.0;

    Point origin = Point(0.0,0.0,0.0);
    auto hor = vec(view_width,0,0);
    auto vert = vec(0,view_height,0);
    vec lower_left_corner = origin - hor/2 - vert/2 - vec(0, 0, focal);


    myfile<<"P3\n"<<width<<" "<<height<<"\n255\n";

    //materials
    //auto material_floor = make_shared<diffuse>(color(0.7,0.6,0.1));
    color color_1(0.7,0.0,0.1);
    color color_2(0.6,0.6,0.6);
    color color_3(1,0.7,0.75);
    color color_4(0.89,0.82,0.45);
    auto material_sphere_1 = make_shared<diffuse>(color_1);
    auto material_sphere_2 = make_shared<metal>(color_2);
    auto material_sphere_3 = make_shared<metal>(color_4);
    auto material_plan = make_shared<diffuse>(color_3);

    // Ajout d'une sphère
    double rayon = 4;
    Point center_sphere(0.0,0.0,-15.0);
    color the_sphere_color(8.0,0.0,0.0);
    sphere the_sphere(center_sphere, rayon, material_sphere_1);

    // Ajout d'une 2e sphère
    double rayon_2 = 5;
    Point center_sphere_2(10.0,0.0,-11.0);
    color the_sphere_color_2(3.0,2.0,0.0);
    sphere the_sphere_2(center_sphere_2, rayon_2,material_sphere_2);  

    // Ajout d'un plan 
    vec the_normal(0.3,0.3,0.3);
    the_normal = unit_vect(the_normal);
    //Point the_origin(0.0,-20.0,-10);
    Point the_origin(4.0,0.0,-15.0);
    color the_plan_color(0.0,2.0,0.0);
    plan the_plan(the_normal, the_origin, material_plan);

    //List of hittable
    list_hittable Shapes;
    Shapes.add(make_shared<sphere> (Point(0.0,0.0,-15.0), 4.0, material_sphere_1));
    Shapes.add(make_shared<sphere> (Point(10.0,0.0,-11.0), 5.0, material_sphere_2));
    Shapes.add(make_shared<sphere> (Point(4.0,4.0,-11.0), 3.0, material_sphere_3));
    Shapes.add(make_shared<plan> (the_plan));
    
    std::cerr<<"Nbr objects : "<<Shapes.objects.size()<<std::endl;

    int max_depth = 10;
    int number_of_sample = 25;

    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();

    for(int j = height-1; j>=0; j--){
        //Progressindicatorto the error output
       // std::cerr<<"\n Lines remaining: "<<j<<' '<<std::flush;

        // Parralelisons le parcours en longueur : (a faire plus tard)
        for(int i= 0; i<width; i++){
            color pixel_color(0.0,0.0,0.0);
            //#pragma omp parallel for
            for(int k=0; k<number_of_sample; ++k){
                auto u = double(i+random_double()) / (width-1);
                auto v = double(j+random_double()) / (height-1);
                ray r = the_camera.get_ray(u,v);
                pixel_color = pixel_color + ray_color(r, Shapes, max_depth);
            }
            
            write_color(myfile,pixel_color, number_of_sample);


        }
    }
    end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end-start;
    std::cerr << "\nTemps calcul image : " << elapsed_seconds.count() <<" seconds"<< std::endl;
    std::cerr<<"\nDone.\n";

    myfile.close();
    return EXIT_SUCCESS;
}