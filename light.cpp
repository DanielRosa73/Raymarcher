#include "light.h"

Light::Light() : m_position(Vector3(0.0f, 0.0f, 0.0f)), m_color(Color(1.0f, 1.0f, 1.0f)) {}

Light::Light(const Vector3& position, const Color& color) : m_position(position), m_color(color) {}

const Vector3& Light::getPosition() const {
    return m_position;
}

const Color& Light::getColor() const {
    return m_color;
}

//Get random point inside the sphere. This allows for softer shadows.
Vector3 Light::getRandomPoint() const {
    float x = static_cast<float>(rand()) / RAND_MAX * 2.0f - 1.0f;
    float y = static_cast<float>(rand()) / RAND_MAX * 2.0f - 1.0f;
    float z = static_cast<float>(rand()) / RAND_MAX * 2.0f - 1.0f;
    Vector3 random_direction(x, y, z);
    random_direction = random_direction.normalized();
    return m_position + random_direction * m_radius;
}
