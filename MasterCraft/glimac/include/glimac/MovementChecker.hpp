//
// Created by Matthias Hudelot on 04/02/2020.
//

#ifndef TEMPLATE_MOVEMENTCHECKER_HPP
#define TEMPLATE_MOVEMENTCHECKER_HPP

#include <vector>
#include "glm.hpp"

namespace glimac {

    /**
     * MovementChecker allowing movement or not
     */
    class MovementChecker {
    public:
        /**
         * Constructor for the MovementChecker
         * @param heightData Data of the HeightMap
         * @param vegetationData Data of the VegetationMap
         */
        MovementChecker(std::vector <std::vector<float>> heightData,
                        std::vector <std::vector<glm::vec3>> vegetationData) :
                m_width(heightData.size()),
                m_height(heightData[0].size()),
                m_heightData(heightData),
                m_vegetationData(vegetationData) {

        }

        /**
         * Test whether or not the future position is within the boundaries of the map
         * @param futurePosition Future position of the entity
         * @return true if within boundaries, false otherwise
         */
        bool isWithinBoundaries(glm::vec3 futurePosition);

        /**
         * Test whether or not the entity can move to another cube
         * @param currentPosition Current position of the entity
         * @param futurePosition Future position of the entity
         * @return true if can move, false otherwise
         */
        bool canMoveToTheOtherCube(glm::vec3 currentPosition, glm::vec3 futurePosition);

        /**
         * Level future position
         * @param currentPosition Current position of the entity
         * @param futurePosition Future position of the entity
         * @param heightOfEntity Height of the entity
         * @return
         */
        glm::vec3 leveledFuturePosition(glm::vec3 currentPosition, glm::vec3 futurePosition, float heightOfEntity);

    private:

        /**
         * Width of the maps
         */
        unsigned int m_width;

        /**
         * Height of the maps
         */
        unsigned int m_height;

        /**
         * Data of the HeightMap
         */
        std::vector <std::vector<float>> m_heightData;

        /**
         * Data of the VegetationMap
         */
        std::vector <std::vector<glm::vec3>> m_vegetationData;

        /**
         * Test whether or not the entity will move to another cube
         * @param currentPosition Current position of the entity
         * @param futurePosition Future position of the entity
         * @return true if will move to another cube, false otherwise
         */
        bool willMoveToAnotherCube(glm::vec3 currentPosition, glm::vec3 futurePosition);
    };

}

#endif //TEMPLATE_MOVEMENTCHECKER_HPP
