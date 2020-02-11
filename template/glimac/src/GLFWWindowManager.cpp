
#include "glimac/GLFWWindowManager.hpp"

GLFWWindowManager::GLFWWindowManager(int width, int height, const char *appName, windowModes::WindowMode windowMode)
        : width(width), height(height), windowMode(windowMode) {
    createWindow(appName);
}

GLFWWindowManager::~GLFWWindowManager() {
    glfwDestroyWindow(window);
    glfwTerminate();
}

void GLFWWindowManager::createWindow(const char *app_name) {
    if (!glfwInit()) {
        std::cerr << "Failed in initialization GLFW" << std::endl;
        return;
    }
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    if (windowMode == windowModes::Fullscreen) {
        GLFWmonitor *monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode *vidmode = glfwGetVideoMode(monitor);
        glfwWindowHint(GLFW_RED_BITS, vidmode->redBits);
        glfwWindowHint(GLFW_GREEN_BITS, vidmode->greenBits);
        glfwWindowHint(GLFW_BLUE_BITS, vidmode->blueBits);
        glfwWindowHint(GLFW_REFRESH_RATE, vidmode->refreshRate);
        width = vidmode->width;
        height = vidmode->height;
        window = glfwCreateWindow(width, height, app_name, monitor, nullptr);
    } else if (windowMode == windowModes::BorderlessWindowed) {
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
        glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
        window = glfwCreateWindow(width, height, app_name, nullptr, nullptr);
    } else {
        window = glfwCreateWindow(width, height, app_name, nullptr, nullptr);
    }
    glfwMakeContextCurrent(window);
}

GLFWwindow *GLFWWindowManager::getWindow() {
    return window;
}

int GLFWWindowManager::windowShouldClose() {
    return glfwWindowShouldClose(window);
}

void GLFWWindowManager::swapBuffers() {
    glfwSwapBuffers(window);
}

void GLFWWindowManager::pollEvent() {
    glfwPollEvents();
}

int GLFWWindowManager::getMouseButton(int mouseButtonId) {
    return glfwGetMouseButton(window, mouseButtonId);
}

int GLFWWindowManager::getKey(int key) {
    return glfwGetKey(window, key);
}

void GLFWWindowManager::getCursorPosition(double *pX, double *pY) {
    glfwGetCursorPos(window, pX, pY);
}

/**
 *
 * @param camera
 * @param oldX
 * @param oldY
 */
void GLFWWindowManager::handleEventsForFPSview(glimac::FreeflyCamera &camera) {
    glfwPollEvents();
    int stateUpKey = getKey(GLFW_KEY_W);
    if (stateUpKey == GLFW_PRESS) {
        camera.moveFront(0.5f);
    }
    int stateDownKey = getKey(GLFW_KEY_S);
    if (stateDownKey == GLFW_PRESS) {
        camera.moveFront(-0.5f);
    }
    int stateLeftKey = getKey(GLFW_KEY_A);
    if (stateLeftKey == GLFW_PRESS) {
        camera.moveLeft(0.5f);
    }
    int stateRightKey = getKey(GLFW_KEY_D);
    if (stateRightKey == GLFW_PRESS) {
        camera.moveLeft(-0.5f);
    }

    int stateEscapeKey = getKey(GLFW_KEY_ESCAPE);
    if (stateEscapeKey == GLFW_PRESS) {
        glfwSetWindowShouldClose(window,1);
    }
    double tmpxPos, tmpyPos;
    getCursorPosition(&tmpxPos, &tmpyPos);
    float xrel = oldX - (float) tmpxPos;
    float yrel = oldY - (float) tmpyPos;
    int stateMouseClick = getMouseButton(GLFW_MOUSE_BUTTON_LEFT);
    if (stateMouseClick == GLFW_PRESS) {
        camera.rotateUp(yrel);
        camera.rotateLeft(xrel);
    }
    oldX = (float) tmpxPos;
    oldY = (float) tmpyPos;
}

void GLFWWindowManager::handleEventsForFPSConstrainedView(glimac::ConstrainedCamera &camera, bool &isDay) {
    glfwPollEvents();
    int stateUpKey = getKey(GLFW_KEY_W);
    if (stateUpKey == GLFW_PRESS) {
        camera.moveFront(0.1f);
    }
    int stateDownKey = getKey(GLFW_KEY_S);
    if (stateDownKey == GLFW_PRESS) {
        camera.moveFront(-0.1f);
    }
    int stateLeftKey = getKey(GLFW_KEY_A);
    if (stateLeftKey == GLFW_PRESS) {
        camera.moveLeft(0.1f);
    }
    int stateRightKey = getKey(GLFW_KEY_D);
    if (stateRightKey == GLFW_PRESS) {
        camera.moveLeft(-0.1f);
    }

    int stateFlyingKey = getKey(GLFW_KEY_F);
    if(stateFlyingKey == GLFW_PRESS) {
        if (camera.iAmFlying()) {
            camera.stopFlying();
        } else {
            camera.startFlying();
        }
    }

    int stateDayKey = getKey(GLFW_KEY_E);
    if(stateDayKey == GLFW_PRESS) {
        isDay = !isDay;
    }

    int stateEscapeKey = getKey(GLFW_KEY_ESCAPE);
    if (stateEscapeKey == GLFW_PRESS) {
        glfwSetWindowShouldClose(window,1);
    }
    double tmpxPos, tmpyPos;
    getCursorPosition(&tmpxPos, &tmpyPos);
    float xrel = oldX - (float) tmpxPos;
    float yrel = oldY - (float) tmpyPos;
    int stateMouseClick = getMouseButton(GLFW_MOUSE_BUTTON_LEFT);
    if (stateMouseClick == GLFW_PRESS) {
        camera.rotateUp(yrel);
        camera.rotateLeft(xrel);
    }
    oldX = (float) tmpxPos;
    oldY = (float) tmpyPos;
}
