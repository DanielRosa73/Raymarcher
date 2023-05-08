#include "plane.h"

Plane::Plane() : m_normal(Vector3(0.0f, 1.0f, 0.0f)), m_distance(0.0f), m_color(Color(0.5f, 0.5f, 0.5f)), m_opacity(1.0f) {}

Plane::Plane(const Vector3& normal, float distance, const Color& color, float opacity) : m_normal(normal.normalized()), m_distance(distance), m_color(color), m_opacity(opacity) {}

bool Plane::intersect(const Ray& ray, float& t, Vector3& normal, Color& color) const {
    float denom = m_normal.dot(ray.getDirection());
    if (std::abs(denom) > 1e-6) {
        float tCandidate = (m_distance - m_normal.dot(ray.getOrigin())) / denom;
        if (tCandidate > 0.0f) {
            t = tCandidate;
            normal = m_normal;
            color = m_color;
            return true;
        }
    }
    return false;
}

