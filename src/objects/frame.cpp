#include "frame.h"


Frame::Frame(const Cube& cube): m_cube(cube) {}

float Frame::SDF(const Vector3& point) const {
        float FRAME_THICKNESS = 3;
        const Cube& cube = m_cube;
        Vector3 dimensions = cube.getDimensions();
        float dim = dimensions.x;
        Vector3 innerCenter = cube.getCenter();
        

        // Outer cube
        float outerSDF = cube.SDF(point);

        std::vector<Vector3> rectangleDimensions = {
            Vector3(dim * 2, FRAME_THICKNESS + 0.001 , FRAME_THICKNESS + 0.001), // Along y-axis
            Vector3(FRAME_THICKNESS + 0.001, dim * 2, FRAME_THICKNESS + 0.001), // Along z-axis
            Vector3(FRAME_THICKNESS + 0.001, FRAME_THICKNESS + 0.001, dim * 2)  // Along x-axis
        };

        std::vector<float> SDFs = {outerSDF};

        for (int i = 0; i < 3; ++i) {
            Vector3 innerCubeCenter = innerCenter;
            Cube innerCube(innerCubeCenter, rectangleDimensions[i], cube.getColor(), cube.getMaterial());
            float innerSDF = innerCube.SDF(point);

            // Subtract from outer SDF
            SDFs.push_back(-innerSDF);
        }

        // Take the maximum
        float result = *std::max_element(SDFs.begin(), SDFs.end());

        return result;
}

