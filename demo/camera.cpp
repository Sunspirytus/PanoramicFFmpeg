#include "camera.h"
#include "glm/geometric.hpp"

Camera::Camera(const glm::vec3& position, const glm::vec3& lookat, const glm::vec3& up, float fovY, float aspectRatio)
{
    m_Position = position;
    m_Lookat = lookat;
    m_Up = up;
    m_FovY = fovY;
    m_AspectRatio = aspectRatio;
}

Camera::~Camera()
{

}

void Camera::GetViewportCornerDirection(glm::vec3* output)
{
    glm::vec3 ForwardDir = m_Lookat - m_Position;
    ForwardDir = glm::normalize(ForwardDir);
    glm::vec3 Left = glm::normalize(glm::cross(m_Up, ForwardDir));

    glm::vec3 U = glm::tan(glm::radians(m_FovY / 2)) * glm::length(ForwardDir) * glm::normalize(m_Up);
    glm::vec3 V = glm::length(U) * m_AspectRatio * Left;
    output[0] =  U + V + ForwardDir; //LeftTop
    output[1] =  U - V + ForwardDir; //RightTop
    output[2] = -U + V + ForwardDir; //LeftBottom
    output[3] = -U - V + ForwardDir; //RightBottom

}