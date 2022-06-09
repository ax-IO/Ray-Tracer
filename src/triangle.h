#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "rtweekend.h"

#include <algorithm> //min

// Minimal Obj Loader
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

#include "hittable.h"
#include "hittable_list.h"

class triangle : public hittable
{
public:
    triangle(const point3 _v0, const point3 _v1, const point3 _v2, shared_ptr<material> m) : v0(_v0), v1(_v1), v2(_v2), mat(m)
    {
        set_bounding_box();
    }

    virtual void set_bounding_box()
    {
        double minX = std::min({v0.x(), v1.x(), v2.x()});
        double minY = std::min({v0.y(), v1.y(), v2.y()});
        double minZ = std::min({v0.z(), v1.z(), v2.z()});

        double maxX = std::max({v0.x(), v1.x(), v2.x()});
        double maxY = std::max({v0.y(), v1.y(), v2.y()});
        double maxZ = std::max({v0.z(), v1.z(), v2.z()});

        bbox = aabb(point3(minX, minY, minZ), point3(maxX, maxY, maxZ)).pad();
    }

    // Algorithme d'intersection rayon-triangle de Möller–Trumbore
    bool hit(const ray &r, interval ray_t, hit_record &rec) const override
    {
        const double EPSILON = 1e-8;
        vec3 v0v1 = v1 - v0;
        vec3 v0v2 = v2 - v0;
        vec3 pvec = cross(r.direction(), v0v2);
        double det = dot(v0v1, pvec);

        // Le rayon est parallèle au triangle si det proche de 0
        if (fabs(det) < EPSILON)
            return false;

        double invDet = 1.0 / det;

        vec3 tvec = r.origin() - v0;
        double u = dot(tvec, pvec) * invDet;
        if (u < 0 || u > 1)
            return false;

        vec3 qvec = cross(tvec, v0v1);
        double v = dot(r.direction(), qvec) * invDet;
        if (v < 0 || u + v > 1)
            return false;

        double t = dot(v0v2, qvec) * invDet;

        if (ray_t.contains(t))
        {
            rec.t = t;
            rec.p = r.at(t);
            rec.mat = mat;
            rec.set_face_normal(r, unit_vector(cross(v0v1, v0v2)));

            return true;
        }

        // On a bien une intersection de droite, mais pas de rayon.
        return false;
    }

    aabb bounding_box() const override { return bbox; }

public:
    // Vertices
    point3 v0, v1, v2;

    // Edges
    // vec3 v0v1, v1v2, v2v0;

    // Normals
    // point3 normal;
    // vec3 n0, n1, n2;

    shared_ptr<material> mat;
    aabb bbox;
};

// Print Triangles to console for debug
std::ostream &operator<<(std::ostream &stream, const triangle &tri)
{
    stream << "(" << tri.v0 << "), (" << tri.v1 << "), (" << tri.v2 << ")";
    return stream;
}

inline shared_ptr<hittable_list> mesh(const std::string &filepath, shared_ptr<material> mat)
{
    auto triangles_ptr = make_shared<hittable_list>();

    std::fstream myFile;
    myFile.open(filepath, std::ios::in); //read

    if (myFile.is_open())
    {
        std::vector<vec3> vertices;
        std::vector<triangle> triangles;

        std::string line = "";
        while (std::getline(myFile, line))
        {
            std::string type;
            std::string value1;
            std::string value2;
            std::string value3;

            std::stringstream inputString(line);

            getline(inputString, type, ' ');
            if (type[0] == 'v')
            {
                getline(inputString, value1, ' ');
                getline(inputString, value2, ' ');
                getline(inputString, value3);

                vertices.push_back(vec3(std::stod(value1), std::stod(value2), std::stod(value3)));
            }
            if (type[0] == 'f')
            {
                getline(inputString, value1, ' ');
                getline(inputString, value2, ' ');
                getline(inputString, value3);

                int id1 = std::stoi(value1);
                int id2 = std::stoi(value2);
                int id3 = std::stoi(value3);
                triangles.push_back(triangle(vertices[id1 - 1], vertices[id2 - 1], vertices[id3 - 1], mat));
            }

            line = "";
        }

        myFile.close();

        // for (int i = 0; i < vertices.size(); i++)
        // {
        //     std::cout << "v" << i + 1 << " : " << vertices[i] << std::endl;
        // }

        // for (int i = 0; i < triangles.size(); i++)
        // {
        //     std::cout << "t" << i + 1 << " : " << triangles[i] << std::endl;
        // }

        for (int i = 0; i < triangles.size(); i++)
        {
            triangles_ptr->add(make_shared<triangle>(triangles[i]));
        }
    }

    return triangles_ptr;
}
#endif