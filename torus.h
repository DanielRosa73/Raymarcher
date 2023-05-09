#ifndef TORUS_H
#define TORUS_H

#include "object.h"

class Torus : public Object {
public:
    Torus(const Vector3& position, float major_radius, float minor_radius, const Color& color);

    
    Color getColor() const;
    bool intersect(const Ray& ray, float& t, Vector3& normal, Color& color) const override {
        return false;
    }
    Vector3 getCenter() const {return position; }
    float getMajorRadius() const {return major_radius; }
    float getMinorRadius() const {return minor_radius;}

private:
    Vector3 position;
    float major_radius;
    float minor_radius;
    Color color;
};

#endif // TORUS_H
