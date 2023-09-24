#pragma once

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_vulkan.h"
#include "lve_device.hpp"
#include "lve_frame_info.hpp"
#include "lve_renderer.hpp"
#include <string>
#include <vulkan/vulkan_core.h>

namespace lve
{
    class LveGui
    {
    public:
        LveGui(LveDevice& device, LveWindow& window, LveRenderer& rederer);
        ~LveGui();

        void init();

        void newFrame();

        void renderFrame(VkCommandBuffer& commandBuffer);

    private:
        LveDevice& device;
        LveWindow& window;
        LveRenderer& renderer;
        VkDescriptorPool pool = nullptr;
        void loadFonts();
        ImGuiIO io;
    };
}