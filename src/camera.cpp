#include <camera.h>
#include <glm/gtc/matrix_transform.hpp>
#include <global.h>

Camera::Camera() {

}

Camera::Camera(float x, float y, float z, float speed) {
    this->cameraPos = glm::vec3(x, y, z);
    this->speed = speed;
}

void Camera::handleMouseMovement(GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
  
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; 
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw   += xoffset;
    pitch += yoffset;

    if(pitch > 89.0f)
        pitch = 89.0f;
    if(pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    
    cameraFront = glm::normalize(direction);

    direction.y = 0;
    
    cameraMoveFront = glm::normalize(direction);
}

void Camera::handleResize(int lastX, int lastY) {
    this->lastX = lastX;
    this->lastY = lastY;
}

void Camera::update(float deltaTime) {
    bool blockBelow = false;
    for (int y = -MAP_HEIGHT / 2; y < MAP_HEIGHT / 2; y++) {
        for (int z = -MAP_WIDTH / 2; z < MAP_WIDTH / 2; z++) {
            for (int x = -MAP_LENGTH / 2; x < MAP_LENGTH / 2; x++) {
                int rx = x + 0.5*MAP_LENGTH;
                int ry = y + 0.5*MAP_HEIGHT;
                int rz = z + 0.5*MAP_WIDTH;
                int block = Global::world[ry][rz][rx];
                
                if (block != -1) {
                    if (abs((float)x - cameraPos[0]) < 1.0f) {
                        if (abs((float)y - cameraPos[1]) < 1.5f) {
                            if (abs((float)z - cameraPos[2]) < 1.0f) {
                                blockBelow = true;
                            }
                        }
                    }
                }

                if (blockBelow) { break; }
            }
        }
        if (blockBelow) { break; };
    }

    if (!blockBelow) {
        cameraPos[1] -= speed * deltaTime;
    } else {
        if (controls["FORWARD"]) {
           cameraPos += speed * cameraMoveFront * deltaTime;
        } else if (controls["BACKWARD"]) {
            cameraPos -= speed * cameraMoveFront * deltaTime;
        }

        if (controls["LEFT"]) {
            cameraPos -= glm::normalize(glm::cross(cameraMoveFront, cameraUp)) * speed * deltaTime;
        } else if (controls["RIGHT"]) {
            cameraPos += glm::normalize(glm::cross(cameraMoveFront, cameraUp)) * speed * deltaTime;
        }
    }
}

void Camera::setControl(std::string control, bool state) {
    controls[control] = state;
}

// void Camera::handle_resize(int lastX, int lastY) {
//     this->lastX = lastX;
//     this->lastY = lastY;
// }

// void Camera::move_left(float deltaTime) {
//     cameraPos -= glm::normalize(glm::cross(cameraMoveFront, cameraUp)) * cameraSpeed * deltaTime;
// }

// void Camera::move_right(float deltaTime) {
//     cameraPos += glm::normalize(glm::cross(cameraMoveFront, cameraUp)) * cameraSpeed * deltaTime;
// }

// void Camera::move_forward(float deltaTime) {
//     cameraPos += cameraSpeed * cameraMoveFront * deltaTime;
// }

// void Camera::move_backward(float deltaTime) {
//     cameraPos -= cameraSpeed * cameraMoveFront * deltaTime;
// }

glm::mat4 Camera::generateViewMatrix() {
    glm::mat4 view;
    view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    return view;
}


float Camera::getX() { return cameraPos[0]; }
float Camera::getY() { return cameraPos[1]; }
float Camera::getZ() { return cameraPos[2]; }

// void Camera::move_down(float deltaTime) {
//     cameraPos.y -= deltaTime * 4.9;
// }
