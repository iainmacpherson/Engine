// std library stuff
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stbi/stb_image.h>

#include "shader_program.hpp"
#include "camera.hpp"

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

// ---------------------------- Globals ----------------------------

// Global camera object.
static Camera camera;
// set true if the mouse is currently held by the window.
static bool grabbed_mouse = false;
static float mouse_last_x = 400.0f;
static float mouse_last_y = 300.0f;
// time between current frame and last frame
static float delta_time;
// time of the last frame
static float last_frame_time;

// ---------------------------- Forward Declarations ----------------------------

void resizeViewportCallback(GLFWwindow* window, int width, int height);
void mouseMovementCallback(GLFWwindow* window, double xpos, double ypos);
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
void processKeyboardInput(GLFWwindow* window);
unsigned int loadTexture(const char* path);

int main()
{
    // ---------------------------- GLFW Instantiation ----------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Engine", NULL, NULL);
    if (!window)
    {
        std::cout << "failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // ---------------------------- GLAD Instantiation ----------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialise GLAD" << std::endl;
        return -1;
    }

    // ---------------------------- OpenGL Instantiation ----------------------------
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glfwSetFramebufferSizeCallback(window, resizeViewportCallback);

    // ---------------------------- Shader Program Setup ----------------------------
    ShaderProgram lightingShader("../../shaders/lighting.vert", "../../shaders/lighting.frag");
    ShaderProgram lampShader("../../shaders/lamp.vert", "../../shaders/lamp.frag");

    // ---------------------------- Camera Setup ----------------------------
    camera = Camera();
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouseMovementCallback);
    glfwSetScrollCallback(window, scrollCallback);
    // ---------------------------- OpenGL Buffers Setup ----------------------------
    // TODO(iain): tidy all this up into a model class when we introduce model loading

    // cube
    float vertices[] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
    };

    glm::vec3 cubePositions[] = {
        glm::vec3(0.0f,  0.0f,  0.0f),
        glm::vec3(2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f,  2.0f, -2.5f),
        glm::vec3(1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    glm::vec3 pointLightPositions[] = {
        glm::vec3(0.7f,  0.2f,  2.0f),
        glm::vec3(2.3f, -3.3f, -4.0f),
        glm::vec3(-4.0f,  2.0f, -12.0f),
        glm::vec3(0.0f,  0.0f, -3.0f)
    };


    // Buffer objects

    // cube
    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // light source
    unsigned int lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // already contains the data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // ---------------------------- Textures Setup ----------------------------

    unsigned int diffuseMap = loadTexture("../../textures/box.png");
    unsigned int specularMap = loadTexture("../../textures/box_specular.png");

    // Tell openGL to test depth so it knows when something is behind something else.
    glEnable(GL_DEPTH_TEST);

    // ---------------------------- Loop Begin ----------------------------
    while (!glfwWindowShouldClose(window))
    {
        float current_frame_time = static_cast<float>(glfwGetTime());
        delta_time = current_frame_time - last_frame_time;
        last_frame_time = current_frame_time;

        // Rendering Commands
        glClearColor(0.01f, 0.01f, 0.01f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // TODO(iain): this needs to be cleaned up badly

        // Lights
        lightingShader.use();
        lightingShader.setUniformVec3("viewPos", camera.Position);
        lightingShader.setUniformFloat("material.shininess", 32.0f);
        // directional light
        lightingShader.setUniformVec3("dirLight.direction", glm::vec3(-0.2f, -1.0f, -0.3f));
        lightingShader.setUniformVec3("dirLight.ambient", glm::vec3(0.05f, 0.05f, 0.05f));
        lightingShader.setUniformVec3("dirLight.diffuse", glm::vec3(0.4f, 0.4f, 0.4f));
        lightingShader.setUniformVec3("dirLight.specular", glm::vec3(0.5f, 0.5f, 0.5f));
        // point light 0
        lightingShader.setUniformVec3("pointLights[0].position", pointLightPositions[0]);
        lightingShader.setUniformVec3("pointLights[0].ambient", glm::vec3(0.05f, 0.05f, 0.05f));
        lightingShader.setUniformVec3("pointLights[0].diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
        lightingShader.setUniformVec3("pointLights[0].specular", glm::vec3(1.0f, 1.0f, 1.0f));
        lightingShader.setUniformFloat("pointLights[0].constant", 1.0f);
        lightingShader.setUniformFloat("pointLights[0].linear", 0.09f);
        lightingShader.setUniformFloat("pointLights[0].quadratic", 0.032f);
        // point light 1
        lightingShader.setUniformVec3("pointLights[1].position", pointLightPositions[1]);
        lightingShader.setUniformVec3("pointLights[1].ambient", glm::vec3(0.05f, 0.05f, 0.05f));
        lightingShader.setUniformVec3("pointLights[1].diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
        lightingShader.setUniformVec3("pointLights[1].specular", glm::vec3(1.0f, 1.0f, 1.0f));
        lightingShader.setUniformFloat("pointLights[1].constant", 1.0f);
        lightingShader.setUniformFloat("pointLights[1].linear", 0.09f);
        lightingShader.setUniformFloat("pointLights[1].quadratic", 0.032f);
        // point light 2
        lightingShader.setUniformVec3("pointLights[2].position", pointLightPositions[2]);
        lightingShader.setUniformVec3("pointLights[2].ambient", glm::vec3(0.05f, 0.05f, 0.05f));
        lightingShader.setUniformVec3("pointLights[2].diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
        lightingShader.setUniformVec3("pointLights[2].specular", glm::vec3(1.0f, 1.0f, 1.0f));
        lightingShader.setUniformFloat("pointLights[2].constant", 1.0f);
        lightingShader.setUniformFloat("pointLights[2].linear", 0.09f);
        lightingShader.setUniformFloat("pointLights[2].quadratic", 0.032f);
        // point light 3
        lightingShader.setUniformVec3("pointLights[3].position", pointLightPositions[3]);
        lightingShader.setUniformVec3("pointLights[3].ambient", glm::vec3(0.05f, 0.05f, 0.05f));
        lightingShader.setUniformVec3("pointLights[3].diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
        lightingShader.setUniformVec3("pointLights[3].specular", glm::vec3(1.0f, 1.0f, 1.0f));
        lightingShader.setUniformFloat("pointLights[3].constant", 1.0f);
        lightingShader.setUniformFloat("pointLights[3].linear", 0.09f);
        lightingShader.setUniformFloat("pointLights[3].quadratic", 0.032f);
        // spot light
        lightingShader.setUniformVec3("spotLight.position", camera.Position);
        lightingShader.setUniformVec3("spotLight.direction", camera.Front);
        lightingShader.setUniformVec3("spotLight.ambient", glm::vec3(0.0f, 0.0f, 0.0f));
        lightingShader.setUniformVec3("spotLight.diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
        lightingShader.setUniformVec3("spotLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));
        lightingShader.setUniformFloat("spotLight.constant", 1.0f);
        lightingShader.setUniformFloat("spotLight.linear", 0.09f);
        lightingShader.setUniformFloat("spotLight.quadratic", 0.032f);
        lightingShader.setUniformFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
        lightingShader.setUniformFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

        // view/projection transforms
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 proj = glm::perspective(glm::radians(camera.FoV), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);
        lightingShader.setUniformMat4("proj", proj);
        lightingShader.setUniformMat4("view", view);
        // world transformation
        glm::mat4 model = glm::mat4(1.0f);
        lightingShader.setUniformMat4("model", model);

        // bind diffuse map
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseMap);
        // bind specular map
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specularMap);

        // render containers
        glBindVertexArray(VAO);
        for (unsigned int i = 0; i < 10; i++)
        {
            // calculate model matrix
            model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            lightingShader.setUniformMat4("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // also draw the lamp objects
        lampShader.use();
        lampShader.setUniformVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
        lampShader.setUniformMat4("proj", proj);
        lampShader.setUniformMat4("view", view);
        // draw as many as there are point lights
        glBindVertexArray(lightVAO);
        for (unsigned int i = 0; i < 4; i++)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, pointLightPositions[i]);
            model = glm::scale(model, glm::vec3(0.2f));
            lampShader.setUniformMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        processKeyboardInput(window);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    // ---------------------------- Loop End----------------------------

    // ---------------------------- Finish and Clean Up ----------------------------

    // de-allocate all resources
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwTerminate();
    return 0;
}

// ---------------------------- Helper Functions ----------------------------

/* Resize the OpenGL viewport when the window has been resized. */
void resizeViewportCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

/* Capture movement of the mouse. */
void mouseMovementCallback(GLFWwindow* window, double xpos, double ypos)
{
    if (!grabbed_mouse)
    {
        mouse_last_x = static_cast<float>(xpos);
        mouse_last_y = static_cast<float>(ypos);
        grabbed_mouse = true;
    }
    float xoffset = static_cast<float>(xpos) - mouse_last_x;
    float yoffset = mouse_last_y - static_cast<float>(ypos);
    mouse_last_x = static_cast<float>(xpos);
    mouse_last_y = static_cast<float>(ypos);

    camera.UpdateLookDirection(xoffset, yoffset);
}

/* Capture mouse scroll. */
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.UpdateFoV(static_cast<float>(yoffset));
}

/* Capture keyboard input from the user. */
void processKeyboardInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.Move(Camera::FORWARD, delta_time);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.Move(Camera::BACKWARD, delta_time);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.Move(Camera::LEFT, delta_time);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.Move(Camera::RIGHT, delta_time);
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        camera.Move(Camera::UP, delta_time);
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        camera.Move(Camera::DOWN, delta_time);
}

/* Load a 2D texture from a file. */
unsigned int loadTexture(const char* path)
{
    unsigned int texture;
    glGenTextures(1, &texture);

    int width, height, nr_channels;
    unsigned char* data = stbi_load(path, &width, &height, &nr_channels, 0);
    if (data)
    {
        GLenum format;
        if (nr_channels == 1)       format = GL_RED;
        else if (nr_channels == 3)  format = GL_RGB;
        else if (nr_channels == 4)  format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }
    else
    {
        std::cerr << "Failed to load texture." << std::endl;
    }
    stbi_image_free(data);

    return texture;
}