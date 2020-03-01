//
// Created by adam on 08/02/2020.
//

#ifndef IMACGL_PNJ_HPP
#define IMACGL_PNJ_HPP

#include <glm/vec3.hpp>
#include "Cube.hpp"
#include "MovementChecker.hpp"

/**
 * Represent a PNJ
 */
class Pnj {
private:
    /**
     * Vector direction of the PNJ
     */
    glm::vec3 direction;

    /**
     * Position of the PNJ
     */
    glm::vec3 position;

    /**
     * Timeout for changing the direction
     */
    double timeout = 5000.0f;

    /**
     * MovementChecker allowing movement
     */
    MovementChecker movementChecker;

    /**
     * Generation a new direction
     */
    void generateNewRandomDirection();

public:

    /**
     * Getter for the ModelMatrix of the PNJ
     * @return ModelMatrix
     */
    glm::mat4 getModelMatrix() const;

    /**
     * Update the position of the PNJ
     */
    void updatePosition();

    /**
     * Constructor for the PNJ
     * @param position Position of the PNJ
     * @param heightData Data of the HeightMap used
     * @param vegetationData Data of the VegetationMap used
     */
    explicit Pnj(const vec3 &position, const std::vector<std::vector<float>> &heightData,
                 const std::vector<std::vector<glm::vec3>> &vegetationData) :
            position(position),
            movementChecker(heightData, vegetationData) {
        generateNewRandomDirection();
    }

};


#endif //IMACGL_PNJ_HPP
