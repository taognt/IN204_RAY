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

#include<omp.h>


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

// color ray_color_the_sphere(ray the_ray, sphere the_sphere){
//     data_hit rec;
//     if(the_sphere.hit(the_ray, 0, infinity, rec) && ((rec.point_hit - the_sphere.get_center()).lenght() < the_sphere.get_rayon()) ){
//         return 0.5*(rec.normal_hit+color(1,1,1));
//     }
//     else{
//         return BG_color;
//     }
// } 

// color ray_color(ray& the_ray, const list_hittable& Shapes){
//     data_hit rec;
//     if(Shapes.hit(the_ray, 0, infinity, rec)){
//         auto t_hit = rec.t;
//         color the_color = rec.the_color;
//         //std::cerr<<"\nt_hit : "<<t_hit<<std::endl; 
//         //std::cerr<<"\nnormal hit : "<<rec.normal_hit<<std::endl;
//         //return the_color + 0.5*(rec.normal_hit+color(1,1,1));
//         return 0.5*(rec.normal_hit+color(1,1,1));

//     }
//     else{
//         return BG_color;
//     }

// }



// FONCTION MODIFÉE
color ray_color(ray& the_ray, const list_hittable& Shapes, int depth){

    if(depth<=0){
        return color(0.0,0.0,0.0);
    } 

    data_hit rec;
    if(Shapes.hit(the_ray, 0.001, infinity, rec)){
        auto t_hit = rec.t;
        color the_color = rec.the_color;
        Point target = rec.point_hit + rec.normal_hit + random_vec_unit();
        //vec new_vec target-rec.point_hit;
       //std::cerr<<"\nt_hit : "<<t_hit<<std::endl; 
        //std::cerr<<"\nnormal hit : "<<rec.normal_hit<<std::endl;
        //return the_color + 0.5*(rec.normal_hit+color(1,1,1));
        //return 0.5*(rec.normal_hit+color(1,1,1));
        ray ray2(rec.point_hit, target - rec.point_hit);
        return 0.5*ray_color(ray2, Shapes, depth-1)+0.2*(rec.normal_hit+color(1,0.3,0.7));


    }
    else{
        return BG_color;
    }

}

// Function to draw the picture, change "the_sphere" to a list of hittable
// void draw_picture(int i,int j,const int width,const int height, auto hor, auto vert, vec lower_left_corner,vec origin,list_hittable Shapes, int max_depth){
//     auto u = double(i) / (width-1);
//     auto v = double(j) / (height-1);
//     vec direction = lower_left_corner + u*hor+v*vert;//- origin;
//     ray r(origin, direction);
//     color the_color;// = ray_color(r);
//     the_color = ray_color(r, Shapes, max_depth);
//     write_color(std::cout,the_color);
// }

// void draw_picture_the_sphere(int i,int j,const int width,const int height, auto hor, auto vert, vec lower_left_corner,vec origin,sphere the_sphere){
//     auto u = double(i) / (width-1);
//     auto v = double(j) / (height-1);
//     vec direction = lower_left_corner + u*hor+v*vert - origin;
//     ray r(origin, direction);
//     color the_color;// = ray_color(r);
//     the_color = ray_color_the_sphere(r, the_sphere);
//     write_color(std::cout,the_color);
// }

// Creation d'une image au format PPM, exemple
int main(){
    //stream definition :
    std::ofstream myfile;
    myfile.open("image_720.ppm");


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
    // Ajout d'une sphère
    double rayon = 4;
    Point center_sphere(0.0,0.0,-15.0);
    color the_sphere_color(8.0,0.0,0.0);
    sphere the_sphere(center_sphere, rayon,the_sphere_color);

    // Ajout d'une 2e sphère
    double rayon_2 = 5;
    Point center_sphere_2(10.0,0.0,-11.0);
    color the_sphere_color_2(3.0,2.0,0.0);
    sphere the_sphere_2(center_sphere_2, rayon_2,the_sphere_color_2);  

    // Ajout d'un plan 
    vec the_normal(0.3,0.3,0.3);
    the_normal = unit_vect(the_normal);
    //Point the_origin(0.0,-20.0,-10);
    Point the_origin(4.0,0.0,-15.0);
    color the_plan_color(0.0,2.0,0.0);
    plan the_plan(the_normal, the_origin, the_plan_color);

    //List of hittable
    list_hittable Shapes;
    Shapes.add(make_shared<sphere> (the_sphere));
    Shapes.add(make_shared<sphere> (the_sphere_2));
    Shapes.add(make_shared<plan> (the_plan));
    
    std::cerr<<"Nbr objects : "<<Shapes.objects.size()<<std::endl;

    int max_depth = 50;
    int number_of_sample = 50;

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