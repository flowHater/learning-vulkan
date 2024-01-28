#pragma once
#include "lve_game_object.hpp"
#include "lve_window.hpp"
#include <GLFW/glfw3.h>

namespace lve
{
    class KeyboardMovementController
    {
    public:
        struct KeyMappings
        {
            int moveLeft = GLFW_KEY_A;
            int moveRight = GLFW_KEY_D;
            int moveForward = GLFW_KEY_W;
            int moveBackward = GLFW_KEY_S;
            int moveUp = GLFW_KEY_SPACE;
            int moveDown = GLFW_KEY_LEFT_CONTROL;
            int lookLeft = GLFW_KEY_LEFT;
            int lookRight = GLFW_KEY_RIGHT;
            int lookUp = GLFW_KEY_UP;
            int lookDown = GLFW_KEY_DOWN;

            int escape = GLFW_KEY_ESCAPE;
        };
        void controll(GLFWwindow* window, float d, LveGameObject& gameObject);

        KeyMappings keys {};
        float moveSpeed { 3.0f };
        float lookSpeed { 2.5f };

    private:
        void moveInPlane(GLFWwindow* window, float d, LveGameObject& gameObject);
        void mouse(GLFWwindow* window, float d, LveGameObject& gameObject);
        void escape(GLFWwindow* window, int status);
    };

}