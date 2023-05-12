#ifndef OBJECT_H
#define OBJECT_H

#include "ray.h"
#include "vector3.h"
#include "color.h"




struct Material {
    float diffuse;
    float specular;
    float reflectivity;

    Material() {diffuse = 1; specular = 1;reflectivity = 0;}
    Material(float diffuse, float specular, float reflectivity) : diffuse(diffuse), specular(specular), reflectivity(reflectivity) {} 
};

class Object {
public:
    virtual ~Object() {}
};

#endif // OBJECT_H
