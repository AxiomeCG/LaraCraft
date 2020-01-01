//
// Created by adam on 01/01/2020.
//

#ifndef IMACGL_DIRECTIONALLIGHT_HPP
#define IMACGL_DIRECTIONALLIGHT_HPP

#include <glimac/glm.hpp>
#include <GLFW/glfw3.h>

class DirectionalLight {
public:
    glm::mat4 lightModelMatrix = glm::rotate(glm::mat4(), (float) glfwGetTime(), glm::vec3(0., 1., 0.));
    glm::vec3 lightIntensity = glm::vec3(1., 1., 1.);

    glm::vec3 getLightDirection(const glm::mat4 &viewMatrix) const{
        return glm::vec3(viewMatrix * glm::vec4(1., 1., 1., 0.) * lightModelMatrix);
    }

};

#endif //IMACGL_DIRECTIONALLIGHT_HPP
