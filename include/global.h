#ifndef GLOBAL_H
#define GLOBAL_H

#include <constants.h>
#include <camera.h>
#include <models.h>

#include <glm/glm.hpp>

class Global {
    public:
        static inline int world[MAP_HEIGHT][MAP_WIDTH][MAP_LENGTH];

        static inline float width;
        static inline float height;

        static inline Camera *myCamera;

        static inline glm::vec3 blockLookingAt = glm::vec3(-1, -1, -1);

        static inline int blockSelected = GLCRAFT::STONE;
};

#endif
