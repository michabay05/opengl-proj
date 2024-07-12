#ifndef _WINDOW_H_
#define _WINDOW_H_

#include "GLFW/glfw3.h"

#include <stdbool.h>
#include <stdint.h>

#define MAX_KEY_INPUT 32

enum ButtonState {
    BS_IDLE,
    BS_PRESSED,
    BS_RELEASED,
    // BS_DOWN,
};

#define KEYS_COUNT GLFW_KEY_LAST

struct Input {
    enum ButtonState keys[KEYS_COUNT];
    // TODO: implement mouse input
};

struct Window {
    GLFWwindow *handle;
    uint32_t width;
    uint32_t height;
    struct Input input;

    // TODO: potentially add function pointers for the following
    // init, update, render, destroy
};

extern struct Window window;

bool window_create(uint32_t width, uint32_t height, const char *title);
void window_clear_input(void);

#endif // _WINDOW_H_
