//
// Created by Matthias Hudelot on 04/02/2020.
//

#include "glimac/MovementChecker.hpp"
#include <cmath>

namespace glimac{

    MovementChecker::MovementChecker(HeightMap heightMap) : m_heightMap(heightMap) {
        auto heightData = heightMap.getHeightData();
        m_currentPosition = glm::vec3(0.f, 0.f, heightData[0][0]);
    }

    bool MovementChecker::movedToAnotherCube(glm::vec3 futurePosition) {
        auto futurePosFlooredX = floor(futurePosition.x);
        auto futurePosFlooredY = floor(futurePosition.y);
        auto currentPosFlooredX = floor(m_currentPosition.x);
        auto currentPosFlooredY = floor(m_currentPosition.y);
        if((futurePosFlooredX == currentPosFlooredX) && (futurePosFlooredY == currentPosFlooredY)) {
            return false;
        }
        return false;
    }


}