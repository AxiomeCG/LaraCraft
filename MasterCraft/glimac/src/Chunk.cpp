#include <glimac/TextureOffsetEnum.hpp>
#include "glimac/Chunk.hpp"

void Chunk::build() {
    initializeIsFilled3DVector();
    computeIsFilled3DVector();
    searchForNeighborhood();
    addVertexOfDisplayedCube();
}

void Chunk::addVertexOfDisplayedCube() {
    for (int x = 0; x < WIDTH; ++x) {
        for (int z = 0; z < LENGTH; ++z) {
            for (int height = 0; height < (int) localHeightMap.at(x)
                    .at(z); height++) {

                if (!isFilled3DVector[x][z][height]) {
                    continue;
                }

                for (int v = 0; v < cube.getVertexCount(); ++v) {
                    ShapeVertex shapeVertex;
                    const ShapeVertex &currentCubeVertex = cube.getDataPointer()[v];
                    double vertexX = currentCubeVertex.position
                                             .x /
                                     2.; // The cube is defined from -1 to 1 so it's size 2. Divide by two to get a 1x1x1 size cube
                    double vertexY = currentCubeVertex.position
                                             .y / 2.;
                    double vertexZ = currentCubeVertex.position
                                             .z / 2.;

                    shapeVertex.position = vec3(vertexX + (float) x + positionTranslation.x,
                                                vertexY + floor((float) height) + positionTranslation.y,
                                                vertexZ + (float) z + positionTranslation.z);

                    shapeVertex.normal = vec3(currentCubeVertex.normal);

                    shapeVertex.texCoords = vec2(
                            localOffsetTextureMap.at(x)
                                    .at(z) + currentCubeVertex.texCoords
                                                     .x / TextureOffsetEnum::NUMBER_TEXTURE,
                            currentCubeVertex.texCoords
                                    .y);// + offset
                    m_Vertices.push_back(shapeVertex);
                }
            }

        }
    }


}

void Chunk::searchForNeighborhood() {
    std::vector < std::vector < std::vector < bool >> > clone(isFilled3DVector);
    for (int x = 0; x < WIDTH; ++x) {
        for (int z = 0; z < LENGTH; ++z) {
            for (int height = 0; height < (int) localHeightMap.at(x)
                    .at(z); height++) {
                if (isSurroundedByNeighbors(x, z, height)) clone[x][z][height] = false;
            }
        }
    }

    isFilled3DVector = clone;
}

void Chunk::computeIsFilled3DVector() {
    for (int x = 0; x < WIDTH; ++x) {
        for (int z = 0; z < LENGTH; ++z) {
            for (int height = 0; height < (int) localHeightMap.at(x)
                    .at(z); height++) {
                isFilled3DVector[x][z][height] = true;
            }
        }
    }
}

void Chunk::initializeIsFilled3DVector() {
    for (int x = 0; x < WIDTH; x++) {
        std::vector <std::vector<bool>> isFilled2DVector;
        for (int z = 0; z < LENGTH; ++z) {
            std::vector<bool> isFilled1DVector(HEIGHT, false);
            isFilled2DVector.push_back(isFilled1DVector);
        }
        isFilled3DVector.push_back(isFilled2DVector);
    }
}

GLsizei Chunk::getVertexCount() const {
    return m_Vertices.size();
}

std::vector <glimac::ShapeVertex> Chunk::getDataVector() const {
    return m_Vertices;
}

const ShapeVertex *Chunk::getDataPointer() const {
    return &m_Vertices[0];
}

bool Chunk::isSurroundedByNeighbors(int x, int z, int height) {
    bool isAboveFilled = height + 1 < HEIGHT && isFilled3DVector[x][z][height + 1];
    bool isBelowFilled = height - 1 >= 0 && isFilled3DVector[x][z][height - 1];
    bool isLeftFilled = x + 1 < WIDTH && isFilled3DVector[x + 1][z][height];
    bool isRightFilled = x - 1 >= 0 && isFilled3DVector[x - 1][z][height];
    bool isFrontFilled = z + 1 < LENGTH && isFilled3DVector[x][z + 1][height];
    bool isBackFilled = z - 1 >= 0 && isFilled3DVector[x][z - 1][height];

    return isAboveFilled && isBelowFilled && isLeftFilled && isRightFilled && isFrontFilled && isBackFilled;
}

const Cube Chunk::cube;