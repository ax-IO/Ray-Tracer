#include "rtweekend.h"

#include "bvh.h"
#include "camera.h"
#include "color.h"
#include "constant_medium.h"
#include "hittable_list.h"
#include "material.h"
#include "moving_sphere.h"
#include "quad.h"
#include "scene.h"
#include "triangle.h"
#include "sphere.h"
#include "texture.h"

#include <fstream>
#include <string>
#include <vector>
#include <sstream>
// g++ -O3 main.cc -o main && ./main > image.ppm

void cornell_box(scene &scene_desc)
{
    scene_desc.image_width = 600;
    scene_desc.aspect_ratio = 1.0;
    scene_desc.samples_per_pixel = 50;
    scene_desc.max_depth = 50;
    scene_desc.background = color(0, 0, 0);

    // Camera
    scene_desc.cam.lookfrom = point3(278, 278, -800);
    scene_desc.cam.lookat = point3(278, 278, 0);
    scene_desc.cam.vup = vec3(0, 1, 0);
    scene_desc.cam.vfov = 40.0;
    scene_desc.cam.aperture = 0.0;
    scene_desc.cam.focus_dist = 10.0;

    hittable_list &world = scene_desc.world;

    auto red = make_shared<lambertian>(color(.65, .05, .05));
    auto white = make_shared<lambertian>(color(.73, .73, .73));
    auto green = make_shared<lambertian>(color(.12, .45, .15));
    auto black = make_shared<lambertian>(color(.05, .05, .05));
    auto orange = make_shared<lambertian>(color(.95, .59, .05));
    auto lightorange = make_shared<lambertian>(color(.95, .65, .55));
    auto light = make_shared<diffuse_light>(color(15, 15, 15));
    auto aluminium = make_shared<metal>(color(0.8, 0.85, 0.88), 0.0);
    auto glass = make_shared<dielectric>(1.5);

    // Cornell box sides
    world.add(make_shared<quad>(point3(555, 0, 0), vec3(0, 0, 555), vec3(0, 555, 0), green));
    world.add(make_shared<quad>(point3(0, 0, 555), vec3(0, 0, -555), vec3(0, 555, 0), red));
    world.add(make_shared<quad>(point3(0, 555, 0), vec3(555, 0, 0), vec3(0, 0, 555), white));
    world.add(make_shared<quad>(point3(0, 0, 555), vec3(555, 0, 0), vec3(0, 0, -555), white));
    world.add(make_shared<quad>(point3(555, 0, 555), vec3(-555, 0, 0), vec3(0, 555, 0), white));

    // Small light
    // world.add(make_shared<quad>(point3(213, 554, 227), vec3(130, 0, 0), vec3(0, 0, 105), light));

    // Big light
    // world.add(make_shared<quad>(point3(173, 554, 192), vec3(170, 0, 0), vec3(0, 0, 140), light));
    world.add(make_shared<quad>(point3(185, 554, 195), vec3(170, 0, 0), vec3(0, 0, 140), light));

    // Box 1
    // shared_ptr<hittable> box1 = box(point3(0, 0, 0), point3(165, 330, 165), white);
    // box1 = make_shared<rotate_y>(box1, 15);
    // box1 = make_shared<translate>(box1, vec3(265, 0, 295));
    // box1 = make_shared<scale>(box1, 0.9);
    // world.add(make_shared<bvh_node>(box1));

    // Box 2
    // shared_ptr<hittable> box2 = box(point3(0, 0, 0), point3(165, 165, 165), white);
    // box2 = make_shared<rotate_y>(box2, -18);
    // box2 = make_shared<translate>(box2, vec3(130, 0, 65));
    // world.add(box2);

    // Glass Sphere
    world.add(make_shared<sphere>(point3(350, 130, 400), 130, aluminium));

    // Mirror Sphere
    world.add(make_shared<sphere>(point3(190, 90, 150), 90, glass));

    // Mesh Star
    // shared_ptr<hittable> mesh2 = mesh("obj/star.obj", white);
    // mesh2 = make_shared<scale>(mesh2, 300);
    // mesh2 = make_shared<rotate_y>(mesh2, 45);
    // mesh2 = make_shared<translate>(mesh2, vec3(275, 175, 365));
    // world.add(make_shared<bvh_node>(mesh2));

    // Moving Sphere
    // world.add(make_shared<moving_sphere>(point3(225.0, 150.0, 250.0), point3(325.0, 150.0, 250.0), 100.0, orange));

    // Blurred Spheres
    // point3 focus = point3(250, 75, 250);
    // scene_desc.cam.focus_dist = (focus - scene_desc.cam.lookfrom).length();
    // scene_desc.cam.aperture = scene_desc.cam.focus_dist / 5.0;
    // world.add(make_shared<sphere>(focus, 75, orange));                // Focus
    // world.add(make_shared<sphere>(point3(370, 75, 120), 75, orange)); // Avant-gauche
    // world.add(make_shared<sphere>(point3(125, 75, 450), 75, orange)); // Arrière-droite
}

int main()
{
    scene scene_desc;
    cornell_box(scene_desc);
    // final_scene(scene_desc);
    scene_desc.render();

    return 0;
}