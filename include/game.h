#ifndef GAME_H
#define GAME_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <shader.h>
#include <camera.h>
#include <models.h>

#include <constants.h>

class Game {
    public:
        Game(float width, float height, char *title);

        void update();
        void draw();
        void handle_input();
        void exit();

        bool is_running();

        inline static float width;
        inline static float height;

    private:
        GLFWwindow *window;

        Shader *myShader;
        Shader *flatShader;

        unsigned int VAO, VBO, EBO;
        float flat_vertices[20] = {
            0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
            0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 
            -0.5f,  0.5f, 0.0f, 0.0f, 1.0f
        };

        unsigned int indices[6] = {
            0, 1, 3, // first triangle
            1, 2, 3  // second triangle
        };

        unsigned int terrain, split;

        float deltaTime = 0.0f;
        float lastFrame = 0.0f;

        float textures[16][5 * 6 * 6];

        int tex_count;


        bool tabEnabled = true;


};

#endif
