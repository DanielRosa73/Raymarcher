// peanut.cpp
#include "peanut.h"
#include <algorithm>

Peanut::Peanut() 
    : m_sphere1(), m_sphere2(), m_smoothness(1.0f) {}

Peanut::Peanut(const Sphere& sphere1, const Sphere& sphere2, float smoothness) 
    : m_sphere1(sphere1), m_sphere2(sphere2), m_smoothness(smoothness) {}

float Peanut::SDF(const Vector3& point) const {
    float d1 = m_sphere1.SDF(point);
    float d2 = m_sphere2.SDF(point);
    float h = std::max(m_smoothness - std::abs(d1-d2), 0.0f);
    return std::min(d1, d2) - h*h*0.25f/m_smoothness;
}

//Just a placeholder
void Peanut::getUV(const Vector3& p, float& u, float& v) const {
    u = 0.5 + atan2(p.z, p.x) / (2.0 * M_PI);
    v = 0.5 - asin(p.y) / M_PI;
}
