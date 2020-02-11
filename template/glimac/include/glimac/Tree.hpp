//
// Created by adam on 11/02/2020.
//

#ifndef IMACGL_TREE_HPP
#define IMACGL_TREE_HPP

#include <vector>
#include "common.hpp"
#include "Cube.hpp"

class Tree {
public:
    explicit Tree(const glm::vec3 &position);
    glimac::ShapeVertex *getDataPointer();
    GLsizei getVertexCount() const;

private:
    void build();
    std::vector<glimac::ShapeVertex> vertexList;
    const Cube cube;
    const glm::vec3 positionTranslation;

    void generateACube(const glm::vec3& localPosition, float texture);
};
#endif //IMACGL_TREE_HPP
