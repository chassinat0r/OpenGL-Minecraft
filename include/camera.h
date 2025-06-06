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

        glm::vec3 getRayCastStart();
        glm::vec3 getRayCastFinish();
        glm::vec3 getRayCastDirection();

    private:
        bool firstMouse = true;
        float yaw = -90.0f;
        float pitch = 0.0f;
        float lastX;
        float lastY;

        float speed;
        float jumpHeight = 1.0f;
        float jumpProgress = 0.0f;
        bool jumping = false;

        glm::vec3 cameraPos;
        glm::vec3 lastCameraPos;
        glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
        glm::vec3 cameraMoveFront = glm::vec3(0.0f, 0.0f, -1.0f);

        glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f,  0.0f);
        bool floating = false;
        float velocityY = 0.0f;
        float maxVelocityY = 4.0f;
        float gravity = 0.2f;

        std::map<std::string, bool> controls{{"FORWARD", false}, {"BACKWARD", false}, {"LEFT", false}, {"RIGHT", false}, {"JUMP", false}};

        glm::vec3 direction;

};

#endif
