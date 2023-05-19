#pragma once

#include "object.h"
#include "../utilities/vector3.h"
#include "../utilities/color.h"
#include "../engine/ray.h"

class Plane : public Object {
public:
    
    Plane();
    Plane(const Vector3& normal, float distance, const Color& color);

   
    Vector3 getNormal() const { return m_normal; }
    float getDistance() const { return m_distance; }
    Color getColor() const { return m_color; }

private:
    Vector3 m_normal;
    float m_distance;
    Color m_color;
};
