//
// Created by Matthias Hudelot on 30/12/2019.
//

#ifndef LARACRAFT_WINDOWMANAGER_HPP
#define LARACRAFT_WINDOWMANAGER_HPP

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cstdint>
#include "FreeflyCamera.hpp"
#include "ConstrainedCamera.hpp"

namespace windowModes {
    enum WindowMode : uint32_t {
        None = 0,
        Fullscreen = 1,
        BorderlessWindowed = 2,
        Windowed = 3
    };
}

/**
 * Window manager to handle GLFW and simplify the code.
 */
class GLFWWindowManager {
public :
    /**
     * Constructor
     * @param width Width of the window
     * @param height Height of the window
     * @param appName Name of the application
     * @param windowMode Mode of the window
     */
    GLFWWindowManager(int width, int height, const char *appName, windowModes::WindowMode windowMode);

    /**
     * Destructor. Flushes the GLFW objects generated in the initialization
     */
    ~GLFWWindowManager();

    /**
     * Getter on the GLFW window object
     * @return GLFW window object
     */
    GLFWwindow *getWindow();

    /**
     * Returns a number that means if the window must be closed. Aimed to break the application loop
     * @return
     */
    int windowShouldClose();

    /**
     * Swaps the rendering buffers with GLFW
     */
    void swapBuffers();

    /**
     * Polls an event in the event queue
     */
    void pollEvent();

    /**
     * Returns the status of the asked mouse button
     * @param mouseButtonId GLFW id of the mouse button
     * @return int that represents the status of the asked key
     */
    int getMouseButton(int mouseButtonId);

    /**
     * Returns the status of the asked keyboard key
     * @param key GLFW id of the keyboard key
     * @return int that represents the status of the asked key
     */
    int getKey(int key);

    /**
     * Writes in the parameters to get the cursor position
     * @param pX pointer on the X value to write
     * @param pY pointer on the Y value to write
     */
    void getCursorPosition(double *pX, double *pY);

    /**
     * Handles the inputs received to simulate an FPS view with a FreeflyCamera
     * @see FreeflyCamera
     * @param camera
     */
    void handleEventsForFPSview(glimac::FreeflyCamera &camera);

    /**
     * Handles the inputs received to simulate an FPS view with a ConstrainedCamera
     * @see ConstrainedCamera
     * @param camera
     */
    void handleEventsForFPSConstrainedView(glimac::ConstrainedCamera &camera, bool &isDay);

private :
    /**
     * Initializes the main window
     * @param appName name of the application to initialize
     */
    void createWindow(const char *appName);

    /**
     * Pointer on the current GLFW window
     */
    GLFWwindow *window;
    /**
     * Window width
     */
    int width;
    /**
     * Window height
     */
    int height;
    /**
     * Mode of the window
     */
    uint32_t windowMode;

    /**
     * Old x position for managing the FPS view and relative X position of the mouse
     */
    float oldX;
    /**
     * Old y position for managing the FPS view and relative Y position of the mouse
     */
    float oldY;

    bool wasPressedBefore = false;
};

#endif //LARACRAFT_WINDOWMANAGER_HPP
