#ifndef MOVING_SPHERE_H
#define MOVING_SPHERE_H

#include "rtweekend.h"

#include "hittable.h"
#include "aabb.h"
class moving_sphere : public hittable
{
public:
    moving_sphere() {}
    moving_sphere(point3 c0, point3 c1, double r, shared_ptr<material> m)
        : center0(c0), center1(c1), center_vec(c1 - c0), radius(r), mat(m)
    {
        const auto rvec = vec3(radius, radius, radius);
        const aabb box0(center0 - rvec, center0 + rvec);
        const aabb box1(center1 - rvec, center1 + rvec);
        bbox = aabb(box0, box1);
    };

    bool hit(const ray &r, interval ray_t, hit_record &rec) const override
    {
        vec3 oc = r.origin() - center(r.time());
        auto a = r.direction().length_squared();
        auto half_b = dot(oc, r.direction());
        auto c = oc.length_squared() - radius * radius;

        auto discriminant = half_b * half_b - a * c;
        if (discriminant < 0)
            return false;
        auto sqrtd = sqrt(discriminant);

        // Find the nearest root that lies in the acceptable range.
        auto root = (-half_b - sqrtd) / a;
        if (!ray_t.contains(root))
        {
            root = (-half_b + sqrtd) / a;
            if (!ray_t.contains(root))
                return false;
        }

        rec.t = root;
        rec.p = r.at(rec.t);
        auto outward_normal = (rec.p - center(r.time())) / radius;
        rec.set_face_normal(r, outward_normal);
        rec.mat = mat;

        return true;
    }

    aabb bounding_box() const override { return bbox; }

    point3 center(double time) const
    {
        // Linearly interpolate from center0 to center1 according to time, where t=0 yields
        // center0, and t=1 yields center1.
        return center0 + time * center_vec;
    }

public:
    point3 center0, center1;
    vec3 center_vec;
    double radius;
    shared_ptr<material> mat;
    aabb bbox;
};

#endif