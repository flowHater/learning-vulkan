#include "app.hpp"

namespace lve {
    void App::run() {
        while (!lveWindow.shouldClose()) {
            glfwPollEvents();
        }
    }
}