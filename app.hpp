#pragma once

#include "lve_window.hpp"

namespace lve {
    class App {
        public:
            static constexpr int WIDTH = 1600;
            static constexpr int HEIGHT = 1200;

            void run();

        private:
            LveWindow lveWindow{WIDTH, HEIGHT, "Vulkan Engine"};
    };
}
