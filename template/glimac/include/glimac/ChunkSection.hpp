//
// Created by adam on 01/01/2020.
//

#ifndef IMACGL_CHUNKSECTION_HPP
#define IMACGL_CHUNKSECTION_HPP


#include <vector>
#include "glm.hpp"
#include "SimpleTexturedCubeProgram.hpp"
#include "DirectionalLight.hpp"

/**
 * Represents a 16*16*16 bundle of cubes.
 * In first approximation, a chunk section is made of only one texture.
 * A coordinate (x,y,z) coordinate in the matrix can be either empty (no cube) or filled with a cube of the material.
 */
class ChunkSection {
public:
    ChunkSection(const glm::vec3 &position) : globalModelMatrix(glm::translate(glm::mat4(), position)) {

    }

    void draw(const SimpleTexturedCubeProgram &program, int vertexCount, const mat4 &projMatrix, const mat4 &viewMatrix,
              const DirectionalLight &light);
private:
    /**
     * Number of cube on the x axis
     */
    static const int WIDTH = 4;
    /**
     * Number of cube on the z axis
     */
    static const int LENGTH = 4;
    /**
     * Number of cube on the y axis
     */
    static const int HEIGHT = 4;

    glm::mat4 globalModelMatrix;

    glm::mat4 getCubeModelMatrix(int x, int y, int z);

    static void drawACube(const SimpleTexturedCubeProgram &program, int vertexCount, const mat4 &projMatrix, const mat4 &viewMatrix,
                          const mat4 &modelMatrix, const DirectionalLight &light) {
        mat4 ModelViewMatrix2 = viewMatrix * modelMatrix;
        mat4 ModelViewProjectionMatrix2 = projMatrix * ModelViewMatrix2;
        mat4 NormalMatrix2 = transpose(glm::inverse(ModelViewMatrix2));
        glUniformMatrix4fv(program.uMVPMatrixId, 1, GL_FALSE, value_ptr(ModelViewProjectionMatrix2));
        glUniformMatrix4fv(program.uMVMatrixId, 1, GL_FALSE, value_ptr(ModelViewMatrix2));
        glUniformMatrix4fv(program.uNormalMatrixId, 1, GL_FALSE, value_ptr(NormalMatrix2));

        const vec3 &kd2 = vec3(1., 1., 1.);
        const vec3 &ks2 = vec3(1., 1., 1.);
        const float shininess2 = 10.;
        glUniform3fv(program.uKdId, 1, value_ptr(kd2));
        glUniform3fv(program.uKsId, 1, value_ptr(ks2));
        glUniform1f(program.uShininessId, shininess2);

        glUniform3fv(program.uLightDir_vsId, 1, value_ptr(light.getLightDirection(viewMatrix))); // TODO Check if it's natural
        glUniform3fv(program.uLightIntensityId, 1, value_ptr(light.lightIntensity));
        glDrawArrays(GL_TRIANGLES, 0, vertexCount);
    }

};


#endif //IMACGL_CHUNKSECTION_HPP
