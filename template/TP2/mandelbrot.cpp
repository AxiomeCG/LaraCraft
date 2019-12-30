#include <glimac/SDLWindowManager.hpp>
#include <GL/glew.h>
#include <iostream>
#include <glimac/FilePath.hpp>
#include <glimac/Program.hpp>

using namespace glimac;


int main(int argc, char **argv) {
    // Initialize SDL and open a window
    SDLWindowManager windowManager(800, 800, "GLImac");

    // Initialize glew for OpenGL3+ support
    GLenum glewInitError = glewInit();
    if (GLEW_OK != glewInitError) {
        std::cerr << glewGetErrorString(glewInitError) << std::endl;
        return EXIT_FAILURE;
    }

    FilePath applicationPath(argv[0]);
    Program program = loadProgram(applicationPath.dirPath() + "shaders/mandelbrot.vs.glsl",
                                  applicationPath.dirPath() + "shaders/mandelbrot.fs.glsl");
    program.use();

    std::cout << "OpenGL Version : " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLEW Version : " << glewGetString(GLEW_VERSION) << std::endl;

    /*********************************
     * HERE SHOULD COME THE INITIALIZATION CODE
     *********************************/
// storing vertex in the GPU
    // vbo binding
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    // vbo filling
    glm::vec2 vertices[] = {
            glm::vec2(-1, -1),
            glm::vec2(-1, 1),
            glm::vec2(1, 1),
            glm::vec2(-1, -1),
            glm::vec2(1, 1),
            glm::vec2(1, -1),

    };
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(glm::vec2), vertices, GL_STATIC_DRAW);
    // vbo releasing
    glBindBuffer(0, vbo);

    // vertex specification
    // vba creation and binding
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    // Vertex attributes activation
    const GLuint VERTEX_ATTR_POSITION = 3;
    glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
    //Specification attributes of vertex
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(VERTEX_ATTR_POSITION, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2),0);
    glBindBuffer(0, vbo);
    glBindVertexArray(0);

    // Application loop:
    bool done = false;
    while (!done) {
        // Event loop:
        SDL_Event e;
        while (windowManager.pollEvent(e)) {
            if (e.type == SDL_QUIT) {
                done = true; // Leave the loop after this iteration
            }
        }

        /*********************************
         * HERE SHOULD COME THE RENDERING CODE
         *********************************/
        glClear(GL_COLOR_BUFFER_BIT);
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
        // Update the display
        windowManager.swapBuffers();
    }

    return EXIT_SUCCESS;
}
