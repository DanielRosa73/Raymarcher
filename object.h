#ifndef OBJECT_H
#define OBJECT_H

#include "ray.h"
#include "vector3.h"
#include "color.h"

struct Material {
    float diffuse;
    float specular;

    Material() {diffuse = 1; specular = 1;}
    Material(float diffuse, float specular) : diffuse(diffuse), specular(specular) {} 
};

class Object {
public:
    virtual ~Object() {}
    virtual bool intersect(const Ray& ray, float& t, Vector3& normal, Color& color, Material& mat) const = 0;    
};

#endif // OBJECT_H
