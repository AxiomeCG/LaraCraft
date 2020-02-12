//
// Created by Matthias Hudelot on 04/02/2020.
//

#include "glimac/MovementChecker.hpp"
#include <iostream>
#include <cmath>

namespace glimac{

    bool MovementChecker::isWithinBoundaries(glm::vec3 futurePosition) {
        return (futurePosition.x >= 0 && futurePosition.x <= m_width) && (futurePosition.z >= 0 && futurePosition.z <= m_height);
    }

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
        if(isWithinBoundaries(futurePosition)) {
            auto futurePosFlooredX = floor(futurePosition.x);
            auto futurePosFlooredZ = floor(futurePosition.z);
            auto currentPosFlooredX = floor(currentPosition.x);
            auto currentPosFlooredZ = floor(currentPosition.z);
            auto heightDifference = m_heightData[futurePosFlooredX][futurePosFlooredZ] -
                                    m_heightData[currentPosFlooredX][currentPosFlooredZ];
            if (heightDifference > 1) {
                return false;
            }
            if(m_vegetationData[futurePosFlooredX][futurePosFlooredZ].x) {
                return false;
            }
            return true;
        }
        return false;
    }

    glm::vec3 MovementChecker::leveledFuturePosition(glm::vec3 currentPosition, glm::vec3 futurePosition, float heightOfEntity) {
        auto futurePosFlooredX = floor(futurePosition.x);
        auto futurePosFlooredZ = floor(futurePosition.z);
        if(willMoveToAnotherCube(currentPosition, futurePosition)) {
            if(canMoveToTheOtherCube(currentPosition, futurePosition)) {
                auto futureY = floor((float) m_heightData[futurePosFlooredX][futurePosFlooredZ]);
                return glm::vec3(futurePosition.x, futureY + heightOfEntity, futurePosition.z);
            }
            return currentPosition;
        }
        auto futureY = floor((float) m_heightData[futurePosFlooredX][futurePosFlooredZ]);
        return glm::vec3(futurePosition.x, futureY + heightOfEntity, futurePosition.z);

    }

}