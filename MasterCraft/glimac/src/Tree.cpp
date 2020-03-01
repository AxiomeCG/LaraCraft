//
// Created by adam on 11/02/2020.
//

#include <glimac/TextureOffsetEnum.hpp>
#include <glimac/Tree.hpp>

glimac::ShapeVertex *Tree::getDataPointer() { return &vertexList[0]; }

GLsizei Tree::getVertexCount() const { return vertexList.size(); }

void Tree::build() {
  /**
   *
   */
  generateACube(glm::vec3(0., 0., 0.), TextureOffsetEnum::woodTextureOffset);
  generateACube(glm::vec3(0., 1., 0.), TextureOffsetEnum::woodTextureOffset);
  generateACube(glm::vec3(0., 2., 0.), TextureOffsetEnum::woodTextureOffset);
  generateACube(glm::vec3(0., 3., 0.), TextureOffsetEnum::woodTextureOffset);
  generateACube(glm::vec3(0., 4., 0.), TextureOffsetEnum::woodTextureOffset);
  generateACube(glm::vec3(0., 5., 0.), TextureOffsetEnum::woodTextureOffset);

  /**
   * LEVEL 3 LEAF
   */

  generateACube(glm::vec3(1., 3., 1.), TextureOffsetEnum::leafTextureOffset);
  generateACube(glm::vec3(1., 3., 0.), TextureOffsetEnum::leafTextureOffset);
  generateACube(glm::vec3(0., 3., 1.), TextureOffsetEnum::leafTextureOffset);
  generateACube(glm::vec3(-1., 3., -1.), TextureOffsetEnum::leafTextureOffset);
  generateACube(glm::vec3(-1., 3., 0.), TextureOffsetEnum::leafTextureOffset);
  generateACube(glm::vec3(0., 3., -1.), TextureOffsetEnum::leafTextureOffset);
  generateACube(glm::vec3(-1., 3., 1.), TextureOffsetEnum::leafTextureOffset);
  generateACube(glm::vec3(1., 3., -1.), TextureOffsetEnum::leafTextureOffset);

  generateACube(glm::vec3(0., 3., -2.), TextureOffsetEnum::leafTextureOffset);
  generateACube(glm::vec3(0., 3., 2.), TextureOffsetEnum::leafTextureOffset);
  generateACube(glm::vec3(-2., 3., 0.), TextureOffsetEnum::leafTextureOffset);
  generateACube(glm::vec3(2., 3., 0.), TextureOffsetEnum::leafTextureOffset);

  generateACube(glm::vec3(1., 3., -2.), TextureOffsetEnum::leafTextureOffset);
  generateACube(glm::vec3(-2., 3., 1.), TextureOffsetEnum::leafTextureOffset);

  generateACube(glm::vec3(1., 3., 2.), TextureOffsetEnum::leafTextureOffset);
  generateACube(glm::vec3(2., 3., 1.), TextureOffsetEnum::leafTextureOffset);

  generateACube(glm::vec3(-1., 3., 2.), TextureOffsetEnum::leafTextureOffset);
  generateACube(glm::vec3(2., 3., -1.), TextureOffsetEnum::leafTextureOffset);

  generateACube(glm::vec3(-2., 3., -1.), TextureOffsetEnum::leafTextureOffset);
  generateACube(glm::vec3(-1., 3., -2.), TextureOffsetEnum::leafTextureOffset);

  generateACube(glm::vec3(-2., 3., 1.), TextureOffsetEnum::leafTextureOffset);
  generateACube(glm::vec3(1., 3., -2.), TextureOffsetEnum::leafTextureOffset);

  generateACube(glm::vec3(2., 3., 1.), TextureOffsetEnum::leafTextureOffset);
  generateACube(glm::vec3(1., 3., 2.), TextureOffsetEnum::leafTextureOffset);

  generateACube(glm::vec3(2., 3., -2.), TextureOffsetEnum::leafTextureOffset);
  generateACube(glm::vec3(-2., 3., 2.), TextureOffsetEnum::leafTextureOffset);

  generateACube(glm::vec3(2., 3., 2.), TextureOffsetEnum::leafTextureOffset);
  generateACube(glm::vec3(-2., 3., -2.), TextureOffsetEnum::leafTextureOffset);

  /**
   * LEVEL 4 LEAF
   */
  generateACube(glm::vec3(1., 4., 1.), TextureOffsetEnum::leafTextureOffset);
  generateACube(glm::vec3(1., 4., 0.), TextureOffsetEnum::leafTextureOffset);
  generateACube(glm::vec3(0., 4., 1.), TextureOffsetEnum::leafTextureOffset);
  generateACube(glm::vec3(-1., 4., -1.), TextureOffsetEnum::leafTextureOffset);
  generateACube(glm::vec3(-1., 4., 0.), TextureOffsetEnum::leafTextureOffset);
  generateACube(glm::vec3(0., 4., -1.), TextureOffsetEnum::leafTextureOffset);
  generateACube(glm::vec3(-1., 4., 1.), TextureOffsetEnum::leafTextureOffset);
  generateACube(glm::vec3(1., 4., -1.), TextureOffsetEnum::leafTextureOffset);

  generateACube(glm::vec3(0., 4., -2.), TextureOffsetEnum::leafTextureOffset);
  generateACube(glm::vec3(0., 4., 2.), TextureOffsetEnum::leafTextureOffset);
  generateACube(glm::vec3(-2., 4., 0.), TextureOffsetEnum::leafTextureOffset);
  generateACube(glm::vec3(2., 4., 0.), TextureOffsetEnum::leafTextureOffset);
  /**
   * LEVEL 5 LEAF
   */
  generateACube(glm::vec3(1., 5., 1.), TextureOffsetEnum::leafTextureOffset);
  generateACube(glm::vec3(1., 5., 0.), TextureOffsetEnum::leafTextureOffset);
  generateACube(glm::vec3(0., 5., 1.), TextureOffsetEnum::leafTextureOffset);
  generateACube(glm::vec3(-1., 5., -1.), TextureOffsetEnum::leafTextureOffset);
  generateACube(glm::vec3(-1., 5., 0.), TextureOffsetEnum::leafTextureOffset);
  generateACube(glm::vec3(0., 5., -1.), TextureOffsetEnum::leafTextureOffset);
  generateACube(glm::vec3(-1., 5., 1.), TextureOffsetEnum::leafTextureOffset);
  generateACube(glm::vec3(1., 5., -1.), TextureOffsetEnum::leafTextureOffset);

  /**
   * LEVEL 6 LEAF
   */
  generateACube(glm::vec3(0., 6., 0.), TextureOffsetEnum::leafTextureOffset);
}

void Tree::generateACube(const glm::vec3 &localPosition, float texture) {
  for (int v = 0; v < cube.getVertexCount(); ++v) {
    ShapeVertex shapeVertex;
    const ShapeVertex &currentCubeVertex = cube.getDataPointer()[v];
    double vertexX = currentCubeVertex.position.x / 2.;
    // The cube is defined from -1 to 1 so it's size 2. Divide by two to get a
    // 1x1x1 size cube
    double vertexY = currentCubeVertex.position.y / 2.;
    double vertexZ = currentCubeVertex.position.z / 2.;

    shapeVertex.position =
        vec3(vertexX + positionTranslation.x + localPosition.x,
             vertexY + positionTranslation.y + localPosition.y,
             vertexZ + positionTranslation.z + localPosition.z);

    // std::cout << shapeVertex.position << std::endl;
    shapeVertex.normal = vec3(currentCubeVertex.normal);

    shapeVertex.texCoords =
        vec2(texture + currentCubeVertex.texCoords.x /
                           TextureOffsetEnum::NUMBER_TEXTURE,
             currentCubeVertex.texCoords.y); // + offset
    vertexList.push_back(shapeVertex);
  }
}

Tree::Tree(const glm::vec3 &position)
    : vertexList(0), positionTranslation(position) {
  build();
}
