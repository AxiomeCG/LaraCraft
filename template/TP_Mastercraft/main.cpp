
#include <glimac/Program.hpp>
#include <glimac/FilePath.hpp>

#include <glimac/common.hpp>

#include <glimac/FreeflyCamera.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cstddef>
#include <glimac/Cube.hpp>
#include <glimac/WindowManager.hpp>

using namespace glimac;

int main(int argc, char** argv) {
    // Initialize glfw
    /*if (!glfwInit())
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
    glfwMakeContextCurrent(windowManager);*/

    WindowManager windowManager = WindowManager(800, 800, "Matthias", 0);

    //glfwSetInputMode(windowManager, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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
    Program program = loadProgram(applicationPath.dirPath() + "shaders/3D.vs.glsl",
                                  applicationPath.dirPath() + "shaders/normals.fs.glsl");
    program.use();

    GLuint locateMVPMatrix = glGetUniformLocation(program.getGLId(), "uMVPMatrix");
    GLuint locateMVMatrix = glGetUniformLocation(program.getGLId(), "uMVMatrix");
    GLuint locateNormalMatrix = glGetUniformLocation(program.getGLId(), "uNormalMatrix");

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    /*********************************
     * HERE SHOULD COME THE INITIALIZATION CODE
     *********************************/

    Cube cube;

    GLuint vbo, vao;

    glGenBuffers(1, &vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferData(GL_ARRAY_BUFFER, cube.getVertexCount() * sizeof(ShapeVertex), cube.getDataPointer(), GL_STATIC_DRAW);

    // Debinding d'un VBO sur la cible GL_ARRAY_BUFFER:
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenVertexArrays(1, &vao);

    // Binding de l'unique VAO:
    glBindVertexArray(vao);

    const GLuint VERTEX_ATTR_POSITION = 0;
    const GLuint VERTEX_ATTR_NORMAL = 1;
    const GLuint VERTEX_ATTR_TEXTURE = 2;
    glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
    glEnableVertexAttribArray(VERTEX_ATTR_NORMAL);
    glEnableVertexAttribArray(VERTEX_ATTR_TEXTURE);

    // Re-binding du VBO sur la cible GL_ARRAY_BUFFER pour glVertexAttribPointer:
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // Specification des attributs de position :
    glVertexAttribPointer(VERTEX_ATTR_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(ShapeVertex), (const GLvoid *) offsetof(ShapeVertex, position));
    glVertexAttribPointer(VERTEX_ATTR_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(ShapeVertex), (const GLvoid *) offsetof(ShapeVertex, normal));
    glVertexAttribPointer(VERTEX_ATTR_TEXTURE, 2, GL_FLOAT, GL_FALSE, sizeof(ShapeVertex), (const GLvoid *) offsetof(ShapeVertex, texCoords));

    // Debinding d'un VBO sur la cible GL_ARRAY_BUFFER:
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Debinding de l'unique VAO:
    glBindVertexArray(0);

    glm::mat4 ProjMatrix, MVMatrix, NormalMatrix;

    ProjMatrix = glm::perspective(glm::radians(70.f), 1.f, 0.1f, 100.f);
    double xpos, ypos;
    xpos = ypos = 0;

    FreeflyCamera camera;
    // Application loop:
    while (!windowManager.windowShouldClose()) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glBindVertexArray(vao);

        glm::mat4 VMatrix = camera.getViewMatrix();

        MVMatrix = VMatrix;

        glm::mat4 MVPMatrix = ProjMatrix * MVMatrix;

        NormalMatrix = glm::transpose(glm::inverse(MVMatrix));

        glUniformMatrix4fv(locateMVPMatrix, 1, GL_FALSE, glm::value_ptr(MVPMatrix));
        glUniformMatrix4fv(locateMVMatrix, 1, GL_FALSE, glm::value_ptr(MVMatrix));
        glUniformMatrix4fv(locateNormalMatrix, 1, GL_FALSE, glm::value_ptr(NormalMatrix));

        glDrawArrays(GL_TRIANGLES, 0, cube.getVertexCount());


        glBindVertexArray(0);

        windowManager.swapBuffer();
        glfwPollEvents();
        int stateUpKey = windowManager.getKey(GLFW_KEY_W);
        if(stateUpKey == GLFW_PRESS) {
            camera.moveFront(0.1f);
        }
        int stateDownKey = windowManager.getKey(GLFW_KEY_S);
        if(stateDownKey == GLFW_PRESS) {
            camera.moveFront(-0.1f);
        }
        int stateLeftKey = windowManager.getKey(GLFW_KEY_A);
        if(stateLeftKey == GLFW_PRESS) {
            camera.moveLeft(0.1f);
        }
        int stateRightKey = windowManager.getKey(GLFW_KEY_D);
        if(stateRightKey == GLFW_PRESS) {
            camera.moveLeft(-0.1f);
        }
        double tmpxPos, tmpyPos;
        windowManager.getCursorPos(&tmpxPos, &tmpyPos);
        float xrel = xpos - tmpxPos;
        float yrel = ypos - tmpyPos;
        int stateMouseClick = windowManager.getMouseButton(GLFW_MOUSE_BUTTON_LEFT);
        if(stateMouseClick == GLFW_PRESS) {
            camera.rotateUp(yrel);
            camera.rotateLeft(xrel);
        }
        xpos = tmpxPos;
        ypos = tmpyPos;
    }
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);

    /*
    glfwDestroyWindow(windowManager);
    glfwTerminate();*/

    return EXIT_SUCCESS;
}