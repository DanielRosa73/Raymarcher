#include "torus.h"

Torus::Torus(const Vector3& position, float major_radius, float minor_radius, const Color& color, const Material& mat)
    : position(position), major_radius(major_radius), minor_radius(minor_radius), color(color), material(mat) {}


Color Torus::getColor() const {
    return color;
}

Material Torus::getMaterial() const {
    return material;
}

