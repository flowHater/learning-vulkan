#include "lve_window.hpp"

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

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

        glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);

        glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);

        window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
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