#pragma once

#include "lve_window.hpp"
#include "lve_pipeline.hpp"
#include "lve_device.hpp"

namespace lve {
    class App {
        public:
            static constexpr int WIDTH = 1600;
            static constexpr int HEIGHT = 1200;

            void run();

        private:
            LveWindow lveWindow{WIDTH, HEIGHT, "Vulkan Engine"};
            LveDevice lveDevice{lveWindow};
            LvePipeline lvePipeline{lveDevice, "shaders/simple_shader.vert.spv", "shaders/simple_shader.frag.spv", LvePipeline::defaultPipelineConfigInfo(WIDTH, HEIGHT)};
    };
}
