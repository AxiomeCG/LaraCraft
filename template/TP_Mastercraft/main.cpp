
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
#include <glimac/HeightMap.hpp>
#include <glimac/ColorMap.hpp>

using namespace glimac;


void
drawACube(const SimpleTexturedCubeProgram &program, int vertexCount, const mat4 &projMatrix, const mat4 &viewMatrix,
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

    glUniform3fv(program.uLightDir_vsId, 1,
                 value_ptr(light.getLightDirection(viewMatrix))); // TODO Check if it's natural
    glUniform3fv(program.uLightIntensityId, 1, value_ptr(light.lightIntensity));
    glDrawArrays(GL_TRIANGLES, 0, vertexCount);
}

void
draw(const SimpleTexturedCubeProgram &program, long vertexCount, const mat4 &projMatrix, const mat4 &viewMatrix,
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

    glUniform3fv(program.uLightDir_vsId, 1,
                 value_ptr(light.getLightDirection(viewMatrix))); // TODO Check if it's natural
    glUniform3fv(program.uLightIntensityId, 1, value_ptr(light.lightIntensity));
    glDrawArrays(GL_TRIANGLES, 0, vertexCount);
}

long concatDataVector(const std::unique_ptr<HeightMap> &heightMapPtr, long &globalCount, std::vector<ShapeVertex> &v) {
    for(int i = 0 ; i < heightMapPtr->getWidth() ; i+=16) {
        for( int j = 0 ; j < heightMapPtr->getHeight() ; j+=16) {
            auto chunkSection = ChunkSection(vec2(i, j), *heightMapPtr, i, j);
            auto vector = chunkSection.getDataVector();
            globalCount += chunkSection.getVertexCount();
            v.insert(v.end(), vector.begin(), vector.end());
        }
    }
    return globalCount;
}

int main(int argc, char **argv) {
    GLFWWindowManager windowManager = GLFWWindowManager(1920, 1920, "LaraCraft", windowModes::Windowed);
    // Initialize glew for OpenGL3+ support
    glewExperimental = GL_TRUE;
    GLenum glewInitError = glewInit();
    if (GLEW_OK != glewInitError) {
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

    std::unique_ptr<Image> dirtImagePtr = loadImage(
            "TP_Mastercraft/assets/textures/blocks/dirt.png");
    assert(dirtImagePtr != nullptr);

//    std::cout << dirtImagePtr->getWidth() << std::endl;
//    unsigned int size = dirtImagePtr->getWidth() * dirtImagePtr->getHeight();
//    auto ptr = dirtImagePtr->getPixels();
//    for(auto i = 0u; i < size; ++i) {
//        std::cout << ptr->r << std::endl;
//        std::cout << ptr->g << std::endl;
//        std::cout << ptr->b << std::endl;
//        std::cout << ptr->a << std::endl;
//        ++ptr;
//    }
    std::unique_ptr<HeightMap> heightMapPtr = loadHeightMap(
            "TP_Mastercraft/assets/map/perlin64_64.png", 1.0f, 1.0f, 1.0f);
    assert(heightMapPtr != nullptr);

    std::cout << heightMapPtr->getWidth() << std::endl;
    unsigned int heightMapWidth = heightMapPtr->getWidth();
    unsigned int heightMapHeight = heightMapPtr->getHeight();
    auto ptr = heightMapPtr->getHeightData();

    std::unique_ptr<ColorMap> colorMapPtr = loadColorMap(
            "TP_Mastercraft/assets/map/perlincolor64_64.png", 1.0f, 1.0f, 1.0f);
    assert(colorMapPtr != nullptr);
    std::cout << colorMapPtr->getWidth() << std::endl;
    unsigned int colorMapWidth = colorMapPtr->getWidth();
    unsigned int colorMapHeight = colorMapPtr->getHeight();
    auto ptrColor = colorMapPtr->getColorData();

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
    long globalCount = 0;
    std::vector<ShapeVertex> concatDataList;


    concatDataVector(heightMapPtr, globalCount, concatDataList);


    /*ChunkSection chunkSection = ChunkSection(glm::vec2(0., 0.),*heightMapPtr, 0, 0);
    ChunkSection chunkSection2 = ChunkSection(glm::vec2(32., 0.),*heightMapPtr, 16, 0);


    globalCount = chunkSection.getVertexCount() + chunkSection2.getVertexCount();
    auto vector = chunkSection.getDataVector();
    auto vector2 = chunkSection2.getDataVector();

    concatDataList.insert(concatDataList.end(), vector.begin(), vector.end());
    concatDataList.insert(concatDataList.end(), vector2.begin(), vector2.end());*/


    GLuint vbo, vao;

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    //glBufferData(GL_ARRAY_BUFFER, cube.getVertexCount() * sizeof(ShapeVertex), cube.getDataPointer(), GL_STATIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, concatDataList.size() * sizeof(ShapeVertex), &concatDataList[0], GL_STATIC_DRAW);
    //glBufferData(GL_ARRAY_BUFFER, chunkSection2.getVertexCount() * sizeof(ShapeVertex), chunkSection2.getDataPointer(), GL_STATIC_DRAW);
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
    glVertexAttribPointer(VERTEX_ATTR_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(ShapeVertex),
                          (const GLvoid *) offsetof(ShapeVertex, position));
    glVertexAttribPointer(VERTEX_ATTR_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(ShapeVertex),
                          (const GLvoid *) offsetof(ShapeVertex, normal));
    glVertexAttribPointer(VERTEX_ATTR_TEXTURE, 2, GL_FLOAT, GL_FALSE, sizeof(ShapeVertex),
                          (const GLvoid *) offsetof(ShapeVertex, texCoords));

    // Debinding d'un VBO sur la cible GL_ARRAY_BUFFER:
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Debinding de l'unique VAO:
    glBindVertexArray(0);

    glm::mat4 projMatrix, viewMatrix;

    projMatrix = glm::perspective(glm::radians(70.f), 1.f, 0.1f, 100.f);

    FreeflyCamera camera = FreeflyCamera(glm::vec3(0.,  ((float) ptr[0][0]) + 2, 5.)); //(float)ptr[0][0]


    /*
    std::cout << "Height map" << std::endl;
    for (int i = 0; i < 16; i++){
        for (int j = 0; j < 16; j++) {
            std::cout << heightMapPtr->getHeightData().at(i).at(j) << ", ";
        }
        std::cout << std::endl;
    }*/
    // Application loop:

    while (!windowManager.windowShouldClose()) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glBindVertexArray(vao);

        viewMatrix = camera.getViewMatrix();

        //Light object
        DirectionalLight light = DirectionalLight(glm::rotate(glm::mat4(), (float) glfwGetTime(), glm::vec3(1., 0., 0.)));
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, dirtTextureLocation);
        glUniform1i(simpleTexturedCubeProgram.uTextureId, 0);

        //glDrawArrays(GL_TRIANGLES,0,globalCount);
/*
        for (auto x = 0u; x < heightMapWidth; ++x) {
            for (auto z = 0u; z < heightMapHeight; ++z) {
                drawACube(simpleTexturedCubeProgram,cube.getVertexCount(),projMatrix,viewMatrix,scale(translate(mat4(),vec3((float)x,(float)ptr[x][z],(float)z)),vec3(0.5,0.5,0.5)),light);
            }
        } */

        draw(simpleTexturedCubeProgram, globalCount, projMatrix, viewMatrix, mat4(), light);

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