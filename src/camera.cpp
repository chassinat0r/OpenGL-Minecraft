#include <camera.h>
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera() {

}

Camera::Camera(glm::vec3 cameraPos, float cameraSpeed) {
    this->cameraPos = cameraPos;
    this->cameraSpeed = cameraSpeed;
}

void Camera::mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse)
    {
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
}

void Camera::handle_resize(int lastX, int lastY) {
    this->lastX = lastX;
    this->lastY = lastY;
}

void Camera::move_left(float deltaTime) {
    cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed * deltaTime;
}

void Camera::move_right(float deltaTime) {
    cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed * deltaTime;
}

void Camera::move_forward(float deltaTime) {
    cameraPos += cameraSpeed * cameraFront * deltaTime;
}

void Camera::move_backward(float deltaTime) {
    cameraPos -= cameraSpeed * cameraFront * deltaTime;
}

glm::mat4 Camera::generate_view_matrix() {
    glm::mat4 view;
    view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    return view;
}
