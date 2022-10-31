#pragma once

#include "lve_window.hpp"
#include "lve_pipeline.hpp"

namespace lve {
    class App {
        public:
            static constexpr int WIDTH = 1600;
            static constexpr int HEIGHT = 1200;

            void run();

        private:
            LveWindow lveWindow{WIDTH, HEIGHT, "Vulkan Engine"};
            LvePipeline lvePipeline{"shaders/simple_shader.vert.spv", "shaders/simple_shader.frag.spv"};
    };
}
