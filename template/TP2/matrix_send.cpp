#include <glimac/SDLWindowManager.hpp>
#include <GL/glew.h>
#include <iostream>
#include <glimac/Program.hpp>
#include <chrono>

using namespace glimac;
using clk = std::chrono::system_clock;
using sec = std::chrono::duration<double,std::milli>;

class Vertex2DUV {

public:
    glm::vec2 position;
    glm::vec2 texture;

    Vertex2DUV(glm::vec2 position, glm::vec2 texture) : position(position), texture(texture) {
    }

};

glm::mat3 translate(float tx, float ty) {
    glm::mat3 M = glm::mat3(
            glm::vec3(1, 0, 0), // col1
            glm::vec3(0, 1, 0), // col2
            glm::vec3(tx, ty, 1)// col3
    );
    return M;
}

glm::mat3 scale(float sx, float sy) {
    glm::mat3 M = glm::mat3(
            glm::vec3(sx, 0, 0), // col1
            glm::vec3(0, sy, 0), // col2
            glm::vec3(0, 0, 1)// col3
    );
    return M;
}

glm::mat3 rotate(float a) {
    glm::mat3 M = glm::mat3(
            glm::vec3(cos(glm::radians(a)), sin(glm::radians(a)), 0), // col1
            glm::vec3(-sin(glm::radians(a)), cos(glm::radians(a)), 0), // col2
            glm::vec3(0, 0, 1)// col3
    );
    return M;
}

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
    Program program = loadProgram(applicationPath.dirPath() + "shaders/tex2DSendMatrix.vs.glsl",
                                  applicationPath.dirPath() + "shaders/tex2DSendMatrix.fs.glsl");
    program.use();
    GLint uModelMatrixId = glGetUniformLocation(program.getGLId(), "uModelMatrix");
    std::cout << "OpenGL Version : " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLEW Version : " << glewGetString(GLEW_VERSION) << std::endl;

    /*********************************
     * HERE SHOULD COME THE INITIALIZATION CODE
     *********************************/

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    Vertex2DUV vertices[] = {
            Vertex2DUV(glm::vec2(-1, -1), glm::vec2(0, 0)),
            Vertex2DUV(glm::vec2(1, -1), glm::vec2(0, 0)),
            Vertex2DUV(glm::vec2(0, 1), glm::vec2(0, 0)),
    };
    glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(Vertex2DUV), vertices, GL_STATIC_DRAW);
    // vbo releasing
    glBindBuffer(0, vbo);

    // vertex specification
    // vba creation and binding
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    // Vertex attributes activation
    const GLuint VERTEX_ATTR_POSITION = 3;
    const GLuint VERTEX_ATTR_TEXTURE = 8;
    glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
    glEnableVertexAttribArray(VERTEX_ATTR_TEXTURE);
    //Specification attributes of vertex
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(VERTEX_ATTR_POSITION, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2DUV),
                          offsetof(Vertex2DUV, Vertex2DUV::position));
    glVertexAttribPointer(VERTEX_ATTR_TEXTURE, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2DUV),
                          (const GLvoid *) offsetof(Vertex2DUV, Vertex2DUV::texture));
    glBindBuffer(0, vbo);
    glBindVertexArray(0);
    // Application loop:
    bool done = false;
    float angle = 0;
    float degreeByMs = 0.01;
    auto before = clk::now();
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

        const sec duration = clk::now() - before;
        angle = angle + ((float) duration.count())*degreeByMs;
        before = clk::now();

        //std::cout << duration.count() << std::endl;
        glUniformMatrix3fv(uModelMatrixId, 1, GL_FALSE, glm::value_ptr(rotate(angle)));
        glClear(GL_COLOR_BUFFER_BIT);
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);
        // Update the display
        windowManager.swapBuffers();
    }

    return EXIT_SUCCESS;
}
