#include <game.h>
#include <stdio.h>
#include <stb_image.h>

#include <models.h>

#include <global.h>

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
    Global::width = width;
    Global::height = height;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    Global::myCamera->handleMouseMovement(window, xpos, ypos);
}

Game::Game(float width, float height, char *title) {
    glfwInit(); // Init GLFW
    // Set GL version to OpenGL 3.3 and profile to core
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);

    window = glfwCreateWindow(width, height, title, NULL, NULL); // Create window with given width, height, and title
    // Check if window was created
    if (window == NULL) { 
        // If not created, output error message and terminate
        printf("Failed to create GLFW window\n");
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(window); // Set context to window, so all actions will be done in the window
    
    // Init GLAD and check for failure
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        printf("Failed to initialise GLAD\n");
        glfwTerminate();
        return;
    }

    // Create camera
    Global::myCamera = new Camera(0.0f, 10.0f, 0.0f, 5.0f);

    // Set function to call when the window is resized
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    // Set function to call when the cursor is moved
    glfwSetCursorPosCallback(window, mouse_callback);

    // Grab mouse so the cursor is invisible and all movements update the window
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Load shader for rendering textures
    myShader = new Shader("shaders/default.vert", "shaders/default.frag");

    // Enable blending so transparency shows in textures if necessary
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

    for (int y = -MAP_HEIGHT / 2; y < MAP_HEIGHT / 2; y++) {
        for (int z = -MAP_WIDTH / 2; z < MAP_WIDTH / 2; z++) {
            for (int x = -MAP_LENGTH / 2; x < MAP_LENGTH / 2; x++) {
                int rx = x + 0.5*MAP_LENGTH;
                int ry = y + 0.5*MAP_HEIGHT;
                int rz = z + 0.5*MAP_WIDTH;
                if (y == 0) {
                    Global::world[ry][rz][rx] = GLCRAFT::COBBLESTONE;
                } else if (y == 1) {
                    Global::world[ry][rz][rx] = GLCRAFT::GRASS;
                } else {
                    Global::world[ry][rz][rx] = -1;
                }

                if (y == 2 && x == 5 && z == 5) {
                    Global::world[ry][rz][rx] = GLCRAFT::LOG;
                }
            }
        }
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
        Global::myCamera->setControl("FORWARD", true);
        Global::myCamera->setControl("BACKWARD", false);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        Global::myCamera->setControl("BACKWARD", true);
        Global::myCamera->setControl("FORWARD", false);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        Global::myCamera->setControl("LEFT", true);
        Global::myCamera->setControl("RIGHT", false);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        Global::myCamera->setControl("RIGHT", true);
        Global::myCamera->setControl("LEFT", false);
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_RELEASE) {
        Global::myCamera->setControl("FORWARD", false);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_RELEASE) {
        Global::myCamera->setControl("BACKWARD", false);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_RELEASE) {
        Global::myCamera->setControl("LEFT", false);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_RELEASE) {
        Global::myCamera->setControl("RIGHT", false);
    }  
}

void Game::draw() {
    glClearColor(135.0f / 255.0f, 206.0f / 255.0f, 235.0f / 255.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBindTexture(GL_TEXTURE_2D, terrain);

    myShader->use();

    glm::mat4 view = Global::myCamera->generateViewMatrix();
    glm::mat4 projection    = glm::mat4(1.0f);
    // view  = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
    projection = glm::perspective(glm::radians(70.0f), Global::width / Global::height, 0.1f, 100.0f);
    myShader->setMatrix("projection", projection);
    myShader->setMatrix("view", view);

    int current_block = -1;

    for (int y = -MAP_HEIGHT / 2; y < MAP_HEIGHT / 2; y++) {
        for (int z = -MAP_WIDTH / 2; z < MAP_WIDTH / 2; z++) {
            for (int x = -MAP_LENGTH / 2; x < MAP_LENGTH / 2; x++) {
                int rx = x + 0.5*MAP_LENGTH;
                int ry = y + 0.5*MAP_HEIGHT;
                int rz = z + 0.5*MAP_WIDTH;
                int block = Global::world[ry][rz][rx];
                if (block != current_block && block != -1) {
                    current_block = block;
                    glBindBuffer(GL_ARRAY_BUFFER, VBO);
                    glBufferData(GL_ARRAY_BUFFER, sizeof(textures[block]), textures[block], GL_STATIC_DRAW);

                    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
                    glEnableVertexAttribArray(0);

                    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3*sizeof(float)));
                    glEnableVertexAttribArray(1);

                    glBindBuffer(GL_ARRAY_BUFFER, 0);
                    glBindVertexArray(0);

                    glBindVertexArray(VAO);
                }

                if (block != -1) {
                    glm::mat4 model = glm::mat4(1.0f);
                    glm::vec3 cube_pos = glm::vec3( (float) x, (float) y, (float) z );
                    model = glm::translate(model, cube_pos);
                    // model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));
                    myShader->setMatrix("model", model);

                    glDrawArrays(GL_TRIANGLES, 0, 36);
                }
            }
        }
    }
}

void Game::update() {
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    Global::myCamera->update(deltaTime);

    glfwSwapBuffers(window);
    glfwPollEvents();
}

void Game::exit() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
}


