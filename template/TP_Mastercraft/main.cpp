
#include <glimac/Chunk.hpp>
#include <glimac/ColorMap.hpp>
#include <glimac/ConstrainedCamera.hpp>
#include <glimac/Cube.hpp>

#include <glimac/AntiCube.hpp>
#include <glimac/GLFWWindowManager.hpp>

#include <glimac/SimpleTexturedSkyboxProgram.hpp>

#include <glimac/DirectionalLight.hpp>
#include <glimac/FilePath.hpp>
#include <glimac/FreeflyCamera.hpp>
#include <glimac/GLFWWindowManager.hpp>
#include <glimac/GlobalProgram.hpp>
#include <glimac/HeightMap.hpp>
#include <glimac/Image.hpp>
#include <glimac/common.hpp>

#include <GL/glew.h>

#include <cstddef>
#include <iostream>
#include <glimac/Pnj.hpp>
#include <glimac/Sphere.hpp>

using namespace glimac;

void drawACube(const GlobalProgram &program, int vertexCount,
               const mat4 &projMatrix, const mat4 &viewMatrix,
               const mat4 &modelMatrix, const DirectionalLight &light) {
    mat4 ModelViewMatrix2 = viewMatrix * modelMatrix;
    mat4 ModelViewProjectionMatrix2 = projMatrix * ModelViewMatrix2;
    mat4 NormalMatrix2 = transpose(glm::inverse(ModelViewMatrix2));
    glUniformMatrix4fv(program.uMVPMatrixId, 1, GL_FALSE,
                       value_ptr(ModelViewProjectionMatrix2));
    glUniformMatrix4fv(program.uMVMatrixId, 1, GL_FALSE,
                       value_ptr(ModelViewMatrix2));
    glUniformMatrix4fv(program.uNormalMatrixId, 1, GL_FALSE,
                       value_ptr(NormalMatrix2));

    const vec3 &kd2 = vec3(1., 1., 1.);
    const vec3 &ks2 = vec3(1., 1., 1.);
    const float shininess2 = 10.;
    glUniform3fv(program.uKdId, 1, value_ptr(kd2));
    glUniform3fv(program.uKsId, 1, value_ptr(ks2));
    glUniform1f(program.uShininessId, shininess2);

    glUniform3fv(program.uLightDir_vsId, 1,
                 value_ptr(light.getLightDirection(
                         viewMatrix))); // TODO Check if it's natural
    glUniform3fv(program.uLightIntensityId, 1, value_ptr(light.lightIntensity));
    glDrawArrays(GL_TRIANGLES, 0, vertexCount);
}

void drawVAO(const GlobalProgram &program, long vertexCount,
             const mat4 &projMatrix, const mat4 &viewMatrix,
             const mat4 &modelMatrix, const DirectionalLight &light) {
    mat4 modelViewMatrix = viewMatrix * modelMatrix;
    mat4 modelViewProjectionMatrix = projMatrix * modelViewMatrix;
    mat4 normalMatrix = transpose(glm::inverse(modelViewMatrix));
    glUniformMatrix4fv(program.uMVPMatrixId, 1, GL_FALSE,
                       value_ptr(modelViewProjectionMatrix));
    glUniformMatrix4fv(program.uMVMatrixId, 1, GL_FALSE,
                       value_ptr(modelViewMatrix));
    glUniformMatrix4fv(program.uNormalMatrixId, 1, GL_FALSE,
                       value_ptr(normalMatrix));

    const vec3 &kd = vec3(1., 1., 1.);
    const vec3 &ks = vec3(1., 1., 1.);
    const float shininess = 10.;
    glUniform3fv(program.uKdId, 1, value_ptr(kd));
    glUniform3fv(program.uKsId, 1, value_ptr(ks));
    glUniform1f(program.uShininessId, shininess);

    glUniform3fv(program.uLightDir_vsId, 1,
                 value_ptr(light.getLightDirection(
                         viewMatrix))); // TODO Check if it's natural
    glUniform3fv(program.uLightIntensityId, 1, value_ptr(light.lightIntensity));
    glDrawArrays(GL_TRIANGLES, 0, vertexCount);
}


void
drawSkybox(const SimpleTexturedSkyboxProgram &program, long vertexCount, const mat4 &projMatrix, const mat4 &viewMatrix,
     const mat4 &modelMatrix) {
    mat4 modelViewMatrix = viewMatrix * modelMatrix ;
    mat4 modelViewProjectionMatrix = projMatrix * modelViewMatrix;
    mat4 normalMatrix = transpose(glm::inverse(modelViewMatrix));
    glUniformMatrix4fv(program.uMVPMatrixId, 1, GL_FALSE, value_ptr(modelViewProjectionMatrix));
    glUniformMatrix4fv(program.uMVMatrixId, 1, GL_FALSE, value_ptr(modelViewMatrix));
    glUniformMatrix4fv(program.uNormalMatrixId, 1, GL_FALSE, value_ptr(normalMatrix));

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

void generateSurroundingChunkVertexFromAllChunks(
        std::vector<std::vector<Chunk>> chunkList, long &globalCount,
        std::vector<ShapeVertex> &concatDataList, int offsetX1, int offsetX2,
        int offsetZ1, int offsetZ2) {

    /*std::cout << "Generate chunk for [" << offsetX1 << " " << offsetX2 << "]" <<
       "[" << offsetZ1 << ";" << offsetZ2
              << "]" << std::endl;*/
    for (int i = offsetX1 < 0 ? 0 : offsetX1 * 16;
         i < offsetX2 * 16 && i < chunkList.size() * 16; i += 16) {
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

void refreshChunkVBO(const std::vector<std::vector<Chunk>> &chunkList,
                     std::vector<ShapeVertex> &concatDataList,
                     long &globalNumberOfVertex, GLuint &vbo, int currentChunkX,
                     int currentChunkZ, int distanceChunkLoaded) {

    std::cout << "Refresh : size :" << chunkList.size() << " "
              << chunkList[0].size() << std::endl;

    concatDataList.clear();
    generateSurroundingChunkVertexFromAllChunks(
            chunkList, globalNumberOfVertex, concatDataList,
            currentChunkX - distanceChunkLoaded, currentChunkX + distanceChunkLoaded,
            currentChunkZ - distanceChunkLoaded, currentChunkZ + distanceChunkLoaded);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, concatDataList.size() * sizeof(ShapeVertex),
                 &concatDataList[0], GL_DYNAMIC_DRAW);
    // Debinding d'un VBO sur la cible GL_ARRAY_BUFFER:
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void whereAmI(const FreeflyCamera &freeflyCamera, int &chunkNumberX,
              int &chunkNumberZ) {
    chunkNumberX = (int) freeflyCamera.getPosition().x / 16;
    chunkNumberZ = (int) freeflyCamera.getPosition().z / 16;
}

void whereAmI(const ConstrainedCamera &camera, int &chunkNumberX,
              int &chunkNumberZ) {
    chunkNumberX = camera.getPosition().x / 16;
    chunkNumberZ = camera.getPosition().z / 16;
}

void bindPnjVBOToVAO(GLuint vboPnj, GLuint vaoPnj, const GLuint VERTEX_ATTR_POSITION, const GLuint VERTEX_ATTR_NORMAL,
                     const GLuint VERTEX_ATTR_TEXTURE) {
    glBindVertexArray(vaoPnj);
    glBindBuffer(GL_ARRAY_BUFFER, vboPnj);

    // Specification des attributs de position :
    glVertexAttribPointer(VERTEX_ATTR_POSITION, 3, GL_FLOAT, GL_FALSE,
                          sizeof(ShapeVertex),
                          (const GLvoid *) offsetof(ShapeVertex, position));
    glVertexAttribPointer(VERTEX_ATTR_NORMAL, 3, GL_FLOAT, GL_FALSE,
                          sizeof(ShapeVertex),
                          (const GLvoid *) offsetof(ShapeVertex, normal));
    glVertexAttribPointer(VERTEX_ATTR_TEXTURE, 2, GL_FLOAT, GL_FALSE,
                          sizeof(ShapeVertex),
                          (const GLvoid *) offsetof(ShapeVertex, texCoords));

    // Debinding d'un VBO sur la cible GL_ARRAY_BUFFER:
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Debinding de l'unique VAO:
    glBindVertexArray(0);
}

void bindMapVBOToVAO(GLuint vboMap, GLuint vaoMap, const GLuint VERTEX_ATTR_POSITION, const GLuint VERTEX_ATTR_NORMAL,
                     const GLuint VERTEX_ATTR_TEXTURE) {
    glBindVertexArray(vaoMap);

    // Re-binding du VBO sur la cible GL_ARRAY_BUFFER pour glVertexAttribPointer:
    glBindBuffer(GL_ARRAY_BUFFER, vboMap);

    // Specification des attributs de position :
    glVertexAttribPointer(VERTEX_ATTR_POSITION, 3, GL_FLOAT, GL_FALSE,
                          sizeof(ShapeVertex),
                          (const GLvoid *) offsetof(ShapeVertex, position));
    glVertexAttribPointer(VERTEX_ATTR_NORMAL, 3, GL_FLOAT, GL_FALSE,
                          sizeof(ShapeVertex),
                          (const GLvoid *) offsetof(ShapeVertex, normal));
    glVertexAttribPointer(VERTEX_ATTR_TEXTURE, 2, GL_FLOAT, GL_FALSE,
                          sizeof(ShapeVertex),
                          (const GLvoid *) offsetof(ShapeVertex, texCoords));
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
}

void configureVAOs(GLuint vaoMap, GLuint vaoPnj, GLuint &VERTEX_ATTR_POSITION,  GLuint &VERTEX_ATTR_NORMAL,
                   GLuint &VERTEX_ATTR_TEXTURE) {
    VERTEX_ATTR_POSITION= 0;
    VERTEX_ATTR_NORMAL= 1;
    VERTEX_ATTR_TEXTURE= 2;
    glBindVertexArray(vaoMap);
    glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
    glEnableVertexAttribArray(VERTEX_ATTR_NORMAL);
    glEnableVertexAttribArray(VERTEX_ATTR_TEXTURE);

    glBindVertexArray(0);

    glBindVertexArray(vaoPnj);

    glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
    glEnableVertexAttribArray(VERTEX_ATTR_NORMAL);
    glEnableVertexAttribArray(VERTEX_ATTR_TEXTURE);

    glBindVertexArray(0);
}

void loadPnjInVBO(const Cube &cube, GLuint vboPnj) {
    glBindBuffer(GL_ARRAY_BUFFER, vboPnj);
    glBufferData(GL_ARRAY_BUFFER, cube.getVertexCount() * sizeof(ShapeVertex),
                 cube.getDataPointer(), GL_STATIC_DRAW);
    // Debinding d'un VBO sur la cible GL_ARRAY_BUFFER:
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}


void loadPnjInVBO(const Sphere &sphere, GLuint vboPnj) {
    glBindBuffer(GL_ARRAY_BUFFER, vboPnj);
    glBufferData(GL_ARRAY_BUFFER, sphere.getVertexCount() * sizeof(ShapeVertex),
                 sphere.getDataPointer(), GL_STATIC_DRAW);
    // Debinding d'un VBO sur la cible GL_ARRAY_BUFFER:
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void loadPnjTextureLocation(std::unique_ptr<Image> &pnjImagePtr, GLuint pnjTextureLocation) {
    glBindTexture(GL_TEXTURE_2D, pnjTextureLocation);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, pnjImagePtr->getWidth(),
                 pnjImagePtr->getHeight(), 0, GL_RGBA, GL_FLOAT,
                 pnjImagePtr->getPixels());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void loadAtlasTextureLocation(std::unique_ptr<Image> &atlasImagePtr, GLuint atlasTextureLocation) {
    glBindTexture(GL_TEXTURE_2D, atlasTextureLocation);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, atlasImagePtr->getWidth(),
                 atlasImagePtr->getHeight(), 0, GL_RGBA, GL_FLOAT,
                 atlasImagePtr->getPixels());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void loadSkyboxTextureLocation(std::unique_ptr<Image> &skyboxImagePtr, GLuint skyboxTextureLocation) {
    glBindTexture(GL_TEXTURE_2D, skyboxTextureLocation);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, skyboxImagePtr->getWidth(), skyboxImagePtr->getHeight(), 0, GL_RGBA, GL_FLOAT,
                 skyboxImagePtr->getPixels());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
}


int main(int argc, char **argv) {
    GLFWWindowManager windowManager =
            GLFWWindowManager(1920, 1920, "LaraCraft", windowModes::Windowed);
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


    GlobalProgram globalProgram = GlobalProgram(applicationPath);

    SimpleTexturedSkyboxProgram simpleTexturedSkyboxProgram = SimpleTexturedSkyboxProgram(applicationPath);


    /***************
     * TEXTURE INIT
     ***************/

    std::unique_ptr<Image> atlasImagePtr =
            loadImage("TP_Mastercraft/assets/textures/blocks/atlas.png");
    assert(atlasImagePtr != nullptr);

    std::unique_ptr<Image> pnjImagePtr =
            loadImage("TP_Mastercraft/assets/textures/blocks/baseball.jpeg");
    assert(pnjImagePtr != nullptr);

    std::unique_ptr<Image> snowImagePtr =
            loadImage("TP_Mastercraft/assets/textures/blocks/snow.png");
    assert(pnjImagePtr != nullptr);

    std::unique_ptr<Image> stoneImagePtr =
            loadImage("TP_Mastercraft/assets/textures/blocks/stone.png");
    assert(pnjImagePtr != nullptr);

    std::unique_ptr<HeightMap> heightMapPtr = loadHeightMap(
            "TP_Mastercraft/assets/terrain/64_64_perlin/map.png", 1.0f, 1.0f, 0.2f);
    assert(heightMapPtr != nullptr);

    std::cout << heightMapPtr->getWidth() << std::endl;
    auto ptr = heightMapPtr->getHeightData();

    std::unique_ptr<ColorMap> colorMapPtr = loadColorMap(
            "TP_Mastercraft/assets/terrain/64_64_perlin/color.png", 1.0f, 1.0f, 1.0f);
    assert(colorMapPtr != nullptr);
    auto ptrColor = colorMapPtr->getColorData();

    float width = heightMapPtr->getWidth();
    float height = heightMapPtr->getHeight();

    /*
    for (int i = 0; i < colorMapPtr->getWidth(); ++i) {
        for (int j = 0; j < colorMapPtr->getHeight(); ++j) {
            if (colorMapPtr->getColorData()[i][j] == glm::vec3(255, 255, 0)) {
                std::string s = "S";
                std::cout << s << ",";
                continue;
            }
            if (colorMapPtr->getColorData()[i][j] == glm::vec3(128, 128, 128)) {
                std::string s = "R";
                std::cout << s << ",";
                continue;
            }
            if (colorMapPtr->getColorData()[i][j] == glm::vec3(255, 0, 0)) {
                std::string s = "M";
                std::cout << s << ",";
                continue;
            }
            if (colorMapPtr->getColorData()[i][j] == glm::vec3(255, 255, 255)) {
                std::string s = "N";
                std::cout << s << ",";
                continue;
            }
            std::string s = "G";
            std::cout << s << ",";
        }
        std::cout << ";" << std::endl;

    }*/

    GLuint atlasTextureLocation, pnjTextureLocation, skyboxDayTextureLocation, skyboxNightTextureLocation;

    glGenTextures(1, &atlasTextureLocation);

    glGenTextures(1, &pnjTextureLocation);

    glGenTextures(1, &skyboxDayTextureLocation);

    glGenTextures(1, &skyboxNightTextureLocation);


    /**
     * Atlas texture
     */
    loadAtlasTextureLocation(atlasImagePtr, atlasTextureLocation);

    /**
     * PNJ texture
     */
    loadPnjTextureLocation(pnjImagePtr, pnjTextureLocation);


    /**
     * Skybox day texture
     */
    loadSkyboxTextureLocation(snowImagePtr, skyboxDayTextureLocation);

    /**
     * Skybox night texture
     */
    loadSkyboxTextureLocation(stoneImagePtr, skyboxNightTextureLocation);


    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    /*********************************
     * HERE SHOULD COME THE INITIALIZATION CODE
     *********************************/

    Cube cube;
    Sphere sphere(1, 32 , 16);


    /**
     * MAP INFOS
     */
    long vboMapVertexCount = 0;
    std::vector<ShapeVertex> concatDataList;
    int distanceChunkLoaded = 5;



    GLuint vboMap, vboPnj;
    GLuint vaoMap, vaoPnj;


    /**
     * GENERATION OF THE VBOs
     */
    glGenBuffers(1, &vboMap);
    glGenBuffers(1, &vboPnj);

    std::vector<std::vector<Chunk>> chunkList;

    auto offsetTextureMap = OffsetTextureMap(*colorMapPtr);

    generateAllChunks(*heightMapPtr, offsetTextureMap, chunkList);
    refreshChunkVBO(chunkList, concatDataList, vboMapVertexCount, vboMap, 0, 0,
                    distanceChunkLoaded);

    Pnj pnj(vec3(0,heightMapPtr->getHeightData()[0][0],0), heightMapPtr->getHeightData());

    loadPnjInVBO(sphere, vboPnj);

    glGenVertexArrays(1, &vaoMap);
    glGenVertexArrays(1, &vaoPnj);

    /**
     * BINDING OF VAOs
     */
    GLuint VERTEX_ATTR_POSITION;
    GLuint VERTEX_ATTR_NORMAL;
    GLuint VERTEX_ATTR_TEXTURE;
    configureVAOs(vaoMap, vaoPnj, VERTEX_ATTR_POSITION, VERTEX_ATTR_NORMAL, VERTEX_ATTR_TEXTURE);


    /**
     * BINDING OF THE VBOMAP ON THE VAOMAP
     */
    bindMapVBOToVAO(vboMap, vaoMap, VERTEX_ATTR_POSITION, VERTEX_ATTR_NORMAL, VERTEX_ATTR_TEXTURE);

    /**
     * BINDING OF THE VBOPNJ ON THE PNJ
     */

    bindPnjVBOToVAO(vboPnj, vaoPnj, VERTEX_ATTR_POSITION, VERTEX_ATTR_NORMAL, VERTEX_ATTR_TEXTURE);

    AntiCube skybox;

    GLuint skyVbo, skyVao;
    glGenBuffers(1, &skyVbo);

    glBindBuffer(GL_ARRAY_BUFFER, skyVbo);

    glBufferData(GL_ARRAY_BUFFER, skybox.getVertexCount() * sizeof(ShapeVertex), skybox.getDataPointer(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);


    glGenVertexArrays(1, &skyVao);

    // Binding de l'unique VAO:
    glBindVertexArray(skyVao);

    glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
    glEnableVertexAttribArray(VERTEX_ATTR_NORMAL);
    glEnableVertexAttribArray(VERTEX_ATTR_TEXTURE);

    // Re-binding du VBO sur la cible GL_ARRAY_BUFFER pour glVertexAttribPointer:
    glBindBuffer(GL_ARRAY_BUFFER, skyVbo);

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

    glm::mat4 mapModelMatrix = mat4();

    projMatrix = glm::perspective(glm::radians(70.f), 1.f, 0.1f, 100.f);

    //FreeflyCamera camera;
    // FreeflyCamera camera = FreeflyCamera(glm::vec3(0.f,  0.f, 5.f)); //(float)ptr[0][0]
    ConstrainedCamera camera =
            ConstrainedCamera(0.f, 0.f, heightMapPtr->getHeightData(), 2.f);

    // Application loop:

    int oldChunkPositionX = 0;
    int oldChunkPositionZ = 0;
    int currentChunkPositionX = 0;
    int currentChunkPositionZ = 0;

    glfwSetTime(0);

    bool isDay = true;

    while (!windowManager.windowShouldClose()) {

        whereAmI(camera, currentChunkPositionX, currentChunkPositionZ);
        if (currentChunkPositionX < oldChunkPositionX - distanceChunkLoaded / 2 ||
            currentChunkPositionX > oldChunkPositionX + distanceChunkLoaded / 2 ||
            currentChunkPositionZ < oldChunkPositionZ - distanceChunkLoaded / 2 ||
            currentChunkPositionZ > oldChunkPositionZ + distanceChunkLoaded / 2) {
            std::cout << "Need reload"
                      << "(" << currentChunkPositionX << ";" << currentChunkPositionZ
                      << ")" << std::endl;
            refreshChunkVBO(chunkList, concatDataList, vboMapVertexCount, vboMap,
                            currentChunkPositionX, currentChunkPositionZ,
                            distanceChunkLoaded);
            oldChunkPositionX = currentChunkPositionX;
            oldChunkPositionZ = currentChunkPositionZ;
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glDepthMask(GL_FALSE);

        float angle = (float) glfwGetTime() * 0.1f;

        if(angle > 6.28319) {
            glfwSetTime(0);
            angle = (float) glfwGetTime() * 0.1f;
            isDay = true;
        }

        if(angle > 3.14159) {
            isDay = false;
        }

        simpleTexturedSkyboxProgram.m_Program.use();

        glBindVertexArray(skyVao);

        glActiveTexture(GL_TEXTURE0);

        if(isDay) {
            glBindTexture(GL_TEXTURE_2D, skyboxDayTextureLocation);
        }
        else {
            glBindTexture(GL_TEXTURE_2D, skyboxNightTextureLocation);
        }

        glUniform1i(simpleTexturedSkyboxProgram.uTextureId, 0);


        mat4 modelMatrix = glm::translate(mat4(), camera.getPosition());
        drawSkybox(simpleTexturedSkyboxProgram, skybox.getVertexCount(), projMatrix, viewMatrix, scale(modelMatrix, glm::vec3(30.0,30.0,30.0)));

        //Flush texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, 0);

        glBindVertexArray(0);

        glDepthMask(GL_TRUE);

        globalProgram.m_Program
                     .use();

        glBindVertexArray(vaoMap);

        viewMatrix = camera.getViewMatrix();

        std::cout << angle << std::endl;
        // Light object
        DirectionalLight light = DirectionalLight(glm::rotate(
                glm::translate(glm::mat4(), vec3(camera.getPosition().x, 256.f,
                                                 camera.getPosition().z)),
                angle, glm::vec3(0., 0., 1.)));

        /**
         * DRAW MAP
         */
        glBindVertexArray(vaoMap);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, atlasTextureLocation);
        glUniform1i(globalProgram.uTextureId, 0);

        drawVAO(globalProgram, vboMapVertexCount, projMatrix, viewMatrix, mapModelMatrix, light);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, 0);

        glBindVertexArray(0);

        /**
        * DRAW PNJ
        */
        glBindVertexArray(vaoPnj);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, pnjTextureLocation);
        glUniform1i(globalProgram.uTextureId, 0);

        drawVAO(globalProgram, sphere.getVertexCount(), projMatrix, viewMatrix, pnj.getModelMatrix(),
                light);
        pnj.updatePosition();



        // Flush texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, 0);

        glBindVertexArray(0);

        windowManager.swapBuffers();
        //windowManager.handleEventsForFPSview(camera);
        windowManager.handleEventsForFPSConstrainedView(camera);
    }
    glDeleteTextures(1, &atlasTextureLocation);
    glDeleteTextures(1, &pnjTextureLocation);
    glDeleteBuffers(1, &vboMap);
    glDeleteBuffers(1, &vboPnj);
    glDeleteVertexArrays(1, &vaoMap);
    glDeleteVertexArrays(1, &vaoPnj);

    glDeleteTextures(1, &skyboxDayTextureLocation);
    glDeleteBuffers(1, &skyVbo);
    glDeleteVertexArrays(1, &skyVao);

    return EXIT_SUCCESS;
}
