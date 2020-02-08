
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
#include <glimac/Chunk.hpp>
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

void generateAllChunks(const HeightMap &heightMap, const OffsetTextureMap &offsetTextureMap ,
                       std::vector<std::vector<Chunk>> &chunkVector) {
    std::cout << "Generating the terrain ..." << std::endl;
    for (int i = 0; i < heightMap.getWidth(); i += 16) {
        std::vector<Chunk> columnVector;
        for (int j = 0; j < heightMap.getHeight(); j += 16) {
            columnVector.emplace_back(vec2(i, j), heightMap, offsetTextureMap);
        }
        chunkVector.push_back(columnVector);
    }
}


void generateSurroundingChunkVertexFromAllChunks(std::vector<std::vector<Chunk>> chunkList, long &globalCount,
                                                 std::vector<ShapeVertex> &concatDataList, int offsetX1, int offsetX2,
                                                 int offsetZ1,
                                                 int offsetZ2) {

    /*std::cout << "Generate chunk for [" << offsetX1 << " " << offsetX2 << "]" << "[" << offsetZ1 << ";" << offsetZ2
              << "]" << std::endl;*/
    for (int i = offsetX1 < 0 ? 0 : offsetX1 * 16; i < offsetX2 * 16 && i < chunkList.size() * 16; i += 16) {
        auto chunkNumberI = i / 16;
        for (int j = offsetZ1 < 0 ? 0 : offsetZ1 * 16;
             j < offsetZ2 * 16 && j < chunkList[i / 16].size() * 16; j += 16) {
            auto chunkNumberJ = j / 16;
            auto vector = chunkList[chunkNumberI][chunkNumberJ].getDataVector();
            globalCount += chunkList[i / 16][j / 16].getVertexCount();
            concatDataList.insert(concatDataList.end(), vector.begin(), vector.end());
        }
    }
}


void refreshChunkVBO(const std::vector<std::vector<Chunk>> &chunkList, std::vector<ShapeVertex> &concatDataList,
                     long &globalNumberOfVertex, GLuint &vbo, int currentChunkX, int currentChunkZ,
                     int distanceChunkLoaded) {

    std::cout << "Refresh : size :" << chunkList.size() << " " << chunkList[0].size() << std::endl;
    concatDataList.clear();
    generateSurroundingChunkVertexFromAllChunks(chunkList, globalNumberOfVertex, concatDataList,
                                                currentChunkX - distanceChunkLoaded,
                                                currentChunkX + distanceChunkLoaded,
                                                currentChunkZ - distanceChunkLoaded,
                                                currentChunkZ + distanceChunkLoaded);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, concatDataList.size() * sizeof(ShapeVertex), &concatDataList[0], GL_DYNAMIC_DRAW);
    // Debinding d'un VBO sur la cible GL_ARRAY_BUFFER:
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void whereAmI(const FreeflyCamera &freeflyCamera, int &chunkNumberX, int &chunkNumberZ) {
    chunkNumberX = (int) freeflyCamera.getPosition()
                                .x / 16;
    chunkNumberZ = (int) freeflyCamera.getPosition()
                                .z / 16;
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
    simpleTexturedCubeProgram.m_Program
                             .use();


    /***************
     * TEXTURE INIT
     ***************/

    std::unique_ptr<Image> atlasImagePtr = loadImage(
            "TP_Mastercraft/assets/textures/blocks/atlas.png");
    assert(atlasImagePtr != nullptr);

    std::unique_ptr<Image> dirtImagePtr = loadImage(
            "TP_Mastercraft/assets/textures/blocks/dirt.png");
    assert(dirtImagePtr != nullptr);

    std::unique_ptr<HeightMap> heightMapPtr = loadHeightMap(
            "TP_Mastercraft/assets/terrain/224_224_perlin/map.png", 1.0f, 1.0f, 0.2f);
    assert(heightMapPtr != nullptr);

    std::cout << heightMapPtr->getWidth() << std::endl;
    auto ptr = heightMapPtr->getHeightData();

    std::unique_ptr<ColorMap> colorMapPtr = loadColorMap(
            "TP_Mastercraft/assets/terrain/224_224_perlin/color.png", 1.0f, 1.0f, 1.0f);
    assert(colorMapPtr != nullptr);
    auto ptrColor = colorMapPtr->getColorData();

    /*for (int i = 0; i < colorMapPtr->getWidth(); ++i) {
        for (int j = 0; j < colorMapPtr->getHeight(); ++j) {
            std::string s = (vec3(colorMapPtr->getColorData()[i][j]) == vec3(255, 255, 0)) ? "J" : "V";
            std::cout << s << ", ";
        }
        std::cout << ";" << std::endl;

    }*/

    GLuint atlasTextureLocation;

    glGenTextures(1, &atlasTextureLocation);


    /**
     * Atlas texture
     */
    glBindTexture(GL_TEXTURE_2D, atlasTextureLocation);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, atlasImagePtr->getWidth(), atlasImagePtr->getHeight(), 0, GL_RGBA, GL_FLOAT,
                 atlasImagePtr->getPixels());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
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

    int distanceChunkLoaded = 5;

    GLuint vbo, vao;

    glGenBuffers(1, &vbo);

    std::vector<std::vector<Chunk>> chunkList;

    auto offsetTextureMap = OffsetTextureMap(*colorMapPtr);

    generateAllChunks(*heightMapPtr, offsetTextureMap, chunkList);
    refreshChunkVBO(chunkList, concatDataList, globalCount, vbo, 0, 0, distanceChunkLoaded);

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

    FreeflyCamera camera = FreeflyCamera(glm::vec3(0., ((float) ptr[0][5]), 5.));

    // Application loop:


    int oldChunkPositionX = 0;
    int oldChunkPositionZ = 0;
    int currentChunkPositionX = 0;
    int currentChunkPositionZ = 0;
    while (!windowManager.windowShouldClose()) {

        whereAmI(camera, currentChunkPositionX, currentChunkPositionZ);
        if (currentChunkPositionX < oldChunkPositionX - distanceChunkLoaded / 2 ||
            currentChunkPositionX > oldChunkPositionX + distanceChunkLoaded / 2 ||
            currentChunkPositionZ < oldChunkPositionZ - distanceChunkLoaded / 2 ||
            currentChunkPositionZ > oldChunkPositionZ + distanceChunkLoaded / 2) {
            std::cout << "Need reload" << "(" << currentChunkPositionX << ";" << currentChunkPositionZ << ")"
                      << std::endl;
            refreshChunkVBO(chunkList, concatDataList, globalCount, vbo, currentChunkPositionX,
                            currentChunkPositionZ, distanceChunkLoaded);
            oldChunkPositionX = currentChunkPositionX;
            oldChunkPositionZ = currentChunkPositionZ;
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBindVertexArray(vao);

        viewMatrix = camera.getViewMatrix();

        //Light object
        DirectionalLight light = DirectionalLight(
                glm::rotate(glm::mat4(), (float) glfwGetTime(), glm::vec3(1., 0., 0.)));
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, atlasTextureLocation);
        glUniform1i(simpleTexturedCubeProgram.uTextureId, 0);


        draw(simpleTexturedCubeProgram, globalCount, projMatrix, viewMatrix, mat4(), light);

        //Flush texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, 0);

        glBindVertexArray(0);

        windowManager.swapBuffers();
        windowManager.handleEventsForFPSview(camera);


    }
    glDeleteTextures(1, &atlasTextureLocation);
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);

    return EXIT_SUCCESS;
}

