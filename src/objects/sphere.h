#ifndef SPHERE_H
#define SPHERE_H

#include "object.h"

class Sphere : public Object {
public:
    Sphere();
    Sphere(const Vector3& center, float radius, const Color& color,const Material& mat);
    
    const Vector3& getCenter() const { return m_center; }
    float getRadius() const { return m_radius; }
    float SDF(const Vector3& point) const override;

    // Calculate UV coordinates for a point on the sphere
    void getUV(const Vector3& p, float& u, float& v) const;

private:
    Vector3 m_center;
    float m_radius;
};

#endif // SPHERE_H
