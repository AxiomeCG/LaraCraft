//
// Created by adam on 30/12/2019.
//

#include "glupem/Cube.hpp"

uint32_t Cube::indices[] = {
        0, 1, 2, 0, 2, 3
};

void Cube::build() {
    generateFrontFace();
}

void Cube::generateFrontFace() {
    // FACE 1 (FRONT) (4 vertices (SQUARE) --> 6 vertices (2 TRIANGLES)

    //Triangle 1
    // TOP LEFT CORNER
    ShapeVertex vertex;

    vertex.texCoords.x = 0.f;
    vertex.texCoords.y = 0.f;

    vertex.normal.x = 1.f;
    vertex.normal.y = 0.f;
    vertex.normal.z = 0.f;

    vertex.position = glm::vec3(-1.f, 1.f, 1.f);

    m_Vertices.push_back(vertex);

    //BOTTOM LEFT CORNER
    vertex.texCoords.x = 0.f;
    vertex.texCoords.y = 0.f;

    vertex.normal.x = 0.f;
    vertex.normal.y = 1.f;
    vertex.normal.z = 0.f;

    vertex.position = vec3(-1.f, -1.f, 1.f);

    m_Vertices.push_back(vertex);

    //BOTTOM RIGHT CORNER
    vertex.texCoords.x = 0.f;
    vertex.texCoords.y = 0.f;

    vertex.normal.x = 0.f;
    vertex.normal.y = 0.f;
    vertex.normal.z = 1.f;

    vertex.position = vec3(1.f, -1.f, 1.f);

    m_Vertices.push_back(vertex);

    //Triangle 2
    //TOP RIGHT CORNER
    vertex.texCoords.x = 0.f;
    vertex.texCoords.y = 0.f;

    vertex.normal.x = 1.f;
    vertex.normal.y = 1.f;
    vertex.normal.z = 0.f;

    vertex.position = vec3(1.f, 1.f, 1.f);
    m_Vertices.push_back(vertex);


    //TOP LEFT CORNER
    vertex.texCoords.x = 0.f;
    vertex.texCoords.y = 0.f;

    vertex.normal.x = 1.f;
    vertex.normal.y = 0.f;
    vertex.normal.z = 0.f;

    vertex.position = glm::vec3(-1.f, 1.f, 1.f);

    m_Vertices.push_back(vertex);


    //BOTTOM RIGHT CORNER
    vertex.texCoords.x = 0.f;
    vertex.texCoords.y = 0.f;

    vertex.normal.x = 0.f;
    vertex.normal.y = 0.f;
    vertex.normal.z = 1.f;

    vertex.position = vec3(1.f, -1.f, 1.f);

    m_Vertices.push_back(vertex);

    m_nVertexCount += 6;
}

void Cube::generateLeftFace() {
    // FACE LEFT (4 vertices (SQUARE) --> 6 vertices (2 TRIANGLES)
    // Triangle 1
    // TOP LEFT CORNER
    ShapeVertex vertex;

    vertex.texCoords.x = 0.f;
    vertex.texCoords.y = 0.f;

    vertex.normal.x = 1.f;
    vertex.normal.y = 0.f;
    vertex.normal.z = 0.f;

    vertex.position = glm::vec3(-1.f, 1.f, -1.f);

    m_Vertices.push_back(vertex);

    //BOTTOM LEFT CORNER
    vertex.texCoords.x = 0.f;
    vertex.texCoords.y = 0.f;

    vertex.normal.x = 0.f;
    vertex.normal.y = 1.f;
    vertex.normal.z = 0.f;

    vertex.position = vec3(-1.f, -1.f, -1.f);

    m_Vertices.push_back(vertex);

    //BOTTOM RIGHT CORNER
    vertex.texCoords.x = 0.f;
    vertex.texCoords.y = 0.f;

    vertex.normal.x = 0.f;
    vertex.normal.y = 0.f;
    vertex.normal.z = 1.f;

    vertex.position = vec3(-1.f, -1.f, 1.f);

    m_Vertices.push_back(vertex);

    //Triangle 2
    //TOP RIGHT CORNER
    vertex.texCoords.x = 0.f;
    vertex.texCoords.y = 0.f;

    vertex.normal.x = 1.f;
    vertex.normal.y = 1.f;
    vertex.normal.z = 0.f;

    vertex.position = vec3(-1.f, 1.f, 1.f);
    m_Vertices.push_back(vertex);
    m_nVertexCount += 6;
}

GLsizei Cube::getVertexCount() const {
    return m_nVertexCount;
}

const glimac::ShapeVertex *Cube::getDataPointer() const {
    return &m_Vertices[0];
}

const uint32_t *Cube::getIBOIndices() const {
    return indices;
}

GLsizei Cube::getIndiceCount() const{
    return 6;
}