#include "sphere.h"

Sphere::Sphere() : m_center(Vector3(0.0f, 0.0f, 0.0f)), m_radius(1.0f), m_color(Color(1.0f, 1.0f, 1.0f)) {}

Sphere::Sphere(const Vector3& center, float radius, const Color& color) : m_center(center), m_radius(radius), m_color(color) {}

bool Sphere::intersect(const Ray& ray, float& t, Vector3& normal, Color& color) const {
    Vector3 oc = ray.getOrigin() - m_center;
    float a = ray.getDirection().dot(ray.getDirection());
    float b = 2.0f * oc.dot(ray.getDirection());
    float c = oc.dot(oc) - m_radius * m_radius;
    float discriminant = b * b - 4 * a * c;

    if (discriminant > 0) {
        float temp = (-b - sqrt(discriminant)) / (2.0f * a);
        if (temp > 0) {
            t = temp;
            normal = (ray.pointAtParameter(t) - m_center).normalized();
            color = m_color;
            return true;
        }
        temp = (-b + sqrt(discriminant)) / (2.0f * a);
        if (temp > 0) {
            t = temp;
            normal = (ray.pointAtParameter(t) - m_center).normalized();
            color = m_color;
            return true;
        }
    }

    return false;
}
