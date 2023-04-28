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
    struct GlobalUbo
    {
        glm::mat4 projectionView{1.f};
        glm::vec3 lightDirection = glm::normalize(glm::vec3(1.f, -3.f, -1.f));
    };

    App::App()
    {
        loadGameObjects();
    }

    App::~App()
    {
    }

    void App::run()
    {
        LveBuffer globalUboBuffer{
            lveDevice,
            sizeof(GlobalUbo),
            lveRenderer.swapChainImageCount(),
            VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
            lveDevice.properties.limits.minUniformBufferOffsetAlignment};

        globalUboBuffer.map();

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
                int frameIndex = lveRenderer.getFrameIndex();
                FrameInfo frameInfo{
                    frameIndex,
                    frameTime,
                    commandBuffer,
                    camera};
                // update
                GlobalUbo ubo{};
                ubo.projectionView = camera.getProjection() * camera.getView();
                globalUboBuffer.writeToIndex(&ubo, frameIndex);
                globalUboBuffer.flushIndex(frameIndex);

                // render
                lveRenderer.beginSwapChainRenderPass(commandBuffer);

                system.renderGameObjects(frameInfo, gameObjects);

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