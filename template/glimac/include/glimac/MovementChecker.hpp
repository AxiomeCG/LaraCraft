//
// Created by Matthias Hudelot on 04/02/2020.
//

#ifndef TEMPLATE_MOVEMENTCHECKER_HPP
#define TEMPLATE_MOVEMENTCHECKER_HPP

#include <glimac/HeightMap.hpp>
#include "glm.hpp"

namespace glimac {

class MovementChecker {
public:
    MovementChecker(HeightMap heightMap);

    bool movedToAnotherCube(glm::vec3 futurePosition);

private:
    HeightMap m_heightMap;
    glm::vec3 m_currentPosition;
};

}

#endif //TEMPLATE_MOVEMENTCHECKER_HPP
