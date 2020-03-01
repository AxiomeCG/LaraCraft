//
// Created by adam on 01/01/2020.
//

#ifndef IMACGL_DIRECTIONALLIGHT_HPP
#define IMACGL_DIRECTIONALLIGHT_HPP

#include <glimac/glm.hpp>
#include <GLFW/glfw3.h>

/**
 * Represent a light object
 */
class DirectionalLight {
public:
    /**
     * Constructor for DirectionalLight
     * @param modelMatrix ModelMatrix of the light object
     */
    explicit DirectionalLight(const glm::mat4 &modelMatrix) : lightModelMatrix(modelMatrix) {

    }

    /**
     * ModelMatrix of the light object
     */
    glm::mat4 lightModelMatrix;

    /**
     * Vector for the intensity of the light object
     */
    glm::vec3 lightIntensity = glm::vec3(1., 1., 1.);

    /**
     * Getter for the light direction of the light object
     * @param viewMatrix ViewMatrix used to compute the light direction
     * @return Vector of the light direction
     */
    glm::vec3 getLightDirection(const glm::mat4 &viewMatrix) const {
        return glm::vec3(viewMatrix * lightModelMatrix * glm::vec4(1., 1., 1., 0.));
    }

};

#endif //IMACGL_DIRECTIONALLIGHT_HPP
