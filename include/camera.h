#ifndef CAMERA_H
#define CAMERA_H

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Camera {
    public:
        Camera(glm::vec3 cameraPos, float cameraSpeed);
        Camera();

        void mouse_callback(GLFWwindow* window, double xpos, double ypos);
        void move_left(float deltaTime);
        void move_right(float deltaTime);
        void move_forward(float deltaTime);
        void move_backward(float deltaTime);

        glm::mat4 generate_view_matrix();
        
        void handle_resize(int lastX, int lastY);

    private:
        bool firstMouse = true;
        float yaw = -90.0f;
        float pitch = 0.0f;
        float lastX;
        float lastY;

        float cameraSpeed;

        glm::vec3 cameraPos;
        glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
        glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f,  0.0f);
};

#endif
