//
// Created by Matthias Hudelot on 30/12/2019.
//

#include "glupem/WindowManager"

WindowManager::WindowManager(int w, int h, const char* app_name, uint32_t window_mode) : width(w), height(h), windowMode(window_mode) {
    createWindow(app_name);
}

WindowManager::~WindowManager() {
    glfwDestroyWindow(window);
    glfwTerminate();
}

void WindowManager::createWindow(const char* app_name) {
    if (!glfwInit())
    {
        std::cerr << "Failed in initialization GLFW" << std::endl;
        return;
    }
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    if (windowMode == windowing_modes::Fullscreen) {
        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* vidmode = glfwGetVideoMode(monitor);
        glfwWindowHint(GLFW_RED_BITS, vidmode->redBits);
        glfwWindowHint(GLFW_GREEN_BITS, vidmode->greenBits);
        glfwWindowHint(GLFW_BLUE_BITS, vidmode->blueBits);
        glfwWindowHint(GLFW_REFRESH_RATE, vidmode->refreshRate);
        width = vidmode->width;
        height = vidmode->height;
        window = glfwCreateWindow(width, height, app_name, monitor, nullptr);
    }
    else if (windowMode == windowing_modes::BorderlessWindowed) {
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
        glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
        window = glfwCreateWindow(width, height, app_name, nullptr, nullptr);
    }
    else {
        window = glfwCreateWindow(width, height, app_name, nullptr, nullptr);
    }
    glfwMakeContextCurrent(windowManager);
}

GLFWwindow* WindowManager::getWindow() {
    return window;
}