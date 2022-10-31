#include "lve_window.hpp"

namespace lve {
    LveWindow::LveWindow(int w, int h, std::string name): width{w}, height{h}, windowName{name} {
        initWindow();
    }

    LveWindow::~LveWindow() {
        glfwDestroyWindow(window);

        glfwTerminate();
    }

    void LveWindow::initWindow() {
        if (glfwInit() != GLFW_TRUE) {
            throw "glfwInit()";
        }

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);

        glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);

        window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
    }
}