#include <glimac/SDLWindowManager.hpp>
#include <GL/glew.h>
#include <glimac/Program.hpp>
#include <glimac/FilePath.hpp>
#include <glimac/glm.hpp>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cstddef>

using namespace glimac;

struct Vertex2DColor {
    glm::vec2 _position;
    glm::vec3 _color;

    Vertex2DColor() {}

    Vertex2DColor(glm::vec2 position, glm::vec3 color) {
        _position = position;
        _color = color;
    }
};

int main(int argc, char** argv) {
    // Initialize glfw
    if (!glfwInit())
    {
        std::cerr << "Failed in initialization GLFW" << std::endl;
        return -1;
    }

    //Hint the GL version to use
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    //Create the window
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

    FilePath applicationPath(argv[0]);
    Program program = loadProgram(applicationPath.dirPath() + "shaders/triangle.vs.glsl",
                                  applicationPath.dirPath() + "shaders/triangle.fs.glsl");
    program.use();

    std::cout << "OpenGL Version : " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLEW Version : " << glewGetString(GLEW_VERSION) << std::endl;

    /*********************************
     * HERE SHOULD COME THE INITIALIZATION CODE
     *********************************/
    GLuint vbo;
    glGenBuffers(1, &vbo);


    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    /*GLfloat vertices[] = { -0.5f, -0.5f, 1.f, 0.f, 0.f, // premier sommet
                           0.5f, -0.5f, 0.f, 1.f, 0.f, // deuxième sommet
                           0.0f, 0.5f, 0.f, 0.f, 1.f // troisième sommet
    };*/

    Vertex2DColor vertices[] = {
            Vertex2DColor(glm::vec2(-0.5, 0.5), glm::vec3(1, 0, 0)),
            Vertex2DColor(glm::vec2(-0.5, -0.5), glm::vec3(0,1,0)),
            Vertex2DColor(glm::vec2(0.5, 0.5), glm::vec3(0,0,1)),
            Vertex2DColor(glm::vec2(0.5, -0.5), glm::vec3(1,0,0)),
            Vertex2DColor(glm::vec2(-0.5, -0.5), glm::vec3(0,1,0)),
            Vertex2DColor(glm::vec2(0.5, 0.5), glm::vec3(0,0,1))
    };

    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(Vertex2DColor), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    GLuint vao;

    glGenVertexArrays(1, &vao);

    glBindVertexArray(vao);

    const GLuint aVertexPosition = 3;
    const GLuint aVertexColor = 8;

    glEnableVertexAttribArray(aVertexPosition);
    glEnableVertexAttribArray(aVertexColor);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(aVertexPosition,
            2, GL_FLOAT, GL_FALSE,
            sizeof(Vertex2DColor), (const GLvoid *) offsetof(Vertex2DColor, _position));

    glVertexAttribPointer(aVertexColor,
            3, GL_FLOAT, GL_FALSE,
            sizeof(Vertex2DColor), (const GLvoid *) offsetof(Vertex2DColor, _color));

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    while (!glfwWindowShouldClose(windowManager)) {
        glClear(GL_COLOR_BUFFER_BIT);
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
        glfwSwapBuffers(windowManager);
        glfwPollEvents();
    }
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);

    glfwDestroyWindow(windowManager);
    glfwTerminate();

    return EXIT_SUCCESS;
}
