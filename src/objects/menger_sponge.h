#ifndef MENGER_SPONGE_H
#define MENGER_SPONGE_H

#include "../utilities/vector3.h"
#include "object.h"
#include "../utilities/color.h"

class MengerSponge : public Object {
public:
    MengerSponge();
    MengerSponge(const Vector3& center, float scale, int iterations, const Color& color, const Material& material);
    
    const Vector3& getCenter() const { return m_center; }
    float getScale() const { return m_scale; }
    int getIterations() const { return m_iterations; }
    float SDF(const Vector3& point) const override;
    void getUV(const Vector3& p, float& u, float& v) const override {std::cout << p.x << u << v << std::endl;}

private:
    Vector3 m_center;
    float m_scale;
    int m_iterations;
};

#endif // MENGER_SPONGE_H
