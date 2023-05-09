#ifndef CONE_H
#define CONE_H

#include "object.h"

class Cone : public Object {
public:
    Cone();
    Cone(const Vector3& center, float height, float radius, const Color& color);

    bool intersect(const Ray& ray, float& t, Vector3& normal, Color& color) const override;
    const Vector3& getCenter() const { return m_center; }
    float getHeight() const { return m_height; }
    float getRadius() const { return m_radius; }
    Color getColor() const {return m_color; }

private:
    Vector3 m_center;
    float m_height;
    float m_radius;
    Color m_color;
};

#endif // CONE_H
