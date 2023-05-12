#ifndef SPHERE_H
#define SPHERE_H

#include "object.h"

class Sphere : public Object {
public:
    Sphere();
    Sphere(const Vector3& center, float radius, const Color& color,const Material& mat);

    
    const Vector3& getCenter() const { return m_center; }
    float getRadius() const { return m_radius; }
    Color getColor() const {return m_color; }
    Material getMaterial() const {return m_material; }

private:
    Vector3 m_center;
    float m_radius;
    Color m_color;
    Material m_material;
};

#endif // SPHERE_H
