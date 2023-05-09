#ifndef CUBE_H
#define CUBE_H

#include "object.h"

class Cube : public Object {
public:
    Cube();
    Cube(const Vector3& center, const Vector3& dimensions, const Color& color);

    bool intersect(const Ray& ray, float& t, Vector3& normal, Color& color) const override;
    const Vector3& getCenter() const { return m_center; }
    const Vector3& getDimensions() const { return m_dimensions; }
    Color getColor() const { return m_color; }

private:
    Vector3 m_center;
    Vector3 m_dimensions;
    Color m_color;
};

#endif // CUBE_H
