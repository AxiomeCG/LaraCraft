//
// Created by adam on 01/01/2020.
//

#ifndef IMACGL_CHUNK_HPP
#define IMACGL_CHUNK_HPP


#include <vector>
#include "glm.hpp"
#include "SimpleTexturedCubeProgram.hpp"
#include "DirectionalLight.hpp"
#include "HeightMap.hpp"

/**
 * Represents a 16*resv*16 bundle of cubes.
 * In first approximation, a chunk section is made of only one texture.
 * A coordinate (x,y,z) coordinate in the matrix can be either empty (no cube) or filled with a cube of the material.
 */
class Chunk {
public:
    Chunk(const glm::vec2 &position, HeightMap heightMap) : positionTranslation(
            vec3(position.x, 0, position.y)) {

        int offsetRow = position.x;
        int offsetColumn = position.y;

        assert(heightMap.getWidth() % 16 == 0);
        assert(heightMap.getHeight() % 16 == 0);
        assert(offsetRow % 16 == 0);
        assert(offsetColumn % 16 == 0);

        for (int row = offsetRow; row < WIDTH + offsetRow; row++) {
            std::vector<float> tmpColumn;
            for (int column = offsetColumn; column < LENGTH + offsetColumn; column++) {
                tmpColumn.push_back(heightMap.getHeightData()
                                             .at(row)
                                             .at(column));
            }
            localHeightMap.push_back(tmpColumn);
        }

        build();

        /*
        for (auto & localRow : localHeightMap){
            for (auto & value: localRow) {
                std::cout << value << ", ";
            }
            std::cout << std::endl;
        }*/
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
    static const int WIDTH = 16;
    /**
     * Number of cube on the z axis
     */
    static const int LENGTH = 16;

    /**
     * Number of cube on the y axis
     */
    static const int HEIGHT = 256;

    void build();

    std::vector<std::vector<float>> localHeightMap;

    std::vector<glimac::ShapeVertex> m_Vertices;

    const Cube cube;

    glm::vec3 positionTranslation;

    bool isSurroundedByNeighbors(int x, int z, int height);

    void initializeIsFilled3DVector();

    void computeIsFilled3DVector();

    void searchForNeighborhood();

    void addVertexOfDisplayedCube();
};


#endif //IMACGL_CHUNK_HPP
