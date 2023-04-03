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
#include "../include/extern/json.hpp"

using json = nlohmann::json;

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

//Function that take the json file and create the scene
void create_scene(const json j, list_hittable& Shapes, camera &the_camera){
    // Parse camera
    const auto& cam = j["camera"];
    float view_height = cam["height"].get<float>(); //used directely
    float cx = cam["origin"][0].get<float>();
    float cy = cam["origin"][1].get<float>();
    float cz = cam["origin"][2].get<float>();
    float lx = cam["look_at"][0].get<float>();
    float ly = cam["look_at"][1].get<float>();
    float lz = cam["look_at"][2].get<float>();
    float ux = cam["up_vector"][0].get<float>();
    float uy = cam["up_vector"][1].get<float>();
    float uz = cam["up_vector"][2].get<float>();
    float focal = cam["focal"].get<float>(); //used directely
    //adding to scene
    Point origin = Point(cx,cy,cz);
    auto view_width = ratio*view_height;
    std::cerr<<"\nView_width : "<<view_width<<"\n"<<std::flush;
    auto hor = vec(view_width,0,0);
    auto vert = vec(0,view_height,0);
    vec lower_left_corner = origin - hor/2 - vert/2 - vec(0, 0, focal);
    the_camera = inter_cam(origin, ratio, view_height, focal); 

    // Parse objects
    list_hittable Shapes;
    const auto& objects = j["objects"];
    for (const auto& object : objects) {
        std::string object_type = object["object_type"].get<std::string>();
        if (object_type == "sphere") {
            float sx = object["center"][0].get<float>();
            float sy = object["center"][1].get<float>();
            float sz = object["center"][2].get<float>();
            float radius = object["radius"].get<float>();
            float c1 = object["color"][0].get<float>();
            float c2 = object["color"][1].get<float>();
            float c3 = object["color"][2].get<float>();
            // TODO: add sphere to scene
            double rayon = 4;
            Point center_sphere(sx,sy,sz);
            color the_sphere_color(c1,c2,c3);
            sphere the_sphere(center_sphere, radius,the_sphere_color);
            Shapes.add(make_shared<sphere>(the_sphere));

        } else if (object_type == "plane") {
            float Ox = object["origin"][0].get<float>();
            float Oy = object["origin"][1].get<float>();
            float Oz = object["origin"][2].get<float>();
            float nx = object["normal"][0].get<float>();
            float ny = object["normal"][1].get<float>();
            float nz = object["normal"][2].get<float>();
            float c1 = object["color"][0].get<float>();
            float c2 = object["color"][1].get<float>();
            float c3 = object["color"][2].get<float>();
            //Add plane to scene
            vec the_normal(nx,ny,nz);
            the_normal = unit_vect(the_normal);
            Point the_origin(Ox,Oy,Oz);
            color the_plan_color(c1,c2,c3);
            plan the_plan(the_normal, the_origin, the_plan_color);
            Shapes.add(make_shared<plan>(the_plan));
        
        } else {
            std::cerr << "Invalid object type: " << object_type << std::endl;
            return 1;
        }
    }
}

// Creation d'une image au format PPM, exemple
int main(int argc, char *argv[]){
    //check and read json in argv
    if (argc != 2 ) {
        std::cerr << " No .json input file OR too many inputs \n Exit \n" << std::endl;
        return 1;
    }
    if(argc == 2){
        std::cerr<<"Usage of : "<< argv[1] << " <json_file> as scene description"<<std::endl;
        std::ifstream f(argv[1]);
        //check if the stream is open
        if(!f.is_open()){
            std::cerr<<"Error : file "<<argv[1]<<" not found"<<std::endl;
            return 1;
        }

        json j = json::parse(f);

        // Parse image
        const auto& image = j["image"];
        int width = image["width"].get<int>();
        int max_depth = image["max_depth"].get<int>();
        float aspect_ratio_x = image["aspect_ratio"][0].get<float>();
        float aspect_ratio_y = image["aspect_ratio"][1].get<float>();

        //add image data to scene
        const auto ratio = aspect_ratio_x/aspect_ratio_y; 
        int height = static_cast<int>(width/ratio);
        std::cerr<<"\n Width : "<<width<<"\nHeight: "<<height<<"\n";
        std::cerr << "max_depth: " << max_depth << std::endl;

        // Parse camera
        const auto& cam = j["camera"];
        float view_height = cam["height"].get<float>(); //used directely
        float cx = cam["origin"][0].get<float>();
        float cy = cam["origin"][1].get<float>();
        float cz = cam["origin"][2].get<float>();
        float lx = cam["look_at"][0].get<float>();
        float ly = cam["look_at"][1].get<float>();
        float lz = cam["look_at"][2].get<float>();
        float ux = cam["up_vector"][0].get<float>();
        float uy = cam["up_vector"][1].get<float>();
        float uz = cam["up_vector"][2].get<float>();
        float focal = cam["focal"].get<float>(); //used directely
        //adding to scene
        Point origin = Point(cx,cy,cz);
        auto view_width = ratio*view_height;
        std::cerr<<"\nView_width : "<<view_width<<"\n"<<std::flush;
        auto hor = vec(view_width,0,0);
        auto vert = vec(0,view_height,0);
        vec lower_left_corner = origin - hor/2 - vert/2 - vec(0, 0, focal);

        camera the_camera(origin, ratio, view_height, focal);

        // Parse objects
        list_hittable Shapes;
        const auto& objects = j["objects"];
        for (const auto& object : objects) {
            std::string object_type = object["object_type"].get<std::string>();
            if (object_type == "sphere") {
                float sx = object["center"][0].get<float>();
                float sy = object["center"][1].get<float>();
                float sz = object["center"][2].get<float>();
                float radius = object["radius"].get<float>();
                float c1 = object["color"][0].get<float>();
                float c2 = object["color"][1].get<float>();
                float c3 = object["color"][2].get<float>();
                // TODO: add sphere to scene
                double rayon = 4;
                Point center_sphere(sx,sy,sz);
                color the_sphere_color(c1,c2,c3);
                sphere the_sphere(center_sphere, radius,the_sphere_color);
                Shapes.add(make_shared<sphere>(the_sphere));

            } else if (object_type == "plane") {
                float Ox = object["origin"][0].get<float>();
                float Oy = object["origin"][1].get<float>();
                float Oz = object["origin"][2].get<float>();
                float nx = object["normal"][0].get<float>();
                float ny = object["normal"][1].get<float>();
                float nz = object["normal"][2].get<float>();
                float c1 = object["color"][0].get<float>();
                float c2 = object["color"][1].get<float>();
                float c3 = object["color"][2].get<float>();
                //Add plane to scene
                vec the_normal(nx,ny,nz);
                the_normal = unit_vect(the_normal);
                Point the_origin(Ox,Oy,Oz);
                color the_plan_color(c1,c2,c3);
                plan the_plan(the_normal, the_origin, the_plan_color);
                Shapes.add(make_shared<plan>(the_plan));
            
            } else {
                std::cerr << "Invalid object type: " << object_type << std::endl;
                return 1;
            }

        }

        std::cerr<<"Nbr objects : "<< Shapes.objects.size() <<std::endl;

        //Init of PPM file
        std::ofstream myfile;
        char buffer[100];
        sprintf(buffer,"image_of_%d_%d_obj_.ppm",width,Shapes.objects.size());
        myfile.open(buffer);
        myfile<<"P3\n"<<width<<" "<<height<<"\n255\n";
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

}