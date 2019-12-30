//
// Created by adam on 30/12/2019.
//

#ifndef IMACGL_CUBE_HPP
#define IMACGL_CUBE_HPP


#include <vector>
#include <glimac/common.hpp>
#include <tiff.h>

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
     * Getter on the indices list of the vertices to build the shape via IBO
     * @return
     */
    const uint32_t *getIBOIndices() const;

    /**
     * Getter on the number of vertices in the cube
     * @return the number of vertices in the cube
     */
    GLsizei getVertexCount() const;

    GLsizei getIndiceCount() const;

private:
    std::vector<glimac::ShapeVertex> m_Vertices;
    GLsizei m_nVertexCount;

    void build();

    static uint32_t indices[];

    void generateFrontFace();

    void generateLeftFace();
};


#endif //IMACGL_CUBE_HPP
