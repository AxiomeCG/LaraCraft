
#include <glimac/Program.hpp>
#include <glimac/FilePath.hpp>

#include <glimac/common.hpp>

#include <glimac/FreeflyCamera.hpp>
#include <GL/glew.h>
#include <iostream>
#include <cstddef>
#include <glimac/Cube.hpp>
#include <glimac/GLFWWindowManager.hpp>
#include <glimac/TexturedCubeProgram.hpp>

using namespace glimac;

int main(int argc, char** argv) {
    GLFWWindowManager windowManager = GLFWWindowManager(800, 800, "LaraCraft", windowModes::Windowed);
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
    /*Program program = loadProgram(applicationPath.dirPath() + "shaders/3D.vs.glsl",
                                  applicationPath.dirPath() + "shaders/normals.fs.glsl");
    program.use();*/

    TexturedCubeProgram program = TexturedCubeProgram(applicationPath);

    program.m_Program.use();
    /*GLuint MVPMatrixLocation = glGetUniformLocation(program.getGLId(), "uMVPMatrix");
    GLuint MVMatrixLocation = glGetUniformLocation(program.getGLId(), "uMVMatrix");
    GLuint NormalMatrixLocation = glGetUniformLocation(program.getGLId(), "uNormalMatrix");*/

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

    FreeflyCamera camera;

    // Application loop:
    while (!windowManager.windowShouldClose()) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glBindVertexArray(vao);

        glm::mat4 VMatrix = camera.getViewMatrix();

        MVMatrix = VMatrix;

        glm::mat4 MVPMatrix = ProjMatrix * MVMatrix;

        NormalMatrix = glm::transpose(glm::inverse(MVMatrix));

        glm::mat4 lightMMatrix = glm::rotate(glm::mat4(), (float) glfwGetTime(), glm::vec3(0, 1, 0));
        glm::mat4 lightMVMatrix =  camera.getViewMatrix() * lightMMatrix;

        glUniformMatrix4fv(program.uMVPMatrixId, 1, GL_FALSE, glm::value_ptr(MVPMatrix));
        glUniformMatrix4fv(program.uMVMatrixId, 1, GL_FALSE, glm::value_ptr(MVMatrix));
        glUniformMatrix4fv(program.uNormalMatrixId, 1, GL_FALSE, glm::value_ptr(NormalMatrix));



        glUniform3fv(program.uKdId, 1, glm::value_ptr(glm::vec3(1., 1., 1.)));
        glUniform3fv(program.uKsId, 1, glm::value_ptr(glm::vec3(1., 1., 1.)));
        glUniform1f(program.uShininessId, 10.);

        glUniform3fv(program.uLightDir_vsId, 1, glm::value_ptr(glm::vec3(VMatrix * glm::vec4(1., 1., 1., 0.) * lightMMatrix))); // TODO Check if it's natural
        glUniform3fv(program.uLightIntensityId, 1, glm::value_ptr(glm::vec3(1., 1., 1.)));
        glDrawArrays(GL_TRIANGLES, 0, cube.getVertexCount());


        glBindVertexArray(0);

        windowManager.swapBuffers();
        windowManager.handleEventsForFPSview(camera);

    }
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);

    return EXIT_SUCCESS;
}