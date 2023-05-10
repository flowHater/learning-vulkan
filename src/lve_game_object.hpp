#pragma once

#include "lve_model.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include <memory>
#include <unordered_map>

namespace lve
{
    struct TransformComponent
    {
        glm::vec3 translation{};
        glm::vec3 scale{1.f, 1.f, 1.f};
        glm::vec3 rotation{};

        glm::mat4 mat4();
        glm::mat3 normalMatrix();
    };

    struct PointLightComponent
    {
        float lightIntensity = 1.f;
    };

    class LveGameObject
    {
    public:
        using id_t = unsigned int;
        using Map = std::unordered_map<id_t, LveGameObject>;

        static LveGameObject createGameObject()
        {
            static id_t currentId = 0;
            return LveGameObject{currentId++};
        }

        static LveGameObject makePointLight(float intensity = 10.0f, float radius = 0.1f, glm::vec3 color = glm::vec3(1.f));

        LveGameObject(const LveGameObject &) = delete;
        LveGameObject &operator=(const LveGameObject &) = delete;
        LveGameObject(LveGameObject &&) = default;
        LveGameObject &operator=(LveGameObject &&) = default;

        const id_t get_id() { return id; }

        glm::vec3 color{};
        TransformComponent transform{};

        // optional
        std::shared_ptr<LveModel> model = nullptr;
        std::unique_ptr<PointLightComponent> pointLight = nullptr;

    private:
        LveGameObject(id_t id) : id{id} {}

        id_t id;
    };
}