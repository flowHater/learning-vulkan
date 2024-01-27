#include "lve_game_object.hpp"

#include <glm/ext/matrix_transform.hpp>
#include <glm/fwd.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/io.hpp>
#include <iostream>

namespace lve
{

    glm::mat4 TransformComponent::mat4()
    {
        auto transform = glm::translate(glm::mat4 { 1.f }, translation);
        transform = glm::eulerAngleYXZ(rotation.y, rotation.x, rotation.z) * transform;
        transform = glm::scale(transform, scale);
        std::cout << transform << std::endl;
        return transform;
    }

    glm::mat3 TransformComponent::normalMatrix()
    {
        return glm::translate(glm::scale(glm::eulerAngleYXZ(rotation.y, rotation.x, rotation.z), 1.0f / scale), translation);
    }

    LveGameObject LveGameObject::makePointLight(float intensity, float radius, glm::vec3 color)
    {
        LveGameObject obj = LveGameObject::createGameObject();

        obj.color = color;
        obj.transform.scale.x = radius;

        obj.pointLight = std::make_unique<PointLightComponent>();
        obj.pointLight->lightIntensity = intensity;

        return obj;
    }

}