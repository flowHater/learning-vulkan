#include "lve_window.hpp"
#include <GLFW/glfw3.h>
#include <iostream>

namespace lve
{
    LveWindow::LveWindow(int w, int h, std::string name)
        : width { w }
        , height { h }
        , windowName { name }
    {
        initWindow();
    }

    LveWindow::~LveWindow()
    {
        glfwDestroyWindow(window);

        glfwTerminate();
    }

    void LveWindow::initWindow()
    {
        if (glfwInit() != GLFW_TRUE)
        {
            throw "glfwInit()";
        }

        int countMonitor = 0;

        auto monitors = glfwGetMonitors(&countMonitor);
        if (monitors == nullptr)
        {
            throw "glfwGetMonitors()";
        }
        for (int i = 0; i < countMonitor; i++)
        {
            std::cout << "Monitor #" << i << "= " << glfwGetMonitorName(monitors[i]) << std::endl;
        }

        auto monitor = glfwGetPrimaryMonitor();
        auto mode = glfwGetVideoMode(monitor);
        std::cout << "Monitor " << glfwGetMonitorName(monitor) << " will be used." << std::endl;

        glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
        glfwWindowHint(GLFW_RED_BITS, mode->redBits);
        glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
        glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
        glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

        glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);

        glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);

        window = glfwCreateWindow(width, height, windowName.c_str(), monitor, nullptr);
        glfwSetWindowUserPointer(window, this);
        glfwSetFramebufferSizeCallback(window, frameBufferResizedCallback);
    }

    void LveWindow::createWindowSurface(VkInstance instance, VkSurfaceKHR* surface)
    {
        if (glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create window surface");
        }
    }

    void LveWindow::frameBufferResizedCallback(GLFWwindow* window, int width, int height)
    {
        auto lveWindow = reinterpret_cast<LveWindow*>(glfwGetWindowUserPointer(window));
        if (width % 2)
        {
            width++;
        }
        if (height % 2)
        {
            height++;
        }

        lveWindow->width = width;
        lveWindow->height = height;
        lveWindow->isFrameBufferResized = true;
    }
}