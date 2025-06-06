#include <game.h>
#include <stdio.h>
#include <stb_image.h>

#include <models.h>

#include <global.h>

void mergeArrays(const float *vertices, int vertices_length,  int vert_stride, const float *texture_coords, int tex_length, int tex_stride, float *final_arr, int final_length) {
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

void mouse_btn_callback(GLFWwindow* window, int button, int action, int mods) {
    int rx = Global::blockLookingAt[0];
    int ry = Global::blockLookingAt[1];
    int rz = Global::blockLookingAt[2];
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        if (rx != -1 && ry != -1 && rz != -1) {
            Global::world[ry][rz][rx] = -1;
        }
    }
    // if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
    //     if (rx != -1 && ry != -1 && rz != -1) {
    //         Global::world[ry+1][rz][rx] = Global::blockSelected;
    //     }
    // }
}

Game::Game(float width, float height, char *title) {
    glfwInit(); // Init GLFW
    // Set GL version to OpenGL 3.3 and profile to core
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);

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
    glfwSetMouseButtonCallback(window, mouse_btn_callback);

    // // Grab mouse so the cursor is invisible and all movements update the window
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Load shader for rendering textures
    myShader = new Shader("shaders/default.vert", "shaders/default.frag");

    flatShader = new Shader("shaders/flat.vert", "shaders/flat.frag");

    // Enable blending so transparency shows in textures if necessary
    glEnable(GL_BLEND); 
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

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

        int block_length = sizeof(blockVertices) / sizeof(float);
        int tex_length = sizeof(texture) / sizeof(float);

        mergeArrays(blockVertices, block_length, 3, texture, tex_length, 2, textures[i], block_length + tex_length);
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

                if (y == 3 && x == 5 && z == 5) {
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

    glGenTextures(1, &split);
    glBindTexture(GL_TEXTURE_2D, split); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    data = stbi_load("assets/split.png", &w, &h, &nrc, 0);
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
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        Global::myCamera->setControl("JUMP", true);
    }
    if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS) {
        if (tabEnabled) {
            int cursorStatus = glfwGetInputMode(window, GLFW_CURSOR);
            if (cursorStatus == GLFW_CURSOR_DISABLED) {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            } 
            if (cursorStatus == GLFW_CURSOR_NORMAL) {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            }
        }

        tabEnabled = false;
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
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE) {
        Global::myCamera->setControl("JUMP", false);
    }
    if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_RELEASE) {
        tabEnabled = true;
    }
}

void Game::draw() {
    glBindTexture(GL_TEXTURE_2D, terrain);
    
    glClearColor(135.0f / 255.0f, 206.0f / 255.0f, 235.0f / 255.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    myShader->use();

    glm::mat4 view = Global::myCamera->generateViewMatrix();
    glm::mat4 projection    = glm::mat4(1.0f);
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

    flatShader->use();
    glBindTexture(GL_TEXTURE_2D, split);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(flat_vertices), flat_vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    glm::mat4 model = glm::mat4(1.0f); // Start with an identity matrix
    model = glm::scale(model, glm::vec3((Global::height / Global::width)*0.1, 0.1, 1));

    flatShader->setMatrix("model", model);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void Game::update() {
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    Global::myCamera->update(deltaTime);

    glm::vec3 rayCastPos = Global::myCamera->getRayCastStart();
    glm::vec3 rayCastFinish = Global::myCamera->getRayCastFinish();
    glm::vec3 rayCastDirection = Global::myCamera->getRayCastDirection();
    const float rayCastSpeed = 0.2f;
    glm::vec3 rayCastVelocity = rayCastSpeed * rayCastDirection;
    
    // printf("Ray cast start: %.2f, %.2f, %.2f\n", rayCastPos[0], rayCastPos[1], rayCastPos[2]);
    // printf("Ray cast finish: %.2f, %.2f, %.2f\n", rayCastFinish[0], rayCastFinish[1], rayCastFinish[2]);

    const float distanceToTravel = 3.0f;
    float distanceTravelled = 0.0f;

    Global::blockLookingAt = glm::vec3(-1, -1, -1);

    while (distanceTravelled < distanceToTravel) {
        int x = (int)std::round(rayCastPos[0]);
        int y = (int)std::round(rayCastPos[1]);
        int z = (int)std::round(rayCastPos[2]);

        int rx = x + 0.5*MAP_LENGTH;
        int ry = y + 0.5*MAP_HEIGHT;
        int rz = z + 0.5*MAP_WIDTH;

        int block = Global::world[ry][rz][rx];

        if (block != -1) {
            Global::blockLookingAt = glm::vec3(rx, ry, rz);
            break;
        }
        // printf("Block: (%d, %d, %d)\n", x, y, z);
        distanceTravelled += rayCastSpeed;

        rayCastPos += rayCastVelocity;
    }

    glfwSwapBuffers(window);
    glfwPollEvents();
}

void Game::exit() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
}


