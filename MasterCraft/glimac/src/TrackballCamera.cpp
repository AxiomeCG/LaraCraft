//
// Created by Matthias Hudelot on 23/12/2019.
//

#include <cmath>
#include <vector>
#include <iostream>
#include "glimac/common.hpp"
#include "glimac/TrackballCamera.hpp"

namespace glimac {

    void TrackballCamera::moveFront(float delta){
        m_fDistance += delta;
    }

    void TrackballCamera::rotateLeft(float degrees) {
        m_fAngleY += degrees;
    }

    void TrackballCamera::rotateUp(float degrees){
        m_fAngleX += degrees;
    }

    glm::mat4 TrackballCamera::getViewMatrix() const {
        glm::mat4 VMatrix = glm::mat4(1.f);
        VMatrix =  glm::translate(VMatrix, glm::vec3(0,0,-m_fDistance));
        VMatrix =  glm::rotate(VMatrix,m_fAngleX, glm::vec3(1,0,0));
        VMatrix =  glm::rotate(VMatrix,m_fAngleY, glm::vec3(0,1,0));
        return VMatrix;
    }

}