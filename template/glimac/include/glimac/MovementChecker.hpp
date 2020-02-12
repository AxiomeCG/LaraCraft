//
// Created by Matthias Hudelot on 04/02/2020.
//

#ifndef TEMPLATE_MOVEMENTCHECKER_HPP
#define TEMPLATE_MOVEMENTCHECKER_HPP

#include <vector>
#include "glm.hpp"

namespace glimac {

class MovementChecker {
public:
    MovementChecker(std::vector<std::vector<float>> heightData, std::vector<std::vector<glm::vec3>> vegetationData):
        m_width(heightData.size()),
        m_height(heightData[0].size()),
        m_heightData(heightData),
        m_vegetationData(vegetationData){

    }

    bool isWithinBoundaries(glm::vec3 futurePosition);

    bool canMoveToTheOtherCube(glm::vec3 currentPosition, glm::vec3 futurePosition);

    glm::vec3 leveledFuturePosition(glm::vec3 currentPosition, glm::vec3 futurePosition, float heightOfEntity);

private:
    unsigned int m_width;
    unsigned int m_height;
    std::vector<std::vector<float>> m_heightData;
    std::vector<std::vector<glm::vec3>> m_vegetationData;

    bool willMoveToAnotherCube(glm::vec3 currentPosition, glm::vec3 futurePosition);
};

}

#endif //TEMPLATE_MOVEMENTCHECKER_HPP
