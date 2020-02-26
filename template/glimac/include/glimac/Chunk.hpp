//
// Created by adam on 01/01/2020.
//

#ifndef IMACGL_CHUNK_HPP
#define IMACGL_CHUNK_HPP

#include "TextureOffsetEnum.hpp"
#include "Cube.hpp"
#include <vector>
#include "glm.hpp"
#include "GlobalProgram.hpp"
#include "DirectionalLight.hpp"
#include "HeightMap.hpp"
#include "ColorMap.hpp"
#include "OffsetTextureMap.hpp"


/**
 * Represents a 16*256*16 bundle of cubes.
 * In first approximation, a chunk section is made of only one texture.
 * A coordinate (x,y,z) coordinate in the matrix can be either empty (no cube) or filled with a cube of the material.
 */
class Chunk {
public:

    /**
     * Constructor for the Chunk
     * @param position Coordinates x and z on the map
     * @param heightMap The HeightMap used
     * @param textureMap  OffsetTextureMap used
     */
    Chunk(const glm::vec2 &position, const HeightMap &heightMap, const OffsetTextureMap &textureMap)
            : positionTranslation(
            vec3(position.x, 0, position.y)) {

        int offsetRow = position.x;
        int offsetColumn = position.y;

        assert(heightMap.getWidth() % 16 == 0);
        assert(heightMap.getHeight() % 16 == 0);
        assert(offsetRow % 16 == 0);
        assert(offsetColumn % 16 == 0);

        for (int row = offsetRow; row < WIDTH + offsetRow; row++) {
            std::vector<float> tmpHeightMapColumn;
            std::vector<float> tmpTextureMapColumn;
            for (int column = offsetColumn; column < LENGTH + offsetColumn; column++) {
                tmpHeightMapColumn.push_back(heightMap.getHeightData()
                                                     .at(row)
                                                     .at(column));

                tmpTextureMapColumn.push_back(textureMap.getTextureData()
                                                      .at(row)
                                                      .at(column));
            }
            localHeightMap.push_back(tmpHeightMapColumn);
            localOffsetTextureMap.push_back(tmpTextureMapColumn);
        }

        build();
    }


    /**
    * Getter on the pointer data on vertices.
    * @return
    */
    const ShapeVertex *getDataPointer() const;

    /**
    * Getter on the vector on vertices.
    * @return
    */
    std::vector<glimac::ShapeVertex> getDataVector() const;

    /**
     * Getter on the number of vertices in the cube
     * @return the number of vertices in the cube
     */
    GLsizei getVertexCount() const;

    /**
     * 3D array that tells if there is a cube at this position
     * Beware that the order of the coordinate is not usual (x -> z -> height (y))
     */
    std::vector<std::vector<std::vector<bool>>> isFilled3DVector;

private:
    /**
     * Number of cube on the x axis
     */
    static constexpr int WIDTH = 16;
    /**
     * Number of cube on the z axis
     */
    static constexpr int LENGTH = 16;

    /**
     * Number of cube on the y axis
     */
    static constexpr int HEIGHT = 256;

    /**
     * Function building a chunk (call other functions inside)
     */
    void build();

    /**
     * Data of the HeightMap used
     */
    std::vector<std::vector<float>> localHeightMap;

    /**
     * Data of the OffsetTextureMap used
     */
    std::vector<std::vector<float>> localOffsetTextureMap;

    /**
     * Vector of ShapeVertex
     */
    std::vector<glimac::ShapeVertex> m_Vertices;

    /**
     * A cube
     */
    static const Cube cube;

    /**
     * Vector used to translate the cube when building the Chunk
     */
    glm::vec3 positionTranslation;

    /**
     * Function testing if a cube is surrounded by neighbors
     * @param x Position of the cube on the x axis
     * @param z Position of the cube on the z axis
     * @param height Position of the cube on the y axis
     * @return true if the cube is surrounded by neighbors, false otherwise
     */
    bool isSurroundedByNeighbors(int x, int z, int height);

    /**
     * Initialise the IsFilled3DVector
     */
    void initializeIsFilled3DVector();

    /**
     * Compute the IsFilled3DVector
     */
    void computeIsFilled3DVector();

    /**
     * Search the neighbors
     */
    void searchForNeighborhood();

    /**
     * Add the vertex of the cube to be displayed
     */
    void addVertexOfDisplayedCube();

};


#endif //IMACGL_CHUNK_HPP
