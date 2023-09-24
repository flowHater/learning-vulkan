#include "gui.hpp"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "lve_device.hpp"
#include "lve_renderer.hpp"
#include "lve_swap_chain.hpp"
#include "lve_window.hpp"
#include <iostream>
#include <string>
#include <vulkan/vulkan_core.h>

namespace lve
{
    LveGui::LveGui(LveDevice& device, LveWindow& window, LveRenderer& renderer)
        : device { device }
        , window { window }
        , renderer { renderer }
    {
    }

    LveGui::~LveGui()
    {
        vkDestroyDescriptorPool(device.device(), pool, nullptr);
        ImGui_ImplVulkan_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void LveGui::init()
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; // Enable Gamepad Controls

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();
        // ImGui::StyleColorsLight();

        ImGui_ImplGlfw_InitForVulkan(window.getWindow(), true);

        ImGui_ImplVulkan_InitInfo init_info = {};
        pool = device.createInitImGuiVulkan(init_info);
        ImGui_ImplVulkan_Init(&init_info, renderer.getSwapchainRenderPass());

        loadFonts();
    }

    void LveGui::loadFonts()
    {
        io.Fonts->AddFontDefault();
        VkCommandBuffer commandBuffer;
        VkCommandPool commandPool;

        QueueFamilyIndices queueFamilyIndices = device.findPhysicalQueueFamilies();

        VkCommandPoolCreateInfo poolInfo = {};
        poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily;
        poolInfo.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT | VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

        if (vkCreateCommandPool(device.device(), &poolInfo, nullptr, &commandPool) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create command pool!");
        }

        if (vkResetCommandPool(device.device(), commandPool, 0) != VK_SUCCESS)
        {
            throw "vkResetCommandPool()";
        }

        VkCommandBufferAllocateInfo allocInfo {};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = commandPool;
        allocInfo.commandBufferCount = 1;

        if (vkAllocateCommandBuffers(device.device(), &allocInfo, &commandBuffer) != VK_SUCCESS)
        {
            throw std::runtime_error("vkAllocateCommandBuffers()");
        }

        VkCommandBufferBeginInfo begin_info = {};
        begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        begin_info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
        if (vkBeginCommandBuffer(commandBuffer, &begin_info) != VK_SUCCESS)
        {
            throw "vkBeginCommandBuffer()";
        }

        ImGui_ImplVulkan_CreateFontsTexture(commandBuffer);

        VkSubmitInfo end_info = {};
        end_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        end_info.commandBufferCount = 1;
        end_info.pCommandBuffers = &commandBuffer;
        if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
        {
            throw "vkEndCommandBuffer()";
        }
        if (vkQueueSubmit(device.graphicsQueue(), 1, &end_info, VK_NULL_HANDLE) != VK_SUCCESS)
        {
            throw "vkQueueSubmit()";
        }

        if (vkDeviceWaitIdle(device.device()) != VK_SUCCESS)
        {
            throw "vkDeviceWaitIdle()";
        }

        vkDestroyCommandPool(device.device(), commandPool, nullptr);

        ImGui_ImplVulkan_DestroyFontUploadObjects();
    }

    void LveGui::newFrame()
    {
        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void LveGui::renderFrame(VkCommandBuffer& commandBuffer)
    {

        // Rendering
        ImGui::Render();
        ImDrawData* draw_data = ImGui::GetDrawData();

        ImGui_ImplVulkan_RenderDrawData(draw_data, commandBuffer);
    }
}