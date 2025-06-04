#include <game.h>
#include <stdio.h>
#include <stb_image.h>

#include <models.h>

void mergeArrays(float *vertices, int vertices_length,  int vert_stride, float *texture_coords, int tex_length, int tex_stride, float *final_arr, int final_length) {
	int final_stride = vert_stride + tex_stride; 
	int vertices_count = final_length / final_stride;

	int vert_pos = 0;
	int tex_pos = 0;
	int final_pos = 0;

	while (final_pos < final_length) {
		int vert_end = vert_pos + vert_stride;
		int tex_end = tex_pos + tex_stride;
		while (vert_pos < vert_end) {
			final_arr[final_pos] = vertices[vert_pos];
			final_pos++;
			vert_pos++;
		}
		while (tex_pos < tex_end) {
			final_arr[final_pos] = texture_coords[tex_pos];
			final_pos++;
			tex_pos++;
		}
	}
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
    Game::width = width;
    Game::height = height;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    Game::myCamera->mouse_callback(window, xpos, ypos);
}

Game::Game(float width, float height, char *title) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (window == NULL) {
        printf("Failed to create GLFW window\n");
        glfwTerminate();
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        printf("Failed to initialise GLAD\n");
        glfwTerminate();
    }

    myCamera = new Camera(glm::vec3(0.0f, 0.0f,  3.0f), 5.0f);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    myShader = new Shader("shaders/default.vert", "shaders/default.frag");

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    
    glGenTextures(1, &terrain);
    glBindTexture(GL_TEXTURE_2D, terrain); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    stbi_set_flip_vertically_on_load(true);

    int w, h, nrc;
    unsigned char *data = stbi_load("assets/terrain.png", &w, &h, &nrc, 0);

    tex_count = w / 16;

    for (int i = 0; i < tex_count; i++) {
        int j = i + 1;
        float texture[] = {
            (float)i*SLICE_X, SLICE_Y,
            (float)j*SLICE_X, SLICE_Y,
            (float)j*SLICE_X, 2*SLICE_Y,
            (float)j*SLICE_X, 2*SLICE_Y,
            (float)i*SLICE_X, 2*SLICE_Y,
            (float)i*SLICE_X, SLICE_Y,

            (float)i*SLICE_X, SLICE_Y,
            (float)j*SLICE_X, SLICE_Y,
            (float)j*SLICE_X, 2*SLICE_Y,
            (float)j*SLICE_X, 2*SLICE_Y,
            (float)i*SLICE_X, 2*SLICE_Y,
            (float)i*SLICE_X, SLICE_Y,

            (float)i*SLICE_X, 2*SLICE_Y,
            (float)j*SLICE_X, 2*SLICE_Y,
            (float)j*SLICE_X, SLICE_Y,
            (float)j*SLICE_X, SLICE_Y,
            (float)i*SLICE_X, SLICE_Y,
            (float)i*SLICE_X, 2*SLICE_Y,

            (float)i*SLICE_X, 2*SLICE_Y,
            (float)j*SLICE_X, 2*SLICE_Y,
            (float)j*SLICE_X, SLICE_Y,
            (float)j*SLICE_X, SLICE_Y,
            (float)i*SLICE_X, SLICE_Y,
            (float)i*SLICE_X, 2*SLICE_Y,

            (float)i*SLICE_X, SLICE_Y,
            (float)j*SLICE_X, SLICE_Y,
            (float)j*SLICE_X, 0.0f,
            (float)j*SLICE_X, 0.0f,
            (float)i*SLICE_X, 0.0f,
            (float)i*SLICE_X, SLICE_Y,

            (float)i*SLICE_X, 1.0f,
            (float)j*SLICE_X, 1.0f,
            (float)j*SLICE_X, 2*SLICE_Y,
            (float)j*SLICE_X, 2*SLICE_Y,
            (float)i*SLICE_X, 2*SLICE_Y,
            (float)i*SLICE_X, 1.0f
        };

        int block_length = sizeof(block) / sizeof(float);
        int tex_length = sizeof(texture) / sizeof(float);

        mergeArrays(block, block_length, 3, texture, tex_length, 2, textures[i], block_length + tex_length);
    }

    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load texture" << std::endl;
    }

    stbi_image_free(data);

    glEnable(GL_DEPTH_TEST);  
    myShader->use();

    framebuffer_size_callback(window, width, height);
}

bool Game::is_running() {
    return !glfwWindowShouldClose(window);
}

void Game::handle_input() {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        myCamera->move_forward(deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        myCamera->move_backward(deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        myCamera->move_left(deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        myCamera->move_right(deltaTime);
    }
}

void Game::update() {
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    glClearColor(135.0f / 255.0f, 206.0f / 255.0f, 235.0f / 255.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBindTexture(GL_TEXTURE_2D, terrain);

    myShader->use();

    glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  -1.0f,  0.0f), 
        glm::vec3( 1.0f,  -1.0f, 0.0f), 
        glm::vec3( 0.0f,  -1.0f, -1.0f), 
        glm::vec3( 1.0f,  -1.0f, -1.0f),
    };

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(textures[6]), textures[6], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glm::mat4 view = myCamera->generate_view_matrix();
    glm::mat4 projection    = glm::mat4(1.0f);
    // view  = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
    projection = glm::perspective(glm::radians(70.0f), (float)width / (float)height, 0.1f, 100.0f);
    myShader->setMatrix("projection", projection);
    myShader->setMatrix("view", view);

    glBindVertexArray(VAO);
    for (unsigned int i = 0; i < sizeof(cubePositions) / sizeof(glm::vec3); i++) {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, cubePositions[i]);
        // model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));
        myShader->setMatrix("model", model);

        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    glfwSwapBuffers(window);
    glfwPollEvents();
}

void Game::exit() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
}


