#ifndef CONE_H
#define CONE_H

#include "object.h"

class Cone : public Object {
public:
    Cone();
    Cone(const Vector3& center, float height, float radius, const Color& color, const Material& mat);

    
    const Vector3& getCenter() const { return m_center; }
    float getHeight() const { return m_height; }
    float getRadius() const { return m_radius; }
    float SDF(const Vector3& point) const override;

private:
    Vector3 m_center;
    float m_height;
    float m_radius;
    
};

#endif // CONE_H
