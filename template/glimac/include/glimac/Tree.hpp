//
// Created by adam on 11/02/2020.
//

#ifndef IMACGL_TREE_HPP
#define IMACGL_TREE_HPP

#include <vector>
#include "common.hpp"
#include "Cube.hpp"

/**
 * Represent a tree
 */
class Tree {
public:

    /**
     * Constructor for the Tree
     * @param position Position of the tree
     */
    explicit Tree(const glm::vec3 &position);

    /**
     * Getter on the pointer data on vertices.
     * @return Pointer of ShapeVertex
     */
    glimac::ShapeVertex *getDataPointer();

    /**
     * Getter for the number of vertex
     * @return Number of vertex
     */
    GLsizei getVertexCount() const;

private:

    /**
     * Build the tree
     */
    void build();

    /**
     * A vector of ShapeVertex
     */
    std::vector<glimac::ShapeVertex> vertexList;

    /**
     * A cube
     */
    const Cube cube;

    /**
     * Vector used to translate the cube when building the tree
     */
    const glm::vec3 positionTranslation;

    /**
     * Generate a Cube
     * @param localPosition Position local
     * @param texture
     */
    void generateACube(const glm::vec3 &localPosition, float texture);
};

#endif //IMACGL_TREE_HPP
