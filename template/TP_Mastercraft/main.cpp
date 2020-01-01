
#include <glimac/Program.hpp>
#include <glimac/FilePath.hpp>

#include <glimac/common.hpp>

#include <glimac/FreeflyCamera.hpp>
#include <GL/glew.h>
#include <iostream>
#include <cstddef>
#include <glimac/Cube.hpp>
#include <glimac/GLFWWindowManager.hpp>
#include <glimac/SimpleTexturedCubeProgram.hpp>
#include <glimac/DirectionalLight.hpp>
#include <glimac/ChunkSection.hpp>
#include <glimac/Image.hpp>

using namespace glimac;



void drawACube(const SimpleTexturedCubeProgram &program, int vertexCount, const mat4 &projMatrix, const mat4 &viewMatrix,
               const mat4 &modelMatrix, const DirectionalLight &light) {
    mat4 ModelViewMatrix2 = viewMatrix * modelMatrix;
    mat4 ModelViewProjectionMatrix2 = projMatrix * ModelViewMatrix2;
    mat4 NormalMatrix2 = transpose(glm::inverse(ModelViewMatrix2));
    glUniformMatrix4fv(program.uMVPMatrixId, 1, GL_FALSE, value_ptr(ModelViewProjectionMatrix2));
    glUniformMatrix4fv(program.uMVMatrixId, 1, GL_FALSE, value_ptr(ModelViewMatrix2));
    glUniformMatrix4fv(program.uNormalMatrixId, 1, GL_FALSE, value_ptr(NormalMatrix2));

    const vec3 &kd2 = vec3(1., 1., 1.);
    const vec3 &ks2 = vec3(1., 1., 1.);
    const float shininess2 = 10.;
    glUniform3fv(program.uKdId, 1, value_ptr(kd2));
    glUniform3fv(program.uKsId, 1, value_ptr(ks2));
    glUniform1f(program.uShininessId, shininess2);

    glUniform3fv(program.uLightDir_vsId, 1, value_ptr(light.getLightDirection(viewMatrix))); // TODO Check if it's natural
    glUniform3fv(program.uLightIntensityId, 1, value_ptr(light.lightIntensity));
    glDrawArrays(GL_TRIANGLES, 0, vertexCount);
}

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

    SimpleTexturedCubeProgram simpleTexturedCubeProgram = SimpleTexturedCubeProgram(applicationPath);
    simpleTexturedCubeProgram.m_Program.use();


    /***************
     * TEXTURE INIT
     ***************/

    auto dirtImagePtr = loadImage(
            "TP_Mastercraft/assets/textures/blocks/dirt.png");
    assert(dirtImagePtr != nullptr);


    GLuint dirtTextureLocation;

    glGenTextures(1, &dirtTextureLocation);


    /**
     * Earth texture
     */
    glBindTexture(GL_TEXTURE_2D, dirtTextureLocation);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, dirtImagePtr->getWidth(), dirtImagePtr->getHeight(), 0, GL_RGBA, GL_FLOAT,
                 dirtImagePtr->getPixels());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);


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

    glm::mat4 projMatrix, viewMatrix;

    projMatrix = glm::perspective(glm::radians(70.f), 1.f, 0.1f, 100.f);

    FreeflyCamera camera = FreeflyCamera(glm::vec3(0.,1.,5.));

    // Application loop:
    ChunkSection chunkSection = ChunkSection(glm::vec3(0.,0.,0.));

    while (!windowManager.windowShouldClose()) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glBindVertexArray(vao);

        viewMatrix = camera.getViewMatrix();

        //Light object
        DirectionalLight light = DirectionalLight(glm::rotate(glm::mat4(), (float) 0, glm::vec3(0., 1., 0.)));
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, dirtTextureLocation);
        glUniform1i(simpleTexturedCubeProgram.uTextureId, 0);


        chunkSection.draw(simpleTexturedCubeProgram, cube.getVertexCount(), projMatrix, viewMatrix, light);


        //Flush texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, 0);

        glBindVertexArray(0);

        windowManager.swapBuffers();
        windowManager.handleEventsForFPSview(camera);


    }
    glDeleteTextures(1, &dirtTextureLocation);
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);

    return EXIT_SUCCESS;
}