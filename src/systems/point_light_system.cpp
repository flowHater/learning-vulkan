#include "point_light_system.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <stdexcept>
#include <array>

namespace lve
{

    PointLightSystem::PointLightSystem(LveDevice &device, VkRenderPass pass, VkDescriptorSetLayout globalSetLayout) : lveDevice{device}
    {
        createPipelineLayout(globalSetLayout);
        createPipeline(pass);
    }

    PointLightSystem::~PointLightSystem()
    {
        vkDestroyPipelineLayout(lveDevice.device(), pipelineLayout, nullptr);
    }

    void PointLightSystem::createPipelineLayout(VkDescriptorSetLayout globalSetLayout)
    {
        // VkPushConstantRange pushConstantRange{};
        // pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
        // pushConstantRange.size = sizeof(SimplePushConstantData);
        // pushConstantRange.offset = 0;

        std::vector<VkDescriptorSetLayout> descriptorSetLayouts{globalSetLayout};

        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};

        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(descriptorSetLayouts.size());
        pipelineLayoutInfo.pSetLayouts = descriptorSetLayouts.data();
        pipelineLayoutInfo.pushConstantRangeCount = 0;
        pipelineLayoutInfo.pPushConstantRanges = nullptr;

        if (vkCreatePipelineLayout(lveDevice.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
        {
            throw std::runtime_error("cannot create pipeline layout");
        }
    }

    void PointLightSystem::createPipeline(VkRenderPass pass)
    {
        assert(pipelineLayout != nullptr && "cannot create pipeline before pipelineLayout");

        PipelineConfigInfo pipelineConfig{};
        LvePipeline::defaultPipelineConfigInfo(pipelineConfig);

        pipelineConfig.attributeDescriptions.clear();
        pipelineConfig.bindingDescriptions.clear();
        pipelineConfig.renderPass = pass;
        pipelineConfig.pipelineLayout = pipelineLayout;
        lvePipeline = std::make_unique<LvePipeline>(
            lveDevice, "shaders/point_light.vert.spv", "shaders/point_light.frag.spv", pipelineConfig);
    }

    void PointLightSystem::render(FrameInfo &frameInfo)
    {
        lvePipeline->bind(frameInfo.commandBuffer);

        vkCmdBindDescriptorSets(frameInfo.commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &frameInfo.globalDescriptorSet, 0, nullptr);

        vkCmdDraw(frameInfo.commandBuffer, 6, 1, 0, 0);
    }
}