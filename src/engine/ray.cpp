#include "ray.h"

Ray::Ray() : m_origin(Vector3(0.0f, 0.0f, 0.0f)), m_direction(Vector3(0.0f, 0.0f, -1.0f)) {}

Ray::Ray(const Vector3& origin, const Vector3& direction) : m_origin(origin), m_direction(direction.normalized()) {}

const Vector3& Ray::getOrigin() const {
    return m_origin;
}

const Vector3& Ray::getDirection() const {
    return m_direction;
}

Vector3 Ray::pointAtParameter(float t) const {
    return m_origin + m_direction * t;
}
