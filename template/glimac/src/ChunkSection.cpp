#include "glimac/ChunkSection.hpp"

glm::mat4 ChunkSection::getCubeModelMatrix(int x, int y, int z) {
    return globalModelMatrix * glm::translate(glm::mat4(), glm::vec3((float) x, (float) y, (float) z));
}

/**
 * Draws a chunk section (16*16*16 cubes). Be aware that cube are 2*2*2 in size so it is mandatory to shift them by 2*coordinate for not having cube inside another.
 * @param program
 * @param vertexCount
 * @param projMatrix
 * @param viewMatrix
 * @param light
 */
void ChunkSection::draw(const SimpleTexturedCubeProgram &program, int vertexCount, const glm::mat4& projMatrix, const glm::mat4& viewMatrix,
                        const DirectionalLight& light) {
    for (int x = -WIDTH/2; x < WIDTH/2 -1; x++) {
        for (int y = -HEIGHT + 1; y < 0; y++) {
            for (int z = -LENGTH/2; z < LENGTH/2 -1; z++) {
                drawACube(program, vertexCount, projMatrix, viewMatrix, getCubeModelMatrix(2*x,2*y,2*z), light);
            }
        }
    }
}
