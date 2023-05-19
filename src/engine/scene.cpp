#include "scene.h"
#include "camera.h"

Scene::Scene() {}

void Scene::addObject(std::shared_ptr<Object> object) {
    m_objects.push_back(object);
}

void Scene::addLight(std::shared_ptr<Light> light) {
       m_lights.push_back(light);
}


void Scene::setCamera(const Camera& camera) {
    m_camera = camera;
}

const std::vector<std::shared_ptr<Object>>& Scene::getObjects() const {
    return m_objects;
}

const std::vector<std::shared_ptr<Light>>& Scene::getLights() const {
    return m_lights;
}

const Camera& Scene::getCamera() const {
    return m_camera;
}