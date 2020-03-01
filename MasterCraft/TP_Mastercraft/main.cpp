
#include <glimac/Chunk.hpp>
#include <glimac/ColorMap.hpp>
#include <glimac/ConstrainedCamera.hpp>
#include <glimac/Cube.hpp>

#include <glimac/AntiCube.hpp>
#include <glimac/GLFWWindowManager.hpp>

#include <glimac/SimpleTexturedSkyboxProgram.hpp>

#include <glimac/DirectionalLight.hpp>
#include <glimac/FilePath.hpp>
#include <glimac/GLFWWindowManager.hpp>
#include <glimac/GlobalProgram.hpp>
#include <glimac/HeightMap.hpp>
#include <glimac/Image.hpp>
#include <glimac/common.hpp>
#include <glimac/VegetationMap.hpp>
#include <GL/glew.h>

#include <cstddef>
#include <iostream>
#include <glimac/Pnj.hpp>
#include <glimac/Sphere.hpp>
#include <glimac/Tree.hpp>


using namespace glimac;


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
    mat4 modelViewMatrix = viewMatrix * modelMatrix;
    mat4 modelViewProjectionMatrix = projMatrix * modelViewMatrix;
    mat4 normalMatrix = transpose(glm::inverse(modelViewMatrix));
    glUniformMatrix4fv(program.uMVPMatrixId, 1, GL_FALSE, value_ptr(modelViewProjectionMatrix));
    glUniformMatrix4fv(program.uMVMatrixId, 1, GL_FALSE, value_ptr(modelViewMatrix));
    glUniformMatrix4fv(program.uNormalMatrixId, 1, GL_FALSE, value_ptr(normalMatrix));

    glDrawArrays(GL_TRIANGLES, 0, vertexCount);
}

void generateAllChunks(const HeightMap &heightMap, const OffsetTextureMap &offsetTextureMap,

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

void generateTreeList(const HeightMap &heightMap, const VegetationMap &vegetationMap, std::vector<std::vector<Tree>> &treeList) {
    auto dataList = vegetationMap.getVegetationData();

    for (int x = 0; x < vegetationMap.getWidth(); ++x) {
        std::vector<Tree> columnList;
        for (int z = 0; z < vegetationMap.getHeight(); ++z) {
            if(dataList.at(x).at(z) == vec3(255, 255, 255)) {
                columnList.emplace_back(vec3(x,floor(heightMap.getHeightData().at(x).at(z)),z));
            }
        }
        treeList.push_back(columnList);
    }
}

void generateSurroundingChunkVertexFromAllChunks(
        std::vector<std::vector<Chunk>> chunkList, long &globalCount,
        std::vector<ShapeVertex> &concatDataList, int offsetX1, int offsetX2,
        int offsetZ1, int offsetZ2) {
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

void whereAmI(const ConstrainedCamera &camera, int &chunkNumberX,
              int &chunkNumberZ) {
    chunkNumberX = camera.getPosition().x / 16;
    chunkNumberZ = camera.getPosition().z / 16;
}


void bindVBOToVAO(GLuint vbo, GLuint vao, const GLuint VERTEX_ATTR_POSITION, const GLuint VERTEX_ATTR_NORMAL,
                  const GLuint VERTEX_ATTR_TEXTURE) {
    glBindVertexArray(vao);

    // Re-binding du VBO sur la cible GL_ARRAY_BUFFER pour glVertexAttribPointer:
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

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

void configureVAO(GLuint vao, GLuint &VERTEX_ATTR_POSITION, GLuint &VERTEX_ATTR_NORMAL,
                  GLuint &VERTEX_ATTR_TEXTURE) {

    glBindVertexArray(vao);
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
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, skyboxImagePtr->getWidth(), skyboxImagePtr->getHeight(), 0, GL_RGBA,
                 GL_FLOAT,
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

    std::unique_ptr<Image> dayImagePtr =
            loadImage("TP_Mastercraft/assets/textures/blocks/day.jpg");
    assert(pnjImagePtr != nullptr);

    std::unique_ptr<Image> nightImagePtr =
            loadImage("TP_Mastercraft/assets/textures/blocks/night.png");
    assert(pnjImagePtr != nullptr);




    std::unique_ptr<HeightMap> heightMapPtr = loadHeightMap(
            "TP_Mastercraft/assets/terrain/16_16_perlin/map.png", 1.0f, 1.0f, 0.2f);
    assert(heightMapPtr != nullptr);
    auto ptr = heightMapPtr->getHeightData();

    std::unique_ptr<ColorMap> colorMapPtr = loadColorMap(
            "TP_Mastercraft/assets/terrain/16_16_perlin/color.png", 1.0f, 1.0f, 1.0f);
    assert(colorMapPtr != nullptr);

    std::unique_ptr<VegetationMap> vegetationMapPtr = loadVegetationMap(
            "TP_Mastercraft/assets/terrain/16_16_perlin/vege.png", 1.0f, 1.0f, 1.0f);
    assert(vegetationMapPtr != nullptr);
    auto ptrColor = colorMapPtr->getColorData();

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
    loadSkyboxTextureLocation(dayImagePtr, skyboxDayTextureLocation);

    /**
     * Skybox night texture
     */
    loadSkyboxTextureLocation(nightImagePtr, skyboxNightTextureLocation);


    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    /*********************************
     * HERE SHOULD COME THE INITIALIZATION CODE
     *********************************/

    Cube cube;
    Sphere sphere(1, 32, 16);


    /**
     * MAP INFOS
     */
    long vboMapVertexCount = 0;
    long vboTreeVertexCount = 0;
    std::vector<ShapeVertex> concatDataList;
    int distanceChunkLoaded = 5;


    GLuint mapVbo, mapVao;
    GLuint pnjVbo, pnjVao;
    GLuint skyVbo, skyVao;
    GLuint treeVbo, treeVao;


    /**
     * GENERATION OF THE VBOs
     */
    glGenBuffers(1, &mapVbo);
    glGenBuffers(1, &pnjVbo);
    glGenBuffers(1, &skyVbo);
    glGenBuffers(1, &treeVbo);



    /**
     * LOADING VBOs
     */

    /**
     * MAP
     */
    auto offsetTextureMap = OffsetTextureMap(*colorMapPtr);
    std::vector<std::vector<Chunk>> chunkList;
    generateAllChunks(*heightMapPtr, offsetTextureMap, chunkList);
    refreshChunkVBO(chunkList, concatDataList, vboMapVertexCount, mapVbo, 0, 0,
                    distanceChunkLoaded);

    /**
     * PNJ
     */
    Pnj pnj(vec3(0, heightMapPtr->getHeightData()[0][0], 0), heightMapPtr->getHeightData(), vegetationMapPtr->getVegetationData());
    loadPnjInVBO(sphere, pnjVbo);

    /**
     * SKYBOX
     */

    AntiCube antiCube;

    glBindBuffer(GL_ARRAY_BUFFER, skyVbo);

    glBufferData(GL_ARRAY_BUFFER, antiCube.getVertexCount() * sizeof(ShapeVertex), antiCube.getDataPointer(),
                 GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    /**
     * TREEs
     */

    std::vector<std::vector<Tree>> treeList;
    generateTreeList(*heightMapPtr, *vegetationMapPtr,treeList);


    std::vector<ShapeVertex> shapeVertexList;
    for (const auto& column : treeList){
        for (auto tree: column){
            vboTreeVertexCount += tree.getVertexCount();
            shapeVertexList.insert(shapeVertexList.end(), tree.getDataPointer(), tree.getDataPointer() + tree.getVertexCount());
        }
    }
    glBindBuffer(GL_ARRAY_BUFFER, treeVbo);

    glBufferData(GL_ARRAY_BUFFER, vboTreeVertexCount * sizeof(ShapeVertex), &shapeVertexList[0],
                 GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    /**
     * VAOs GENERATION
     */


    glGenVertexArrays(1, &mapVao);
    glGenVertexArrays(1, &pnjVao);
    glGenVertexArrays(1, &skyVao);
    glGenVertexArrays(1, &treeVao);

    /**
     * BINDING OF VAOs
     */
    GLuint VERTEX_ATTR_POSITION = 0;
    GLuint VERTEX_ATTR_NORMAL = 1;
    GLuint VERTEX_ATTR_TEXTURE = 2;


    configureVAO(mapVao, VERTEX_ATTR_POSITION, VERTEX_ATTR_NORMAL, VERTEX_ATTR_TEXTURE);
    configureVAO(pnjVao, VERTEX_ATTR_POSITION, VERTEX_ATTR_NORMAL, VERTEX_ATTR_TEXTURE);
    configureVAO(skyVao, VERTEX_ATTR_POSITION,VERTEX_ATTR_NORMAL,VERTEX_ATTR_TEXTURE);
    configureVAO(treeVao, VERTEX_ATTR_POSITION, VERTEX_ATTR_NORMAL,VERTEX_ATTR_TEXTURE);

    /**
     * BINDING OF THE VBO ON THE VAO
     */
    bindVBOToVAO(mapVbo, mapVao, VERTEX_ATTR_POSITION, VERTEX_ATTR_NORMAL, VERTEX_ATTR_TEXTURE);
    bindVBOToVAO(pnjVbo, pnjVao, VERTEX_ATTR_POSITION, VERTEX_ATTR_NORMAL, VERTEX_ATTR_TEXTURE);
    bindVBOToVAO(skyVbo, skyVao, VERTEX_ATTR_POSITION, VERTEX_ATTR_NORMAL, VERTEX_ATTR_TEXTURE);
    bindVBOToVAO(treeVbo, treeVao, VERTEX_ATTR_POSITION, VERTEX_ATTR_NORMAL, VERTEX_ATTR_TEXTURE);



    glm::mat4 projMatrix, viewMatrix;
    glm::mat4 mapModelMatrix = mat4();

    projMatrix = glm::perspective(glm::radians(70.f), 1.f, 0.1f, 100.f);

    ConstrainedCamera camera =
            ConstrainedCamera(0.f, 0.f, heightMapPtr->getHeightData(), vegetationMapPtr->getVegetationData(), 2.f);

    // Application loop:

    int oldChunkPositionX = 0;
    int oldChunkPositionZ = 0;
    int currentChunkPositionX = 0;
    int currentChunkPositionZ = 0;

    glfwSetTime(0);

    while (!windowManager.windowShouldClose()) {

        whereAmI(camera, currentChunkPositionX, currentChunkPositionZ);
        if (currentChunkPositionX < oldChunkPositionX - distanceChunkLoaded / 2 ||
            currentChunkPositionX > oldChunkPositionX + distanceChunkLoaded / 2 ||
            currentChunkPositionZ < oldChunkPositionZ - distanceChunkLoaded / 2 ||
            currentChunkPositionZ > oldChunkPositionZ + distanceChunkLoaded / 2) {
            refreshChunkVBO(chunkList, concatDataList, vboMapVertexCount, mapVbo,
                            currentChunkPositionX, currentChunkPositionZ,
                            distanceChunkLoaded);
            oldChunkPositionX = currentChunkPositionX;
            oldChunkPositionZ = currentChunkPositionZ;
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glDepthMask(GL_FALSE);

        float angle = (float) glfwGetTime() * 0.1f;

        if (angle > 2*M_PI) {
            glfwSetTime(0);
        }

        simpleTexturedSkyboxProgram.m_Program.use();

        glBindVertexArray(skyVao);

        glActiveTexture(GL_TEXTURE0);

        if (angle >= M_PI) {
            glBindTexture(GL_TEXTURE_2D, skyboxNightTextureLocation);
        }
        else {
            glBindTexture(GL_TEXTURE_2D, skyboxDayTextureLocation);
        }

        glUniform1i(simpleTexturedSkyboxProgram.uTextureId, 0);


        mat4 modelMatrix = glm::translate(mat4(), camera.getPosition());
        drawSkybox(simpleTexturedSkyboxProgram, antiCube.getVertexCount(), projMatrix, viewMatrix,
                   scale(modelMatrix, glm::vec3(30.0, 30.0, 30.0)));

        //Flush texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, 0);

        glBindVertexArray(0);

        glDepthMask(GL_TRUE);

        globalProgram.m_Program
                     .use();

        viewMatrix = camera.getViewMatrix();

        // Light object
        DirectionalLight light = DirectionalLight(glm::rotate(
                glm::translate(glm::mat4(), vec3(camera.getPosition().x, 256.f,
                                                 camera.getPosition().z)),
                angle, glm::vec3(0., 0., 1.)));

        /**
         * DRAW MAP
         */
        glBindVertexArray(mapVao);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, atlasTextureLocation);
        glUniform1i(globalProgram.uTextureId, 0);

        drawVAO(globalProgram, vboMapVertexCount, projMatrix, viewMatrix, mapModelMatrix, light);


        /**
        * DRAW TREE
        */
        glBindVertexArray(treeVao);
        glUniform1i(globalProgram.uTextureId, 0);

        drawVAO(globalProgram, vboTreeVertexCount, projMatrix, viewMatrix, mat4(),
                light);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, 0);

        glBindVertexArray(0);

        /**
        * DRAW PNJ
        */
        glBindVertexArray(pnjVao);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, pnjTextureLocation);
        glUniform1i(globalProgram.uTextureId, 0);

        drawVAO(globalProgram, sphere.getVertexCount(), projMatrix, viewMatrix, pnj.getModelMatrix(),
                light);
        pnj.updatePosition();

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, 0);

        glBindVertexArray(0);


        windowManager.swapBuffers();
        windowManager.handleEventsForFPSConstrainedView(camera, angle);
    }
    glDeleteTextures(1, &atlasTextureLocation);
    glDeleteTextures(1, &pnjTextureLocation);
    glDeleteTextures(1, &skyboxDayTextureLocation);

    glDeleteBuffers(1, &mapVbo);
    glDeleteBuffers(1, &pnjVbo);
    glDeleteBuffers(1, &skyVbo);

    glDeleteVertexArrays(1, &mapVao);
    glDeleteVertexArrays(1, &pnjVao);
    glDeleteVertexArrays(1, &skyVao);

    return EXIT_SUCCESS;
}


