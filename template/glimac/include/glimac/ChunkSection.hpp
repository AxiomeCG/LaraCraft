//
// Created by adam on 01/01/2020.
//

#ifndef IMACGL_CHUNKSECTION_HPP
#define IMACGL_CHUNKSECTION_HPP


#include <vector>
#include "glm.hpp"
#include "SimpleTexturedCubeProgram.hpp"
#include "DirectionalLight.hpp"
#include "HeightMap.hpp"

/**
 * Represents a 16*16*16 bundle of cubes.
 * In first approximation, a chunk section is made of only one texture.
 * A coordinate (x,y,z) coordinate in the matrix can be either empty (no cube) or filled with a cube of the material.
 */
class ChunkSection {
public:
    ChunkSection(const glm::vec2 &position, HeightMap heightMap, int offsetRow, int offsetColumn) : globalModelMatrix(
            glm::translate(glm::mat4(), glm::vec3(position,0))), positionTranslation(vec3(position.x,0, position.y)) {
        assert(heightMap.getWidth() % 16 == 0);
        assert(heightMap.getHeight() % 16 == 0);
        assert(offsetRow % 16 == 0);
        assert(offsetColumn % 16 == 0);

        for (int row = offsetRow; row < WIDTH + offsetRow; row++) {
            std::vector<float> tmpColumn;
            for (int column = offsetColumn; column < LENGTH + offsetColumn; column++) {
                tmpColumn.push_back(heightMap.getHeightData().at(row).at(column));
            }
            localHeightMap.push_back(tmpColumn);
        }


        build();

        /*
        for (auto & localRow : localHeightMap){
            for (auto & value: localRow) {
                std::cout << value << ", ";
            }
            std::cout << std::endl;
        }*/
    }

    void draw(const SimpleTexturedCubeProgram &program, int vertexCount, const mat4 &projMatrix, const mat4 &viewMatrix,
              const DirectionalLight &light);

    /**
    * Getter on the pointer data on vertices.
    * @return
    */
    const ShapeVertex *getDataPointer() const {
        return &m_Vertices[0];
    }

    /**
    * Getter on the vector on vertices.
    * @return
    */
    std::vector<glimac::ShapeVertex> getDataVector() const {
        return m_Vertices;
    }

    /**
     * Getter on the number of vertices in the cube
     * @return the number of vertices in the cube
     */
    GLsizei getVertexCount() const {
        return m_Vertices.size();
    }

private:
    /**
     * Number of cube on the x axis
     */
    static const int WIDTH = 16;
    /**
     * Number of cube on the z axis
     */
    static const int LENGTH = 16;
    /**
     * Number of cube on the y axis
     */
    static const int HEIGHT = 16;

    void build() {

        for (int x = 0; x < WIDTH; ++x) {
            for (int z = 0; z < LENGTH; ++z) {
                for(int height = 0  ; height < (int) localHeightMap.at(x).at(z);height++){
                    //std::cout << "cube (" << x << ","<< height << "," << z << ")" << std::endl;
                    for (int v = 0; v < cube.getVertexCount(); ++v) {
                        ShapeVertex shapeVertex;
                        const ShapeVertex &currentCubeVertex = cube.getDataPointer()[v];
                        float vertexX = currentCubeVertex.position.x/2.;
                        float vertexY = currentCubeVertex.position.y/2.;
                        float vertexZ = currentCubeVertex.position.z/2.;
                        shapeVertex.position =  vec3(vertexX + (float) x + positionTranslation.x, vertexY + height + positionTranslation.y,
                                                    vertexZ + (float) z + positionTranslation.z);
                        //std::cout << shapeVertex.position << std::endl;
                        shapeVertex.normal = vec3(currentCubeVertex.normal);
                        shapeVertex.texCoords = vec2(currentCubeVertex.texCoords);
                        m_Vertices.push_back(shapeVertex);
                        m_nVertexCount++;
                    }
                }

            }
        }
    }

    std::vector<std::vector<float>> localHeightMap;

    std::vector<glimac::ShapeVertex> m_Vertices;

    const Cube cube;

    GLsizei m_nVertexCount;

    glm::mat4 globalModelMatrix;

    glm::vec3 positionTranslation;

    glm::mat4 getCubeModelMatrix(int x, int y, int z);

    static void
    drawACube(const SimpleTexturedCubeProgram &program, int vertexCount, const mat4 &projMatrix, const mat4 &viewMatrix,
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

        glUniform3fv(program.uLightDir_vsId, 1,
                     value_ptr(light.getLightDirection(viewMatrix))); // TODO Check if it's natural
        glUniform3fv(program.uLightIntensityId, 1, value_ptr(light.lightIntensity));
        glDrawArrays(GL_TRIANGLES, 0, vertexCount);
    }

};


#endif //IMACGL_CHUNKSECTION_HPP
