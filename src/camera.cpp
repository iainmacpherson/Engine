#include "camera.hpp"

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera( glm::vec3 position, glm::vec3 world_up, float yaw, float pitch)
    :   MovementSpeed(kSPEED),
        LookSensitivity(kSENSITIVITY),
        FoV(kFOV)
{
    this->Position  = position;
    this->WorldUp   = world_up;
    this->Yaw       = yaw;
    this->Pitch     = pitch;
    updateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix()
{
    return glm::lookAt(this->Position, this->Position + this->Front, this->Up);
}

void Camera::Move(CameraMovement direction, float delta_time)
{
    float velocity = this->MovementSpeed * delta_time;
    switch (direction)
    {
        case FORWARD:   { this->Position += this->Front * velocity; } break;
        case BACKWARD:  { this->Position -= this->Front * velocity; } break;
        case RIGHT:     { this->Position += this->Right * velocity; } break;
        case LEFT:      { this->Position -= this->Right * velocity; } break;
        case UP:        { this->Position += this->Up * velocity; } break;
        case DOWN:      { this->Position -= this->Up * velocity; } break;
    }
}

void Camera::UpdateLookDirection(float yaw, float pitch, bool constrain_pitch)
{
    this->Yaw   += yaw   * this->LookSensitivity;
    this->Pitch += pitch * this->LookSensitivity;

    if (constrain_pitch)
    {
        if (this->Pitch > 89.0f) this->Pitch = 89.0f;
        if (this->Pitch < -89.0f) this->Pitch = -89.0f;
    }
    updateCameraVectors();
}

void Camera::UpdateFoV(float value)
{
    this->FoV -= value;
    if (this->FoV < 1.0f) this->FoV = 1.0f;
    if (this->FoV > kMAXFOV) this->FoV = kMAXFOV;
}

// -------------------------------- Private Methods -----------------------------------
void Camera::updateCameraVectors()
{
    // calculate a new front vector
    glm::vec3 front = {
        cos(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch)),  // x
        sin(glm::radians(this->Pitch)),                                 // y
        sin(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch))   // z
    };
    this->Front = glm::normalize(front);
    this->Right = glm::normalize(glm::cross(this->Front, this->WorldUp));
    this->Up    = glm::normalize(glm::cross(this->Right, this->Front));
}