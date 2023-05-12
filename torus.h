#ifndef TORUS_H
#define TORUS_H

#include "object.h"

class Torus : public Object {
public:
    Torus(const Vector3& position, float major_radius, float minor_radius, const Color& color, const Material& mat);
    
    Color getColor() const;
    Material getMaterial() const;
    Vector3 getCenter() const {return position; }
    float getMajorRadius() const {return major_radius; }
    float getMinorRadius() const {return minor_radius;}

private:
    Vector3 position;
    float major_radius;
    float minor_radius;
    Color color;
    Material material;
};

#endif // TORUS_H
