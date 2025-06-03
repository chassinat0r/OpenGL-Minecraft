#ifndef MODELS_H
#define MODELS_H

const float SLICE_X = 1.0f / 9.0f;
const float SLICE_Y = 1.0f / 3.0f;

float block[] = {
    -0.5f, -0.5f, -0.5f,
    0.5f, -0.5f, -0.5f,
    0.5f,  0.5f, -0.5f,
    0.5f,  0.5f, -0.5f,
    -0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,

    -0.5f, -0.5f,  0.5f,
    0.5f, -0.5f,  0.5f,
    0.5f,  0.5f,  0.5f,
    0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,

    -0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,

    0.5f,  0.5f,  0.5f,
    0.5f,  0.5f, -0.5f,
    0.5f, -0.5f, -0.5f,
    0.5f, -0.5f, -0.5f,
    0.5f, -0.5f,  0.5f,
    0.5f,  0.5f,  0.5f,

    -0.5f, -0.5f, -0.5f,
    0.5f, -0.5f, -0.5f,
    0.5f, -0.5f,  0.5f,
    0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f, -0.5f,

    -0.5f,  0.5f, -0.5f,
    0.5f,  0.5f, -0.5f,
    0.5f,  0.5f,  0.5f,
    0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f
};

float TEX_STONE[] = {
    0.0f, SLICE_Y,
    SLICE_X, SLICE_Y,
    SLICE_X, 2*SLICE_Y,
    SLICE_X, 2*SLICE_Y,
    0.0f, 2*SLICE_Y,
    0.0f, SLICE_Y,

    0.0f, SLICE_Y,
    SLICE_X, SLICE_Y,
    SLICE_X, 2*SLICE_Y,
    SLICE_X, 2*SLICE_Y,
    0.0f, 2*SLICE_Y,
    0.0f, SLICE_Y,

    0.0f, 2*SLICE_Y,
    SLICE_X, 2*SLICE_Y,
    SLICE_X, SLICE_Y,
    SLICE_X, SLICE_Y,
    0.0f, SLICE_Y,
    0.0f, 2*SLICE_Y,

    0.0f, 2*SLICE_Y,
    SLICE_X, 2*SLICE_Y,
    SLICE_X, SLICE_Y,
    SLICE_X, SLICE_Y,
    0.0f, SLICE_Y,
    0.0f, 2*SLICE_Y,

    0.0f, SLICE_Y,
    SLICE_X, SLICE_Y,
    SLICE_X, 0.0f,
    SLICE_X, 0.0f,
    0.0f, 0.0f,
    0.0f, SLICE_Y,

    0.0f, 1.0f,
    SLICE_X, 1.0f,
    SLICE_X, 2*SLICE_Y,
    SLICE_X, 2*SLICE_Y,
    0.0f, 2*SLICE_Y,
    0.0f, 1.0f
};

float TEX_COBBLESTONE[] = {
    SLICE_X, SLICE_Y,
    2*SLICE_X, SLICE_Y,
    2*SLICE_X, 2*SLICE_Y,
    2*SLICE_X, 2*SLICE_Y,
    SLICE_X, 2*SLICE_Y,
    SLICE_X, SLICE_Y,

    SLICE_X, SLICE_Y,
    2*SLICE_X, SLICE_Y,
    2*SLICE_X, 2*SLICE_Y,
    2*SLICE_X, 2*SLICE_Y,
    SLICE_X, 2*SLICE_Y,
    SLICE_X, SLICE_Y,

    SLICE_X, 2*SLICE_Y,
    2*SLICE_X, 2*SLICE_Y,
    2*SLICE_X, SLICE_Y,
    2*SLICE_X, SLICE_Y,
    SLICE_X, SLICE_Y,
    SLICE_X, 2*SLICE_Y,

    SLICE_X, 2*SLICE_Y,
    2*SLICE_X, 2*SLICE_Y,
    2*SLICE_X, SLICE_Y,
    2*SLICE_X, SLICE_Y,
    SLICE_X, SLICE_Y,
    SLICE_X, 2*SLICE_Y,

    SLICE_X, SLICE_Y,
    2*SLICE_X, SLICE_Y,
    2*SLICE_X, 0.0f,
    2*SLICE_X, 0.0f,
    SLICE_X, 0.0f,
    SLICE_X, SLICE_Y,

    SLICE_X, 1.0f,
    2*SLICE_X, 1.0f,
    2*SLICE_X, 2*SLICE_Y,
    2*SLICE_X, 2*SLICE_Y,
    SLICE_X, 2*SLICE_Y,
    SLICE_X, 1.0f
};
    
#endif
