#ifndef CUBE_H
#define CUBE_H

#include "object.h"

class Cube : public Object {
public:
    Cube();
    Cube(const Vector3& center, const Vector3& dimensions, const Color& color, const Material& mat);

    
    const Vector3& getCenter() const { return m_center; }
    const Vector3& getDimensions() const { return m_dimensions; }
    float SDF(const Vector3& point) const override;

private:
    Vector3 m_center;
    Vector3 m_dimensions;
    
};

#endif // CUBE_H
