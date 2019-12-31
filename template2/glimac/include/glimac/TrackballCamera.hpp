//
// Created by Matthias Hudelot on 23/12/2019.
//

#pragma once

#include "common.hpp"

#ifndef IMACGL_TRACKBALLCAMERA_HPP
#define IMACGL_TRACKBALLCAMERA_HPP

namespace glimac{

class TrackballCamera {
public :
    TrackballCamera():
        m_fDistance(5),
        m_fAngleX(0),
        m_fAngleY(0) {

    }

    TrackballCamera(float distance, float angleX, float angleY):
            m_fDistance(distance),
            m_fAngleX(angleX),
            m_fAngleY(angleY) {

    }

    void moveFront(float delta);

    void rotateLeft(float degrees);

    void rotateUp(float degrees);

    glm::mat4 getViewMatrix() const;




private:
    float m_fDistance; //Distance par rapport au centre de la scène
    float m_fAngleX; //Angle effectue par la camera autour de l'axe des X
    float m_fAngleY; //Angle effectue par la camera autour de l'axe des Y
};
}

#endif //IMACGL_TRACKBALLCAMERA_HPP
