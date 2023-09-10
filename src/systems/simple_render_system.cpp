#include "simple_render_system.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <array>
#include <iostream>
#include <stdexcept>

namespace lve
{
    struct SimplePushConstantData
    {
        glm::mat4 modelMatrix { 1.f };
        glm::mat4 normalMatrix { 1.f };
    };

    SimpleRenderSystem::SimpleRenderSystem(LveDevice& device, VkRenderPass pass, VkDescriptorSetLayout globalSetLayout)
        : lveDevice { device }
    {
        createPipelineLayout(globalSetLayout);
        createPipeline(pass);
    }

    SimpleRenderSystem::~SimpleRenderSystem()
    {
        vkDestroyPipelineLayout(lveDevice.device(), pipelineLayout, nullptr);
    }

    void SimpleRenderSystem::createPipelineLayout(VkDescriptorSetLayout globalSetLayout)
    {
        VkPushConstantRange pushConstantRange {};
        pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
        pushConstantRange.size = sizeof(SimplePushConstantData);
        pushConstantRange.offset = 0;

        std::vector<VkDescriptorSetLayout> descriptorSetLayouts { globalSetLayout };

        VkPipelineLayoutCreateInfo pipelineLayoutInfo {};

        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(descriptorSetLayouts.size());
        pipelineLayoutInfo.pSetLayouts = descriptorSetLayouts.data();
        pipelineLayoutInfo.pushConstantRangeCount = 1;
        pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;

        if (vkCreatePipelineLayout(lveDevice.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
        {
            throw std::runtime_error("cannot create pipeline layout");
        }
    }

    void SimpleRenderSystem::createPipeline(VkRenderPass pass)
    {
        assert(pipelineLayout != nullptr && "cannot create pipeline before pipelineLayout");

        PipelineConfigInfo pipelineConfig {};
        LvePipeline::defaultPipelineConfigInfo(pipelineConfig);

        pipelineConfig.renderPass = pass;
        pipelineConfig.pipelineLayout = pipelineLayout;
        lvePipeline = std::make_unique<LvePipeline>(
            lveDevice, "shaders/simple_shader.vert.spv", "shaders/simple_shader.frag.spv", pipelineConfig);
    }

    void SimpleRenderSystem::renderGameObjects(FrameInfo& frameInfo)
    {
        lvePipeline->bind(frameInfo.commandBuffer);

        vkCmdBindDescriptorSets(frameInfo.commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &frameInfo.globalDescriptorSet, 0, nullptr);

        for (auto& kv : frameInfo.gameObjects)
        {
            auto& obj = kv.second;
            if (obj.model == nullptr)
                continue;
            SimplePushConstantData push {};
            push.modelMatrix = obj.transform.mat4();
            push.normalMatrix = obj.transform.normalMatrix();

            // std::cout << "vkCmdPushConstants(frameInfo.commandBuffer" << std::endl;
            vkCmdPushConstants(frameInfo.commandBuffer,
                pipelineLayout,
                VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
                0,
                sizeof(SimplePushConstantData),
                &push);

            // std::cout << "obj.model->bind" << std::endl;
            obj.model->bind(frameInfo.commandBuffer);
            // std::cout << "obj.model->draw" << std::endl;
            obj.model->draw(frameInfo.commandBuffer);
        }
    }
}