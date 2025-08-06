//
// Created by yuyzc on 2025/8/4.
//

#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace ShapeGame
{
    struct TransformComponent
    {
        glm::vec2 position{0.0f, 0.0f};
        float orientation = 0.0f; // In radians
        glm::vec2 scale{1.0f, 1.0f};

        glm::mat4 GetTransform() const
        {
            glm::mat4 rot = glm::rotate(glm::mat4(1.0f), orientation, {0.0f, 0.0f, 1.0f});
            return glm::translate(glm::mat4(1.0f), {position.x, position.y, 0.0f})
                   * rot
                   * glm::scale(glm::mat4(1.0f), {scale.x, scale.y, 1.0f});
        }
    };
}