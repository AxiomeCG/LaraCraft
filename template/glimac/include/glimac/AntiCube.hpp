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
 * Represents an elementary cube forged from triangles.
 *
 */
class AntiCube {

public:
    /**
     * Constructor of the cube
     */
    AntiCube() : m_nVertexCount(0) {
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
    std::vector<glimac::ShapeVertex> m_Vertices;
    GLsizei m_nVertexCount;

    void build();

    static uint32_t indices[];

    void generateFrontFace();

    void generateLeftFace();

    void generateBackFace();

    void generateRightFace();

    void generateTopFace();

    void generateBottomFace();
};

#endif //TEMPLATE_ANTICUBE_HPP
