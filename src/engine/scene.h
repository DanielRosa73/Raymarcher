#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <memory>
#include "../objects/object.h"
#include "../lights/light.h"
#include "camera.h"

class Scene {
public:
    Scene();

    void addObject(std::shared_ptr<Object> object);
    void addLight(std::shared_ptr<Light> light);
    void setCamera(const Camera& camera);

    const std::vector<std::shared_ptr<Object>>& getObjects() const;
    const std::vector<std::shared_ptr<Light>>& getLights() const;
    const Camera& getCamera() const;

private:
    std::vector<std::shared_ptr<Object>> m_objects;
    std::vector<std::shared_ptr<Light>> m_lights;
    Camera m_camera;
};

#endif // SCENE_H
