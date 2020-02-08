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
    Chunk(const glm::vec2 &position, const HeightMap &heightMap, const OffsetTextureMap &textureMap) : positionTranslation(
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

       /* for (auto c : localOffsetTextureMap) {
            for (auto i : c) {
                std::cout << i << ", ";
            }
            std::cout << ";" << std::endl;
        }*/

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
    static constexpr int WIDTH = 16;
    /**
     * Number of cube on the z axis
     */
    static constexpr int LENGTH = 16;

    /**
     * Number of cube on the y axis
     */
    static constexpr int HEIGHT = 256;

    void build();

    std::vector<std::vector<float>> localHeightMap;
    std::vector<std::vector<float>> localOffsetTextureMap;

    std::vector<glimac::ShapeVertex> m_Vertices;

    static const Cube cube;

    glm::vec3 positionTranslation;

    bool isSurroundedByNeighbors(int x, int z, int height);

    void initializeIsFilled3DVector();

    void computeIsFilled3DVector();

    void searchForNeighborhood();

    void addVertexOfDisplayedCube();

};




#endif //IMACGL_CHUNK_HPP
