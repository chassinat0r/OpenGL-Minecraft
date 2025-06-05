#ifndef GLOBAL_H
#define GLOBAL_H

#include <constants.h>
#include <camera.h>

class Global {
    public:
        static inline int world[MAP_HEIGHT][MAP_WIDTH][MAP_LENGTH];

        static inline float width;
        static inline float height;

        static inline Camera *myCamera;
};

#endif
