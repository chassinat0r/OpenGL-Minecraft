#ifndef CAMERA_H
#define CAMERA_H

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <map>
#include <string>

class Camera {
    public:
        Camera(float x, float y, float z, float speed);
        Camera();

        void handleMouseMovement(GLFWwindow* window, double xpos, double ypos);
        void handleResize(int lastX, int lastY);

        void update(float deltaTime);

        glm::mat4 generateViewMatrix();

        float getX();
        float getY();
        float getZ();

        void setControl(std::string control, bool state);

    private:
        bool firstMouse = true;
        float yaw = -90.0f;
        float pitch = 0.0f;
        float lastX;
        float lastY;

        float speed;

        glm::vec3 cameraPos;
        glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
        glm::vec3 cameraMoveFront = glm::vec3(0.0f, 0.0f, -1.0f);

        glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f,  0.0f);

        std::map<std::string, bool> controls{{"FORWARD", false}, {"BACKWARD", false}, {"LEFT", false}, {"RIGHT", false}};

};
// class Camera {
//     public:
//         Camera(glm::vec3 cameraPos, float cameraSpeed);
//         Camera();

//         void mouse_callback(GLFWwindow* window, double xpos, double ypos);
//         void move_left(float deltaTime);
//         void move_right(float deltaTime);
//         void move_forward(float deltaTime);
//         void move_backward(float deltaTime);

//         glm::mat4 generate_view_matrix();
        
//         void handle_resize(int lastX, int lastY);

//         float get_x();
//         float get_y();
//         float get_z();

//         void move_down(float deltaTime);

//     private:
//         bool firstMouse = true;
//         float yaw = -90.0f;
//         float pitch = 0.0f;
//         float lastX;
//         float lastY;

//         float cameraSpeed;

//         glm::vec3 cameraPos;
//         glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
//         glm::vec3 cameraMoveFront = glm::vec3(0.0f, 0.0f, -1.0f);

//         glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f,  0.0f);
// };

#endif
