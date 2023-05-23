#include "cubewithhole.h"

// Implementation
//CubeWithHole::CubeWithHole() {}

CubeWithHole::CubeWithHole(const Cube& cube, const Sphere& sphere)
    : m_cube(cube), m_sphere(sphere) {}

float CubeWithHole::SDF(const Vector3& point) const {
        const Cube& cube = m_cube;
        const Sphere& sphere = m_sphere;

        float cubeSdf = cube.SDF(point);
        float sphereSdf = sphere.SDF(point);

        return std::max(cubeSdf, -sphereSdf);
}
