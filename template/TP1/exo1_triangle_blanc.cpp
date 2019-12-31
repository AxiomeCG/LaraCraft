#include <glimac/SDLWindowManager.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>


using namespace glimac;

int main(int argc, char* argv[]) {

    if (!glfwInit())
    {
        std::cerr << "Failed in initialization GLFW" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // Initialize SDL and open a window
    //SDLWindowManager windowManager(800, 600, "GLImac");
    GLFWwindow* windowManager = glfwCreateWindow(800, 600, "GLImac", NULL, NULL);
    if (!windowManager)
    {
        std::cerr << "Failed in create Windows" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(windowManager);

    // Initialize glew for OpenGL3+ support
    glewExperimental = GL_TRUE;
    GLenum glewInitError = glewInit();
    if(GLEW_OK != glewInitError) {
        std::cerr << glewGetErrorString(glewInitError) << std::endl;
        return EXIT_FAILURE;
    }


    std::cout << "OpenGL Version : " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLEW Version : " << glewGetString(GLEW_VERSION) << std::endl;
    /*********************************
     * HERE SHOULD COME THE INITIALIZATION CODE
     *********************************/

    GLuint vbo;
    glGenBuffers(1, &vbo);


    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    GLfloat vertices[] = { -0.5f, -0.5f, 0.5f, -0.5f, 0.0f, 0.5f };

    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    GLuint vao;

    glGenVertexArrays(1, &vao);

    glBindVertexArray(vao);
    const GLuint VERTEX_ATTR_POSITION = 0;
    glEnableVertexAttribArray(VERTEX_ATTR_POSITION);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0,
            2, GL_FLOAT, GL_FALSE,
            2 * sizeof(GL_FLOAT), 0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    // Application loop:
//    bool done = false;
//    while(!done) {
//        // Event loop:
//        //SDL_Event e;
//        while(glfwPollEvents()) {
////            if(e.type == SDL_QUIT) {
////                done = true; // Leave the loop after this iteration
////            }
//            done = true;
//        }

        /*********************************
         * HERE SHOULD COME THE RENDERING CODE
         *********************************/

//        glClear(GL_COLOR_BUFFER_BIT);
//        glBindVertexArray(vao);
//        glDrawArrays(GL_TRIANGLES, 0, 3);
//        glBindVertexArray(0);
        // Update the display
        //windowManager.swapBuffers();
        while (!glfwWindowShouldClose(windowManager)) {
            glClear(GL_COLOR_BUFFER_BIT);
            glBindVertexArray(vao);
            glDrawArrays(GL_TRIANGLES, 0, 3);
            glBindVertexArray(0);
            glfwSwapBuffers(windowManager);
            glfwPollEvents();
        }
//    }
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);

    glfwDestroyWindow(windowManager);
    glfwTerminate();

    return EXIT_SUCCESS;
}
