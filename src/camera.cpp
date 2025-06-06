#include <camera.h>
#include <glm/gtc/matrix_transform.hpp>
#include <global.h>

Camera::Camera() {

}

Camera::Camera(float x, float y, float z, float speed) {
    this->cameraPos = glm::vec3(x, y, z);
    this->lastCameraPos = cameraPos;
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
    glm::vec3 velocity = glm::vec3(0, 0, 0);
    lastCameraPos = cameraPos;
    if (controls["FORWARD"]) {
        velocity = speed * cameraMoveFront * deltaTime;
    } else if (controls["BACKWARD"]) {
        velocity = -speed * cameraMoveFront * deltaTime;
    }

    if (controls["LEFT"]) {
        velocity = -glm::normalize(glm::cross(cameraMoveFront, cameraUp)) * speed * deltaTime;
    } else if (controls["RIGHT"]) {
        velocity = glm::normalize(glm::cross(cameraMoveFront, cameraUp)) * speed * deltaTime;
    }

    cameraPos += velocity;

    if (controls["JUMP"] && !floating) {
        jumping = true;
    }

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

    floating = !blockBelow;

    if (jumping) {
        cameraPos[1] += jumpHeight * deltaTime * 3;
        jumpProgress += jumpHeight * deltaTime * 3;
        if (jumpProgress >= jumpHeight) {
            jumping = false;
            jumpProgress = 0.0f;
        }
    } else {
        if (floating) {
            velocityY += gravity;
        } else {
            velocityY = 0.0f;
        }
    }

    if (velocityY > maxVelocityY) {
        velocityY = maxVelocityY;
    }

    cameraPos[1] -= velocityY * deltaTime;

    bool revert = false;

    for (int x = (int)cameraPos[0]; x <= (int)cameraPos[0]+1; x++) {
        for (int y = (int)cameraPos[1]; y <= (int)cameraPos[1]+1; y++) {
            for (int z = (int)cameraPos[2]; z <= (int)cameraPos[2]+1; z++) {
                int rx = x + 0.5*MAP_LENGTH;
                int ry = y + 0.5*MAP_HEIGHT;
                int rz = z + 0.5*MAP_WIDTH;
                int block = Global::world[ry][rz][rx];
                if (block != -1) {
                    if (abs((float)x - cameraPos[0]) < 1.0f) {
                        if (abs((float)y - cameraPos[1]) < 1.0f || abs((float)y - (cameraPos[1] + 1)) < 1.0f) {
                            if (abs((float)z - cameraPos[2]) < 1.0f) {
                                revert = true;
                                break;
                                printf("Collision with camera at (%.2f, %.2f, %.2f) and block (%d, %d, %d)\n", cameraPos[0], cameraPos[1], cameraPos[2], x, y, z);
                            }
                        }
                    }
                }
            }
            if (revert) { break; }
        }
        if (revert) { break; }
    }

    if (revert) {
        cameraPos = lastCameraPos;
    }
}

void Camera::setControl(std::string control, bool state) {
    controls[control] = state;
}

glm::mat4 Camera::generateViewMatrix() {
    glm::mat4 view;
    view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    return view;
}


float Camera::getX() { return cameraPos[0]; }
float Camera::getY() { return cameraPos[1]; }
float Camera::getZ() { return cameraPos[2]; }

