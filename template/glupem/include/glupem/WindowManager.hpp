//
// Created by Matthias Hudelot on 30/12/2019.
//
#include <GLFW/glfw3.h>
#include <cstdint>

#ifndef LARACRAFT_WINDOWMANAGER_HPP
#define LARACRAFT_WINDOWMANAGER_HPP

namespace windowing_modes {
    enum e : uint32_t {
        None = 0,
        Fullscreen = 1,
        BorderlessWindowed = 2,
        Windowed = 3
    };
}

class WindowManager{
public :
    WindowManager(int w, int h, const char* app_name, uint32_t window_mode);
    ~WindowManager();

    GLFWwindow* getWindow();

private :
    void createWindow(const char* app_name);

    GLFWwindow* window;
    int width;
    int height;
    uint32_t windowMode;
};

#endif //LARACRAFT_WINDOWMANAGER_HPP
