//
// Created by Matthias Hudelot on 08/02/2020.
//

#ifndef TEMPLATE_ANTICUBE_HPP
#define TEMPLATE_ANTICUBE_HPP

#include <vector>
#include <glimac/common.hpp>

using namespace glimac;
using namespace glm;

/**
 * Represents an elementary anticube forged from triangles.
 *
 */
class AntiCube {

public:
    /**
     * Constructor of the anticube
     */
    AntiCube() : m_nVertexCount(0) {
        build();
    }

    /**
     * Getter on the pointer data on vertices.
     * @return Pointer of ShapeVertex
     */
    const ShapeVertex *getDataPointer() const;

    /**
     * Getter on the number of vertices in the anticube
     * @return the number of vertices in the anticube
     */
    GLsizei getVertexCount() const;

private:
    /**
     * A vector of ShapeVertex
     */
    std::vector <glimac::ShapeVertex> m_Vertices;

    /**
     * The number of vertex
     */
    GLsizei m_nVertexCount;

    /**
     * Function building the anticube (the face are created clockwise)
     */
    void build();

    /**
     * Generate the front face of the anticube
     */
    void generateFrontFace();

    /**
     * Generate the left face of the anticube
     */
    void generateLeftFace();

    /**
     * Generate the back face of the anticube
     */
    void generateBackFace();

    /**
     * Generate the right face of the anticube
     */
    void generateRightFace();

    /**
     * Generate the top face of the anticube
     */
    void generateTopFace();

    /**
     * Generate the bottom face of the anticube
     */
    void generateBottomFace();
};

#endif //TEMPLATE_ANTICUBE_HPP
