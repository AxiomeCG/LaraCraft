//
// Created by Matthias Hudelot on 04/02/2020.
//

#ifndef TEMPLATE_CONSTRAINEDCAMERA_HPP
#define TEMPLATE_CONSTRAINEDCAMERA_HPP

#include "HeightMap.hpp"
#include "MovementChecker.hpp"

namespace glimac {

class ConstrainedCamera {
public :

    ConstrainedCamera(std::vector<std::vector<float>> heightData, float heigthOfEntity):
            m_fPhi(M_PI),
            m_fTheta(0),
            m_heightOfEntity(heigthOfEntity),
            m_movementChecker(heightData){
        float y = heightData[0.f][0.f] + 2.f;
        m_Position = glm::vec3(0.f, y + m_heightOfEntity, 0.f);
        computeDirectionVectors();
    }

    ConstrainedCamera(float x, float z, std::vector<std::vector<float>> heightData, float heightOfEntity):
            m_fPhi(M_PI),
            m_fTheta(0),
            m_heightOfEntity(heightOfEntity),
            m_movementChecker(heightData){
        float y = heightData[x][z];
        m_Position = glm::vec3(x, y + m_heightOfEntity, z);
        computeDirectionVectors();
    }

    void moveLeft(float t);
    void moveFront(float t);

    void rotateLeft(float degrees);
    void rotateUp(float degrees);

    glm::mat4 getViewMatrix() const;


    glm::vec3 getPosition() const ;

private :

    void computeDirectionVectors();

    glm::vec3 m_Position;
    float m_fPhi;
    float m_fTheta;
    glm::vec3 m_FrontVector;
    glm::vec3 m_LeftVector;
    glm::vec3 m_UpVector;

    float m_heightOfEntity;

    MovementChecker m_movementChecker;
};

}

#endif //TEMPLATE_CONSTRAINEDCAMERA_HPP
