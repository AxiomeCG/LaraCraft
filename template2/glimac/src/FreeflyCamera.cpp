//
// Created by Matthias Hudelot on 24/12/2019.
//

#include <cmath>
#include <vector>
#include <iostream>
#include "glimac/common.hpp"
#include "glimac/FreeflyCamera.hpp"

namespace glimac {

    void FreeflyCamera::computeDirectionVectors() {
        m_FrontVector = glm::vec3(cos(m_fTheta)*sin(m_fPhi), sin(m_fTheta), cos(m_fTheta)*cos(m_fPhi));
        m_LeftVector = glm::vec3(sin(m_fPhi + M_PI_2), 0, cos(m_fPhi + M_PI_2));
        m_UpVector = glm::cross(m_FrontVector, m_LeftVector);
    }

    void FreeflyCamera::moveLeft(float t){
        m_Position += t * m_LeftVector;
    }

    void FreeflyCamera::moveFront(float t){
        m_Position += t * m_FrontVector;
    }

    void FreeflyCamera::rotateLeft(float degrees){
        m_fPhi += glm::radians(degrees);
        computeDirectionVectors();
    }

    void FreeflyCamera::rotateUp(float degrees){
        m_fTheta += glm::radians(degrees);
        computeDirectionVectors();
    }

    glm::mat4 FreeflyCamera::getViewMatrix() const{
        return glm::lookAt(m_Position, m_Position + m_FrontVector, m_UpVector);
    }

}