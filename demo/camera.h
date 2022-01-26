#pragma once

#include "glm/glm.hpp"

class Camera
{
public:
    Camera(const glm::vec3& position, const glm::vec3& lookat, const glm::vec3& up, float fovY, float aspectRatio);
    ~Camera();

    void GetViewportCornerDirection(glm::vec3* output);

private:
    glm::vec3 m_Position;
    glm::vec3 m_Lookat;
    glm::vec3 m_Up;
    float m_FovY;
    float m_AspectRatio; // Width / Height
};