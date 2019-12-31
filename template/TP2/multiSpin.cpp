#include <glimac/SDLWindowManager.hpp>
#include <glimac/Program.hpp>
#include <glimac/FilePath.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cstddef>

using namespace glimac;


glm::mat3 rotate(float a) {
    a *= M_PI / 180.;
    return glm::mat3(glm::vec3(cos(a), sin(a), 0), glm::vec3(-sin(a), cos(a), 0), glm::vec3(0., 0., 1.));
}

glm::mat3 translate(float tx, float ty) {
    return glm::mat3(glm::vec3(1, 0, 0), glm::vec3(0, 1, 0), glm::vec3(tx, ty, 1));
}

glm::mat3 scale(float sx, float sy) {
    return glm::mat3(glm::vec3(sx, 0, 0), glm::vec3(0, sy, 0), glm::vec3(0, 0, 1));
}

struct Vertex2DUV {
    glm::vec2 position;
    glm::vec2 texture;

    Vertex2DUV(){}
    Vertex2DUV(glm::vec2 pos_vec, glm::vec2 tex_vec):position(pos_vec), texture(tex_vec) {}
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
    GLFWwindow* windowManager = glfwCreateWindow(800, 800, "GLImac", NULL, NULL);
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

    FilePath applicationPath(argv[0]);
    Program program = loadProgram(applicationPath.dirPath() + "shaders/spin.vs.glsl",
                                  applicationPath.dirPath() + "shaders/spin.fs.glsl");
    program.use();


    auto locateTime = glGetUniformLocation(program.getGLId(), "uTime");

    auto locateMatrix = glGetUniformLocation(program.getGLId(), "uModelMatrix");

    float angle = 1.;

    //glUniform1f(locateTime, angle);

    /*********************************
     * HERE SHOULD COME THE INITIALIZATION CODE
     *********************************/

    // Création d'un seul VBO:
    GLuint vbo;
    glGenBuffers(1, &vbo);
    // A partir de ce point, la variable vbo contient l'identifiant d'un VBO

    // Binding d'un VBO sur la cible GL_ARRAY_BUFFER:
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    // On peut à présent modifier le VBO en passant par la cible GL_ARRAY_BUFFER

    Vertex2DUV vertices[] = {
            Vertex2DUV(glm::vec2(0., 1.), glm::vec2(0., 0.)),
            Vertex2DUV(glm::vec2(-1., -1.), glm::vec2(0., 0.)),
            Vertex2DUV(glm::vec2(1., -1.), glm::vec2(0., 0.))
    };
    glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(Vertex2DUV), vertices, GL_STATIC_DRAW);

    // Debinding d'un VBO sur la cible GL_ARRAY_BUFFER:
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Création d'un seul VAO:
    GLuint vao;
    glGenVertexArrays(1, &vao);

    // Binding de l'unique VAO:
    glBindVertexArray(vao);

    const GLuint VERTEX_ATTR_POSITION = 0;
    const GLuint VERTEX_ATTR_TEXTURE = 1;
    glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
    glEnableVertexAttribArray(VERTEX_ATTR_TEXTURE);

    // Re-binding du VBO sur la cible GL_ARRAY_BUFFER pour glVertexAttribPointer:
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // Specification des attributs de position :
    glVertexAttribPointer(VERTEX_ATTR_POSITION, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2DUV), (const GLvoid *) offsetof(Vertex2DUV, position));
    glVertexAttribPointer(VERTEX_ATTR_TEXTURE, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2DUV), (const GLvoid *) offsetof(Vertex2DUV, texture));

    // Debinding d'un VBO sur la cible GL_ARRAY_BUFFER:
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Debinding de l'unique VAO:
    glBindVertexArray(0);

    glm::mat3 S = scale(0.5, 0.5);

    glm::mat3 R, T, M;

    // Application loop:
    while (!glfwWindowShouldClose(windowManager)) {
        glClear(GL_COLOR_BUFFER_BIT);
        glBindVertexArray(vao);
        angle *= 1.001;
        R = rotate(angle);
        T = translate(0.5, 0.5);
        M = R * T * S * R;
        glUniformMatrix3fv(locateMatrix, 1, GL_FALSE, glm::value_ptr(M));
        glDrawArrays(GL_TRIANGLES, 0, 3);
        T = translate(-0.5, 0.5);
        M = R * T * S * R;
        glUniformMatrix3fv(locateMatrix, 1, GL_FALSE, glm::value_ptr(M));
        glDrawArrays(GL_TRIANGLES, 0, 3);
        T = translate(0.5, -0.5);
        M = R * T * S * R;
        glUniformMatrix3fv(locateMatrix, 1, GL_FALSE, glm::value_ptr(M));
        glDrawArrays(GL_TRIANGLES, 0, 3);
        T = translate(-0.5, -0.5);
        M = R * T * S * R;
        glUniformMatrix3fv(locateMatrix, 1, GL_FALSE, glm::value_ptr(M));
        glDrawArrays(GL_TRIANGLES, 0, 3);
        //glUniform1f(locateTime, angle);
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