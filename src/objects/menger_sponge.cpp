#include "menger_sponge.h"

MengerSponge::MengerSponge()
    : m_center(0, 0, 0), m_scale(1), m_iterations(0) {
        this->m_color = Color(0,0,1);
        this->m_material = Material();
    }

MengerSponge::MengerSponge(const Vector3& center, float scale, int iterations, const Color& color, const Material& mat)
    : m_center(center), m_scale(scale), m_iterations(iterations) {
        this->m_color = color;
        this->m_material = mat;
    }

float MengerSponge::SDF(const Vector3& point) const {
        Vector3 z = (point - m_center) / m_scale;
        float scale = m_scale;
        int iterations = m_iterations;

        for (int i = 0; i < iterations; ++i) {
            z.x = fabs(z.x);
            z.y = fabs(z.y);
            z.z = fabs(z.z);

            if (z.x - z.y < 0) std::swap(z.x, z.y);
            if (z.x - z.z < 0) std::swap(z.x, z.z);
            if (z.y - z.z < 0) std::swap(z.y, z.z);

            z = scale*z - Vector3(scale-1, scale-1, 2*scale-2);

            if (z.z > -0.5*scale) {
                z.z -= scale;
            }
            else if (z.z < -1.5*scale) {
                z.z += 2*scale;
            }
        }

        return (z.length() - 2) / pow(scale, iterations);
}