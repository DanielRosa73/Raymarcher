#ifndef MANDELBOX_H
#define MANDELBOX_H

#include "../utilities/vector3.h"
#include "object.h"
#include "../utilities/color.h"

class Mandelbox : public Object {
public:
    Mandelbox();
    Mandelbox(const Vector3& center, float scale, const Color& color, const Material& material);
    
    const Vector3& getCenter() const { return m_center; }
    float getScale() const { return m_scale; }
    Vector3 boxFold(const Vector3& v) const;
    Vector3 sphereFold(const Vector3& v) const;
    float SDF(const Vector3& point) const override;
    void getUV(const Vector3& p, float& u, float& v) const override {std::cout << p.x << u << v << std::endl;}

private:
    Vector3 m_center;
    float m_scale;
};

#endif // MANDELBOX_H
