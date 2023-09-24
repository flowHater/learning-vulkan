#pragma once

#include "lve_window.hpp"

#include "lve_device.hpp"

#include "lve_renderer.hpp"

#include "gui.hpp"
#include "lve_descriptors.hpp"
#include "lve_game_object.hpp"

#include <memory>
#include <vector>

namespace lve
{
    class App
    {
    public:
        static constexpr int WIDTH = 1600;
        static constexpr int HEIGHT = 1200;

        App();
        ~App();

        App(const App&) = delete;
        App& operator=(const App&) = delete;

        void run();

    private:
        void loadGameObjects();

        LveWindow lveWindow { WIDTH, HEIGHT, "Vulkan Engine" };
        LveDevice lveDevice { lveWindow };
        LveRenderer lveRenderer { lveWindow, lveDevice };
        LveGui gui { lveDevice, lveWindow, lveRenderer };

        std::unique_ptr<LveDescriptorPool>
            globalPool {};
        LveGameObject::Map gameObjects;
    };
}
