//
// Created by Matthias Hudelot on 04/02/2020.
//

#include "glimac/MovementChecker.hpp"
#include <cmath>

namespace glimac{

    bool MovementChecker::willMoveToAnotherCube(glm::vec3 currentPosition, glm::vec3 futurePosition) {
        auto futurePosFlooredX = floor(futurePosition.x);
        auto futurePosFlooredZ = floor(futurePosition.z);
        auto currentPosFlooredX = floor(currentPosition.x);
        auto currentPosFlooredZ = floor(currentPosition.z);
        if((futurePosFlooredX == currentPosFlooredX) && (futurePosFlooredZ == currentPosFlooredZ)) {
            return false;
        }
        return true;
    }

    bool MovementChecker::canMoveToTheOtherCube(glm::vec3 currentPosition, glm::vec3 futurePosition) {
        if(futurePosition.x < 0 || futurePosition.x > m_width) {
            std::cout << "First false" << std::endl;
            return false;
        }
        if(futurePosition.z < 0 || futurePosition.z > m_height) {
            std::cout << "Second false" << std::endl;
            return false;
        }
        auto futurePosFlooredX = floor(futurePosition.x);
        auto futurePosFlooredZ = floor(futurePosition.z);
        auto currentPosFlooredX = floor(currentPosition.x);
        auto currentPosFlooredZ = floor(currentPosition.z);
        auto heightDifference =  m_heightData[futurePosFlooredX][futurePosFlooredZ] - m_heightData[currentPosFlooredX][currentPosFlooredZ];
        if(heightDifference > 1) {
            std::cout << "Third false" << std::endl;
            return false;
        }
        return true;
    }

    glm::vec3 MovementChecker::leveledFuturePosition(glm::vec3 futurePosition, float heightOfEntity) {
        auto futurePosFlooredX = floor(futurePosition.x);
        auto futurePosFlooredZ = floor(futurePosition.z);
        auto futureY = m_heightData[futurePosFlooredX][futurePosFlooredZ];
        return glm::vec3(futurePosition.x, futureY + heightOfEntity, futurePosition.z);
    }

}