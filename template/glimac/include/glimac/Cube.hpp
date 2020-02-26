//
// Created by adam on 30/12/2019.
//

#ifndef IMACGL_CUBE_HPP
#define IMACGL_CUBE_HPP

#include "MultiTexturedCubeProgram.hpp"
#include <vector>
#include <glimac/common.hpp>

using namespace glimac;
using namespace glm;

/**
 * Represents an elementary cube forged from triangles.
 *
 */
class Cube {

public:
    /**
     * Constructor of the cube
     */
    Cube() : m_nVertexCount(0) {
        build();
    }

    /**
     * Getter on the pointer data on vertices.
     * @return
     */
    const ShapeVertex *getDataPointer() const;

    /**
     * Getter on the number of vertices in the cube
     * @return the number of vertices in the cube
     */
    GLsizei getVertexCount() const;

private:
    /**
     * A vector of ShapeVertex
     */
    std::vector<glimac::ShapeVertex> m_Vertices;

    /**
     * The number of vertex
     */
    GLsizei m_nVertexCount;

    /**
     * Function building the cube (the face are created clockwise)
     */
    void build();

    /**
     * Generate the front face of the cube
     */
    void generateFrontFace();

    /**
     * Generate the left face of the cube
     */
    void generateLeftFace();

    /**
     * Generate the back face of the cube
     */
    void generateBackFace();

    /**
     * Generate the right face of the cube
     */
    void generateRightFace();

    /**
     * Generate the top face of the cube
     */
    void generateTopFace();

    /**
     * Generate the bottom face of the cube
     */
    void generateBottomFace();
};


#endif //IMACGL_CUBE_HPP
