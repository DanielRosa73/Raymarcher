// peanut.h
#ifndef PEANUT_H
#define PEANUT_H

#include "object.h"
#include "sphere.h"

class Peanut : public Object {
public:
    Peanut();
    Peanut(const Sphere& sphere1, const Sphere& sphere2, float smoothness);

    const Sphere& getSphere1() const { return m_sphere1; }
    const Sphere& getSphere2() const { return m_sphere2; }
    float getSmoothness() const { return m_smoothness; }

    float SDF(const Vector3& point) const override;
    void getUV(const Vector3& p, float& u, float& v) const;

    Color getColor() const override {return m_sphere1.getColor();}
    Material getMaterial() const override {return m_sphere1.getMaterial();}


private:
    Sphere m_sphere1;
    Sphere m_sphere2;
    float m_smoothness;  
};

#endif // PEANUT_H
