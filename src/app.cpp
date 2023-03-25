#include "app.hpp"

#include "simple_render_system.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <stdexcept>
#include <array>

namespace lve
{
    App::App()
    {
        loadGameObjects();
    }

    App::~App()
    {
    }

    void App::run()
    {
        SimpleRenderSystem system{lveDevice, lveRenderer.getSwapchainRenderPass()};

        while (!lveWindow.shouldClose())
        {
            glfwPollEvents();

            if (auto commandBuffer = lveRenderer.beginFrame())
            {
                lveRenderer.beginSwapChainRenderPass(commandBuffer);

                system.renderGameObjects(commandBuffer, gameObjects);

                lveRenderer.endSwapChainRenderPass(commandBuffer);
                lveRenderer.endFrame();
            }
        }

        vkDeviceWaitIdle(lveDevice.device());
    }

    void App::loadGameObjects()
    {
        std::vector<LveModel::Vertex> vertices{
            {{0.0f, -0.5f}, {0.0f, 1.0f, 0.0f}},
            {{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
            {{-0.5f, 0.5f}, {1.0f, 0.0f, 0.0f}}};

        auto lveModel = std::make_shared<LveModel>(lveDevice, vertices);
        const int triangleCount = 1000;

        for (int i = 0; i < triangleCount; i++)
        {
            auto triangleFrac = static_cast<float>(i) / triangleCount;
            auto triangle = LveGameObject::createGameObject();

            triangle.model = lveModel;
            triangle.color = {.1f, triangleFrac, 1.0f - triangleFrac};
            triangle.transform2d.translation.x = triangleFrac * .2f;
            triangle.transform2d.translation.y = triangleFrac * .2f;
            triangle.transform2d.scale = {triangleFrac * 2.0f, triangleFrac * 0.5f};
            triangle.transform2d.rotation = triangleFrac * -.25f * glm::two_pi<float>();

            gameObjects.push_back(std::move(triangle));
        }
    }

}