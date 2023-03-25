#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>
#include <stdexcept>

namespace lve
{
    class LveWindow
    {
    public:
        LveWindow(int w, int h, std::string name);
        ~LveWindow();

        LveWindow(const LveWindow &) = delete;
        LveWindow &operator=(const LveWindow &) = delete;

        bool shouldClose() { return glfwWindowShouldClose(window); }
        VkExtent2D getExtend() { return {static_cast<uint32_t>(width), static_cast<uint32_t>(height)}; }
        bool wasWindowResized() { return isFrameBufferResized; }
        void resetIsFrameBufferResized() { isFrameBufferResized = false; }

        void createWindowSurface(VkInstance instance, VkSurfaceKHR *surface);

    private:
        void initWindow();
        static void frameBufferResizedCallback(GLFWwindow *window, int width, int height);

        int width;
        int height;
        bool isFrameBufferResized = false;

        std::string windowName;

        GLFWwindow *window;
    };
}