#include "light.h"

Light::Light() : m_position(Vector3(0.0f, 0.0f, 0.0f)), m_color(Color(1.0f, 1.0f, 1.0f)), m_radius(0.0f) {}

Light::Light(const Vector3& position, const Color& color, float radius) : m_position(position), m_color(color), m_radius(radius) {}

const Vector3& Light::getPosition() const {
    return m_position;
}

const Color& Light::getColor() const {
    return m_color;
}

Vector3 Light::getRandomPoint() const {
    float u = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    float v = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    float theta = 2.f * M_PI * u;
    float phi = acos(2.f * v - 1.f);

    
    float x = m_radius * sin(phi) * cos(theta);
    float y = m_radius * sin(phi) * sin(theta);
    float z = m_radius * cos(phi);
    
    return m_position + Vector3(x, y, z);
}

