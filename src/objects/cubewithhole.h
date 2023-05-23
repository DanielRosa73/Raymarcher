#ifndef CUBEWITHHOLE_H
#define CUBEWITHHOLE_H

#include "cube.h"
#include "sphere.h"
#include "object.h"

class CubeWithHole : public Object {
public:
    //CubeWithHole();
    CubeWithHole(const Cube& cube, const Sphere& sphere);

    const Cube& getCube() const { return m_cube; }
    const Sphere& getSphere() const { return m_sphere; }
    float SDF(const Vector3& point) const override;
    void getUV(const Vector3& p, float& u, float& v) const override {std::cout << p.x << u << v << std::endl;}

private:
    Cube m_cube;
    Sphere m_sphere;
};



#endif // CUBEWITHHOLE_H
