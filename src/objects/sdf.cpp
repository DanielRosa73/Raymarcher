#include "sdf.h"
#include <cmath>


namespace SDF {
    float sphereSDF(const Vector3& point, const Sphere& sphere) {
        return (point - sphere.getCenter()).length() - sphere.getRadius();
    }

    float planeSDF(const Vector3& point, const Plane& plane) {
        return point.dot(plane.getNormal()) - plane.getDistance();
    }

    float cubeSDF(const Vector3& point, const Cube& cube) {
        Vector3 p = point - cube.getCenter();
        Vector3 d = p.abs() - cube.getDimensions() * 0.5f;
        return d.max(0.0f).length() + std::min(std::max(d.x, std::max(d.y, d.z)), 0.0f);
    }

    float torusSDF(const Vector3& point, const Torus& torus) {
        Vector3 p = point - torus.getCenter();
        Vector3 q = Vector3((Vector3(p.x, 0.0f, p.z).length() - torus.getMajorRadius()), p.y, 0.0f);
        return q.length() - torus.getMinorRadius();
    }

    float coneSDF(const Vector3& point, const Cone& cone) {
        Vector3 p = point - cone.getCenter();
        Vector3 c = Vector3(0, cone.getHeight(), 0);  
        float q = (p - c * std::max(0.0f, p.dot(c) / c.dot(c))).length(); 
        return std::max(-q, p.y);
    }

    float cubeWithHoleSDF(const Vector3& point, const CubeWithHole& cubeWithHole) {
        const Cube& cube = cubeWithHole.getCube();
        const Sphere& sphere = cubeWithHole.getSphere();

        float cubeSdf = cubeSDF(point, cube);
        float sphereSdf = sphereSDF(point, sphere);

        return std::max(cubeSdf, -sphereSdf);
    }

    float mandelbulbDE(const Vector3& point, const Mandelbulb& mandelbulb) {
        Vector3 z = point;
        float dr = 1.0;
        float r = 0.0;
        int iterations = 100;
        int power = 8; 
        float bailout = 2.0;
        float epsilon = 0.00001;

        for (int i = 0; i < iterations; ++i) {
            r = z.length();
            if (r < epsilon) {
                return -0.5*log(r)*r/dr;
            }
            if (r > bailout) {
                return 0.5*log(r)*r/dr;
            }
            float theta = acos(z.z / r);
            float phi = atan2(z.y, z.x);
            dr =  pow(r, power-1.0)*power*dr + 1.0;
            float zr = pow(r,power);
            theta = theta*power;
            phi = phi*power;
            z = zr * Vector3(sin(theta)*cos(phi), sin(phi)*sin(theta), cos(theta));
            z += point;
        }
        return 0.5*log(r)*r/dr;
    }

    float frameSDF(const Vector3& point, const Frame& frame) {
        float FRAME_THICKNESS = 3;
        const Cube& cube = frame.getCube();
        Vector3 dimensions = cube.getDimensions();
        float dim = dimensions.x;
        Vector3 innerCenter = cube.getCenter();
        

        // Outer cube
        float outerSDF = cubeSDF(point, cube);

        std::vector<Vector3> rectangleDimensions = {
            Vector3(dim * 2, FRAME_THICKNESS + 0.001 , FRAME_THICKNESS + 0.001), // Along y-axis
            Vector3(FRAME_THICKNESS + 0.001, dim * 2, FRAME_THICKNESS + 0.001), // Along z-axis
            Vector3(FRAME_THICKNESS + 0.001, FRAME_THICKNESS + 0.001, dim * 2)  // Along x-axis
        };

        std::vector<float> SDFs = {outerSDF};

        for (int i = 0; i < 3; ++i) {
            Vector3 innerCubeCenter = innerCenter;
            Cube innerCube(innerCubeCenter, rectangleDimensions[i], cube.getColor(), cube.getMaterial());
            float innerSDF = cubeSDF(point, innerCube);

            // Subtract from outer SDF
            SDFs.push_back(-innerSDF);
        }

        // Take the maximum
        float result = *std::max_element(SDFs.begin(), SDFs.end());

        return result;
    }
}