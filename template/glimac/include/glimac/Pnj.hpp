//
// Created by adam on 08/02/2020.
//

#ifndef IMACGL_PNJ_HPP
#define IMACGL_PNJ_HPP

#include <glm/vec3.hpp>
#include "Cube.hpp"
#include "MovementChecker.hpp"

class Pnj {
private:
    glm::vec3 direction;
    glm::vec3 position;
    double timeout = 5000.0f;
    MovementChecker movementChecker;

    void generateNewRandomDirection();

public:
    glm::mat4 getModelMatrix() const;

    void updatePosition();

    explicit Pnj(const vec3 &position, const std::vector<std::vector<float>> &heightData,
                 const std::vector<std::vector<glm::vec3>> &vegetationData) :
            position(position),
            movementChecker(heightData, vegetationData) {
        generateNewRandomDirection();
    }

};


#endif //IMACGL_PNJ_HPP
