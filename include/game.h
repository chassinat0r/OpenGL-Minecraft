#ifndef GAME_H
#define GAME_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <shader.h>
#include <camera.h>

class Game {
    public:
        Game(float width, float height, char *title);

        void update();
        void handle_input();
        void exit();

        bool is_running();

        inline static float width;
        inline static float height;

        inline static Camera *myCamera;

    private:
        GLFWwindow *window;

        Shader *myShader;

        unsigned int VAO, VBO;

        unsigned int terrain;

        float deltaTime = 0.0f;
        float lastFrame = 0.0f;

        float textures[16][5 * 6 * 6];

        int tex_count;

};

#endif
