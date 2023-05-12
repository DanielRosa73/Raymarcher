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
};

#endif // OBJECT_H
