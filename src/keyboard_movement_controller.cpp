#include "keyboard_movement_controller.hpp"
#include "imgui.h"

#include <GLFW/glfw3.h>
#include <glm/ext/scalar_constants.hpp>
#include <iostream>
#include <limits>

namespace lve
{
    void KeyboardMovementController::controll(GLFWwindow* window, float dt, LveGameObject& gameObject)
    {
        auto status = glfwGetInputMode(window, GLFW_CURSOR);

        escape(window, status);
        if (status == GLFW_CURSOR_DISABLED)
        {
            mouse(window, dt, gameObject);
        }
        moveInPlane(window, dt, gameObject);
    }

    void KeyboardMovementController::moveInPlane(GLFWwindow* window, float dt, LveGameObject& gameObject)
    {
        float yam = gameObject.transform.rotation.y;

        const glm::vec3 forwardDir { sin(yam), 0.f, cos(yam) };
        const glm::vec3 rightDir { forwardDir.z, 0.f, -forwardDir.x };
        const glm::vec3 upDir { 0.f, -1.f, 0.f };

        glm::vec3 moveDir { 0.f };
        if (glfwGetKey(window, keys.moveForward) == GLFW_PRESS)
            moveDir += forwardDir;
        if (glfwGetKey(window, keys.moveBackward) == GLFW_PRESS)
            moveDir -= forwardDir;
        if (glfwGetKey(window, keys.moveRight) == GLFW_PRESS)
            moveDir += rightDir;
        if (glfwGetKey(window, keys.moveLeft) == GLFW_PRESS)
            moveDir -= rightDir;
        if (glfwGetKey(window, keys.moveUp) == GLFW_PRESS)
            moveDir += upDir;
        if (glfwGetKey(window, keys.moveDown) == GLFW_PRESS)
            moveDir -= upDir;

        if (glm::dot(moveDir, moveDir) > std::numeric_limits<float>::epsilon())
            gameObject.transform.translation += moveSpeed * dt * glm::normalize(moveDir);
    }

    void KeyboardMovementController::mouse(GLFWwindow* window, float d, LveGameObject& gameObject)
    {
        int _h, _w = 0;
        double height, width;
        glfwGetWindowSize(window, &_w, &_h);
        height = static_cast<double>(_h);
        width = static_cast<double>(_w);
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        glfwSetCursorPos(window, width / 2.f, height / 2.f);

        glm::vec3 rotate { 0.f };

        auto relative_y = ypos - height / 2.f;
        auto relative_x = xpos - width / 2.f;

        rotate.x = -relative_y / height * glm::pi<double>() / 2.f;
        rotate.y = relative_x / width * glm::pi<double>() / 2.f;

        if (glm::dot(rotate, rotate) > std::numeric_limits<float>::epsilon())
        {
            gameObject.transform.rotation += lookSpeed * rotate;
        }
        gameObject.transform.rotation.x = glm::clamp(gameObject.transform.rotation.x, -1.5f, 1.5f);
        gameObject.transform.rotation.y = glm::mod(gameObject.transform.rotation.y, glm::two_pi<float>());
    }

    auto shouldBeReleased = false;
    void KeyboardMovementController::escape(GLFWwindow* window, int status)
    {
        ImGui::Text("Current status = %d", status);
        auto isPress = glfwGetKey(window, keys.escape) == GLFW_PRESS;
        ImGui::Text("Key Escape is relase = %d", isPress);
        auto focus = glfwGetWindowAttrib(window, GLFW_FOCUSED);
        ImGui::Text("Focused = %d", focus);

        if (isPress)
        {
            shouldBeReleased = true;
        }
        else if (shouldBeReleased)
        {
            shouldBeReleased = false;
            if (status == GLFW_CURSOR_DISABLED)
            {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            }
            if (status == GLFW_CURSOR_NORMAL)
            {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            }
        }
    }
}