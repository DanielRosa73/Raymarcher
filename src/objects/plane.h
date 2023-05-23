#pragma once

#include "object.h"
#include "../utilities/vector3.h"
#include "../utilities/color.h"
#include "../engine/ray.h"

class Plane : public Object {
public:
    
    Plane();
    Plane(const Vector3& normal, float distance, const Color& color, const Material& mat);

   
    Vector3 getNormal() const { return m_normal; }
    float getDistance() const { return m_distance; }
    float SDF(const Vector3& point) const override;
    void getUV(const Vector3& p, float& u, float& v) const override {std::cout << p.x << u << v << std::endl;}

private:
    Vector3 m_normal;
    float m_distance;
};
