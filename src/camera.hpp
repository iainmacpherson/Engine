#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/ext.hpp>

class Camera
{
public:

    enum CameraMovement {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT,
        UP,
        DOWN
    };

    /* Construct a Camera object and set up vectors based on the provided parameters. */
    Camera(glm::vec3 position = kPOSITION, glm::vec3 world_up = kWORLDUP, float yaw = kYAW, float pitch = kPITCH);

    /* Get the view transformation matrix using glm's lookAt method. */
    glm::mat4 GetViewMatrix();

    /* Update the position of the camera by moving in the indicated direction. */
    void Move(CameraMovement direction, float delta_time);

    /* Update the diretion the camera is looking and recalculate camera vectors accordingly. */
    void UpdateLookDirection(float yaw, float pitch, bool constrain_pitch = true);

    /* Update FoV */
    void UpdateFoV(float value);

private:
    /* Recalculate the camera vectors based on changes to yaw and pitch. */
    void updateCameraVectors();

public:
    glm::vec3 Position;
    glm::vec3 Front;
    float FoV;

private:
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    float Yaw;
    float Pitch;
    float MovementSpeed;
    float LookSensitivity;

    // default camera values

    static constexpr float kYAW            = -90.0f;
    static constexpr float kPITCH          = 0.0f;
    static constexpr float kSPEED          = 2.5f;
    static constexpr float kSENSITIVITY    = 0.1f;
    static constexpr float kFOV            = 45.0f;
    static constexpr float kMAXFOV         = 90.0f;
    static constexpr glm::vec3 kPOSITION   = {0.0f, 0.0f, 0.0f};
    static constexpr glm::vec3 kWORLDUP    = {0.0f, 1.0f, 0.0f};
 };

#endif  // CAMERA_HPP