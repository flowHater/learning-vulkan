#include "app.hpp"

#include "imgui.h"
#include "keyboard_movement_controller.hpp"
#include "lve_camera.hpp"
#include "systems/point_light_system.hpp"
#include "systems/simple_render_system.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <array>
#include <chrono>
#include <iostream>
#include <stdexcept>

namespace lve
{
    App::App()
    {
        globalPool = LveDescriptorPool::Builder(lveDevice)
                         .setMaxSets(lveRenderer.swapChainImageCount())
                         .addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, lveRenderer.swapChainImageCount())
                         .build();
        loadGameObjects();
    }

    App::~App()
    {
    }

    void App::run()
    {
        std::vector<std::unique_ptr<LveBuffer>> uboBuffers(lveRenderer.swapChainImageCount());
        for (int i = 0; i < uboBuffers.size(); i++)
        {
            uboBuffers[i] = std::make_unique<LveBuffer>(
                lveDevice,
                sizeof(GlobalUbo),
                1,
                VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
            uboBuffers[i]->map();
        }

        auto globalSetLayout = LveDescriptorSetLayout::Builder(lveDevice)
                                   .addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS)
                                   .build();

        std::vector<VkDescriptorSet> globalDescriptorSets(lveRenderer.swapChainImageCount());

        for (int i = 0; i < globalDescriptorSets.size(); i++)
        {
            auto bufferInfo = uboBuffers[i]->descriptorInfo();

            LveDescriptorWriter(*globalSetLayout, *globalPool)
                .writeBuffer(0, &bufferInfo)
                .build(globalDescriptorSets[i]);
        }

        SimpleRenderSystem system { lveDevice, lveRenderer.getSwapchainRenderPass(), globalSetLayout->getDescriptorSetLayout() };
        PointLightSystem pointLightSystem { lveDevice, lveRenderer.getSwapchainRenderPass(), globalSetLayout->getDescriptorSetLayout() };
        LveCamera camera {};
        camera.setViewDirection(glm::vec3(0.f), glm::vec3(0.5f, 0.f, 1.f));

        gui.init();

        auto viewerObject = LveGameObject::createGameObject();
        viewerObject.transform.translation.z = -2.5f;
        KeyboardMovementController cameraController {};

        auto currentTime = std::chrono::high_resolution_clock::now();

        while (!lveWindow.shouldClose())
        {
            glfwPollEvents();
            gui.newFrame();

            auto newTime = std::chrono::high_resolution_clock::now();
            float frameTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
            currentTime = newTime;

            auto frameRate = 1.0f / frameTime;

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / frameRate, frameRate);

            cameraController.moveInPlane(lveWindow.getWindow(), frameTime, viewerObject);
            camera.setViewYXZ(viewerObject.transform.translation, viewerObject.transform.rotation);

            float aspect = lveRenderer.getAspectRatio();
            camera.setPerspectiveProjection(glm::radians(50.f), aspect, 0.1f, 1000.f);

            if (auto commandBuffer = lveRenderer.beginFrame())
            {
                int frameIndex = lveRenderer.getFrameIndex();

                FrameInfo frameInfo {
                    frameIndex,
                    frameTime,
                    commandBuffer,
                    camera,
                    globalDescriptorSets[frameIndex],
                    gameObjects
                };
                // update
                GlobalUbo ubo {};
                ubo.projection = camera.getProjection();
                ubo.view = camera.getView();
                ubo.inverseView = camera.getInverseView();
                pointLightSystem.update(frameInfo, ubo);
                uboBuffers[frameIndex]->writeToBuffer(&ubo);
                uboBuffers[frameIndex]->flush();

                // render
                lveRenderer.beginSwapChainRenderPass(commandBuffer);

                system.renderGameObjects(frameInfo);
                pointLightSystem.render(frameInfo);

                gui.renderFrame(commandBuffer);

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
            obj.transform.translation = { -.5f, .5f, 0.f };
            obj.transform.scale = { 3.f, 1.5f, 3.f };

            gameObjects.emplace(obj.get_id(), std::move(obj));
        }

        {
            std::string path = "models/smooth_vase.obj";
            std::shared_ptr<LveModel> lveModel = LveModel::createModelFromFile(lveDevice, path);
            auto obj = LveGameObject::createGameObject();

            obj.model = lveModel;
            obj.transform.translation = { .5f, .5f, 0.f };
            obj.transform.scale = { 3.f, 1.5f, 3.f };

            gameObjects.emplace(obj.get_id(), std::move(obj));
        }

        {
            std::string path = "models/quad.obj";
            std::shared_ptr<LveModel> lveModel = LveModel::createModelFromFile(lveDevice, path);
            auto obj = LveGameObject::createGameObject();

            obj.model = lveModel;
            obj.transform.translation = { -0.f, 0.5f, 0.f };
            obj.transform.scale = glm::vec3 { 3.f, 1.f, 3.f };

            gameObjects.emplace(obj.get_id(), std::move(obj));
        }

        {
            std::vector<glm::vec3> lightColors {
                { 1.f, .1f, .1f },
                { .1f, .1f, 1.f },
                { .1f, 1.f, .1f },
                { 1.f, 1.f, .1f },
                { .1f, 1.f, 1.f },
                { 1.f, 1.f, 1.f } //
            };
            for (int i = 0; i < lightColors.size(); i++)
            {
                auto pointLight = LveGameObject::makePointLight(0.2f);
                pointLight.color = lightColors[i];
                auto rotateLight = glm::rotate(glm::mat4(1.f),
                    (i * glm::two_pi<float>() / lightColors.size()), { 0.f, -1.f, 0.f });
                pointLight.transform.translation = glm::vec3(rotateLight * glm::vec4(-1.f, -1.f, -1.f, 1.0f));
                gameObjects.emplace(pointLight.get_id(), std::move(pointLight));
            }
        }
    }

}
