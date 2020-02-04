//
// Created by Matthias Hudelot on 04/02/2020.
//

#include <cmath>
#include <vector>
#include <iostream>
#include "glimac/common.hpp"
#include "glimac/FreeflyCamera.hpp"
#include "glimac/ConstrainedCamera.hpp"

namespace glimac {

    void ConstrainedCamera::computeDirectionVectors() {
        m_FrontVector = glm::vec3(cos(m_fTheta)*sin(m_fPhi), sin(m_fTheta), cos(m_fTheta)*cos(m_fPhi));
        m_LeftVector = glm::vec3(sin(m_fPhi + M_PI_2), 0, cos(m_fPhi + M_PI_2));
        m_UpVector = glm::cross(m_FrontVector, m_LeftVector);
    }

    void ConstrainedCamera::moveLeft(float t){
        auto tmpPosition = m_Position + (t * m_LeftVector);
        std::cout << "Will Move Side" << std::endl;
        std::cout << "X : " <<  tmpPosition.x << " Y : " << tmpPosition.y << " Z : " << tmpPosition.z << std::endl;
        if(m_movementChecker.willMoveToAnotherCube(m_Position, tmpPosition)) {
            std::cout << "Will Move to another cube" << std::endl;
            if (m_movementChecker.canMoveToTheOtherCube(m_Position, tmpPosition)) {
                std::cout << "Can Move to another cube" << std::endl;
                tmpPosition = m_movementChecker.leveledFuturePosition(tmpPosition, m_heightOfEntity);
                m_Position = tmpPosition;
            }
        }
        else {
            m_Position = m_movementChecker.leveledFuturePosition(tmpPosition, m_heightOfEntity);
        }
    }

    void ConstrainedCamera::moveFront(float t){
        auto tmpPosition = m_Position + (t * m_FrontVector);
        std::cout << "Will Move Front" << std::endl;
        std::cout << "X : " <<  tmpPosition.x << " Y : " << tmpPosition.y << " Z : " << tmpPosition.z << std::endl;
        if(m_movementChecker.willMoveToAnotherCube(m_Position, tmpPosition)) {
            std::cout << "Will Move to another cube" << std::endl;
            if (m_movementChecker.canMoveToTheOtherCube(m_Position, tmpPosition)) {
                std::cout << "Can Move to another cube" << std::endl;
                m_Position = m_movementChecker.leveledFuturePosition(tmpPosition, m_heightOfEntity);
            }
        }
        else {
            m_Position = m_movementChecker.leveledFuturePosition(tmpPosition, m_heightOfEntity);
        }
    }

    void ConstrainedCamera::rotateLeft(float degrees){
        m_fPhi += glm::radians(degrees);
        computeDirectionVectors();
    }

    void ConstrainedCamera::rotateUp(float degrees){
        m_fTheta += glm::radians(degrees);
        computeDirectionVectors();
    }

    glm::mat4 ConstrainedCamera::getViewMatrix() const{
        return glm::lookAt(m_Position, m_Position + m_FrontVector, m_UpVector);
    }

}