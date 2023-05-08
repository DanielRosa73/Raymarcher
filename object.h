#ifndef OBJECT_H
#define OBJECT_H

#include "ray.h"
#include "vector3.h"
#include "color.h"

class Object {
public:
    virtual ~Object() {}

    virtual bool intersect(const Ray& ray, float& t, Vector3& normal, Color& color) const = 0;
};

#endif // OBJECT_H
