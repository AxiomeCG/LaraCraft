#include <glimac/Program.hpp>
#include <glimac/DirectionalLight.hpp>
#include <glimac/TexturedCubeProgram.hpp>
#include "glimac/ChunkSection.hpp"

glm::mat4 ChunkSection::getCubeModelMatrix(int x, int y, int z) {
    return globalModelMatrix * glm::translate(glm::mat4(), glm::vec3((float) x, (float) y, (float) z));
}

void ChunkSection::draw(const TexturedCubeProgram &program, int vertexCount, const glm::mat4& projMatrix, const glm::mat4& viewMatrix,
                        const DirectionalLight& light) {
    for (int x = -WIDTH/2; x < WIDTH/2; x++) {
        for (int y = -HEIGHT; y < 0; y++) {
            for (int z = -LENGTH; z < LENGTH; z++) {
                drawACube(program, vertexCount, projMatrix, viewMatrix, getCubeModelMatrix(x,y,z), light);
            }
        }
    }
}
