#ifndef SCENE_H
#define SCENE_H

#include "rtweekend.h"

#include "camera.h"
#include "hittable_list.h"
#include "material.h"
#include "color.h"

class scene
{
public:
    void render()
    {
        const int image_height = static_cast<int>(image_width / aspect_ratio);

        cam.initialize(aspect_ratio);
        std::cout << "P3\n"
                  << image_width << " " << image_height << "\n255\n";

        for (int j = image_height - 1; j >= 0; --j)
        {
            std::clog << "\rScanlines remaining: " << j << ' ' << std::flush;
            for (int i = 0; i < image_width; ++i)
            {
                color pixel_color(0, 0, 0);
                for (int s = 0; s < samples_per_pixel; ++s)
                {
                    auto u = (i + random_double()) / (image_width - 1);
                    auto v = (j + random_double()) / (image_height - 1);
                    ray r = cam.get_ray(u, v);
                    pixel_color += ray_color(r, max_depth);
                }
                write_color(std::cout, pixel_color, samples_per_pixel);
            }
        }

        std::clog << "\nDone.\n";
    }

public:
    hittable_list world;
    camera cam;

    double aspect_ratio = 1.0;
    int image_width = 100;
    int samples_per_pixel = 10;
    int max_depth = 50;
    color background = color(0, 0, 0);

private:
    color ray_color(const ray &r, int depth)
    {
        hit_record rec;

        // If we've exceeded the ray bounce limit, no more light is gathered.
        if (depth <= 0)
            return color(0, 0, 0);

        // If the ray hits nothing, return the background color.
        if (!world.hit(r, interval(0.001, infinity), rec))
            return background;

        ray scattered;
        color attenuation;
        color emitted = rec.mat->emitted(rec.u, rec.v, rec.p);

        if (!rec.mat->scatter(r, rec, attenuation, scattered))
            return emitted;

        return emitted + attenuation * ray_color(scattered, depth - 1);
    }
};

#endif