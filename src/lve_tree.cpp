#include "lve_tree.hpp"

#include "vector"

namespace lve
{
#define roundess 10

    LveTree::LveTree(float radius, float height)
        : radius { radius }
        , height { height }
    {
        std::vector<glm::vec3> base;

        base.reserve(10);

        for (int i = 0; i < roundess; i++)
        {
            float angle = i / roundess;
            float angleCos = glm::cos(angle);
            float angleSin = glm::sin(angle);

            base.push_back(glm::vec3());
        }
    }
} // namespace lve
