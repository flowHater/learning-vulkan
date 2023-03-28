#include "app.hpp"

#include "simple_render_system.hpp"
#include "lve_camera.hpp"
#include "keyboard_movement_controller.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <stdexcept>
#include <array>
#include <chrono>
#include <iostream>

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
        LveCamera camera{};
        camera.setViewDirection(glm::vec3(0.f), glm::vec3(0.5f, 0.f, 1.f));

        auto viewerObject = LveGameObject::createGameObject();
        KeyboardMovementController cameraController{};

        auto currentTime = std::chrono::high_resolution_clock::now();

        while (!lveWindow.shouldClose())
        {
            glfwPollEvents();

            auto newTime = std::chrono::high_resolution_clock::now();
            float frameTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
            currentTime = newTime;

            cameraController.moveInPlane(lveWindow.getWindow(), frameTime, viewerObject);
            camera.setViewYXZ(viewerObject.transform.translation, viewerObject.transform.rotation);

            float aspect = lveRenderer.getAspectRatio();
            camera.setPerspectiveProjection(glm::radians(50.f), aspect, 0.1f, 10.f);

            if (auto commandBuffer = lveRenderer.beginFrame())
            {
                lveRenderer.beginSwapChainRenderPass(commandBuffer);

                system.renderGameObjects(commandBuffer, gameObjects, camera);

                lveRenderer.endSwapChainRenderPass(commandBuffer);
                lveRenderer.endFrame();
            }
        }

        vkDeviceWaitIdle(lveDevice.device());
    }

    void App::loadGameObjects()
    {
        {
            std::string path = "models/flat_vase.obj";
            std::shared_ptr<LveModel> lveModel = LveModel::createModelFromFile(lveDevice, path);

            auto obj = LveGameObject::createGameObject();

            obj.model = lveModel;
            obj.transform.translation = {.5f, .5f, 2.5f};
            obj.transform.scale = glm::vec3{1.f, 0.5f, 1.f};

            gameObjects.push_back(std::move(obj));
        }

        {
            std::string path = "models/smooth_vase.obj";
            std::shared_ptr<LveModel> lveModel = LveModel::createModelFromFile(lveDevice, path);
            auto obj = LveGameObject::createGameObject();

            obj.model = lveModel;
            obj.transform.translation = {-.5f, 0.5f, 2.5f};
            obj.transform.scale = glm::vec3{1.f, 0.5f, 1.f};

            gameObjects.push_back(std::move(obj));
        }
    }

}