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

private:
    Cube m_cube;
    Sphere m_sphere;
};



#endif // CUBEWITHHOLE_H
