#ifndef TORUS_H
#define TORUS_H

#include "object.h"

class Torus : public Object {
public:
    Torus(const Vector3& position, float major_radius, float minor_radius, const Color& color, const Material& mat);
    
    Vector3 getCenter() const {return m_center; }
    float getMajorRadius() const {return m_major_radius; }
    float getMinorRadius() const {return m_minor_radius;}
    float SDF(const Vector3& point) const override;

private:
    Vector3 m_center;
    float m_major_radius;
    float m_minor_radius;
};

#endif // TORUS_H
