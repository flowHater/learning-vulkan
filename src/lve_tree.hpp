#pragma once

#include <glm/glm.hpp>


namespace lve {
    class LveTree
    {
    private:
        float radius;
        float height;


    public:
        LveTree(float radius, float height);
        ~LveTree();
    };
}