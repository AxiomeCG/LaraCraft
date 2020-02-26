//
// Created by Matthias Hudelot on 04/02/2020.
//

#ifndef TEMPLATE_CONSTRAINEDCAMERA_HPP
#define TEMPLATE_CONSTRAINEDCAMERA_HPP

#include "HeightMap.hpp"
#include "MovementChecker.hpp"

namespace glimac {

    /**
     * Represent a camera that can walk and fly
     */
    class ConstrainedCamera {
    public :

        /**
         * Constructor of a ConstrainedCamera
         * @param heightData Data of the HeightMap used
         * @param vegetationData Data of the VegetationData used
         * @param heigthOfEntity Height of the Camera (emulate a human)
         */
        ConstrainedCamera(const std::vector<std::vector<float>> &heightData,
                          const std::vector<std::vector<glm::vec3>> &vegetationData, float heigthOfEntity) :
                m_fPhi(M_PI),
                m_fTheta(0),
                m_heightOfEntity(heigthOfEntity),
                m_movementChecker(heightData, vegetationData),
                m_isFlying(false) {
            float y = heightData[0.f][0.f] + 2.f;
            m_Position = glm::vec3(0.f, y + m_heightOfEntity, 0.f);
            computeDirectionVectors();
        }

        /**
         * Constructor of a ConstrainedCamera
         * @param x Position on the x axis
         * @param z Position on the z axis
         * @param heightData Data of the HeightMap used
         * @param vegetationData Data of the VegetationData used
         * @param heightOfEntity Height of the Camera (emulate a human)
         */
        ConstrainedCamera(float x, float z, const std::vector<std::vector<float>> &heightData,
                          const std::vector<std::vector<glm::vec3>> &vegetationData, float heightOfEntity) :
                m_fPhi(M_PI),
                m_fTheta(0),
                m_heightOfEntity(heightOfEntity),
                m_movementChecker(heightData, vegetationData),
                m_isFlying(false) {
            float y = heightData[x][z];
            m_Position = glm::vec3(x, y + m_heightOfEntity, z);
            computeDirectionVectors();
        }

        /**
         * Move left or right depending on the value given
         * @param t Direction to go
         */
        void moveLeft(float t);

        /**
         * Move front or back depending on the value given
         * @param t Direction to go
         */
        void moveFront(float t);

        /**
         * Rotate left or right depending on the value given
         * @param degrees Degree of rotation
         */
        void rotateLeft(float degrees);

        /**
         * Rotate up or down depending on the value given
         * @param degrees Degree of rotation
         */
        void rotateUp(float degrees);

        /**
         * Getter for the ViewMatrix of the Camera
         * @return
         */
        glm::mat4 getViewMatrix() const;

        /**
         * Getter for the position of the Camera
         * @return
         */
        glm::vec3 getPosition() const;

        /**
         * Test whether or not the Camera is flying
         * @return true if flying, false otherwise
         */
        bool iAmFlying() {
            return m_isFlying;
        }

        /**
         * Set Camera to flying
         */
        void startFlying() {
            m_isFlying = true;
        }

        /**
         * Set Camera to not flying
         */
        void stopFlying() {
            m_isFlying = false;
            m_Position = m_movementChecker.leveledFuturePosition(m_Position, m_Position, m_heightOfEntity);
        }

    private :

        /**
         * Compute the direction vectors
         */
        void computeDirectionVectors();

        /**
         * Position of the Camera on the map
         */
        glm::vec3 m_Position;

        /**
         * Angle phi used for rotation left or right
         */
        float m_fPhi;

        /**
         * Angle theta used for rotation up or down
         */
        float m_fTheta;

        /**
         * Front vector
         */
        glm::vec3 m_FrontVector;

        /**
         * Left vector
         */
        glm::vec3 m_LeftVector;

        /**
         * Up vector
         */
        glm::vec3 m_UpVector;

        /**
         * Height of the Camera
         */
        float m_heightOfEntity;

        /**
         * Boolean to know if camera is flying
         */
        bool m_isFlying;

        /**
         * MovementChecker allowing movement
         */
        MovementChecker m_movementChecker;
    };

}

#endif //TEMPLATE_CONSTRAINEDCAMERA_HPP
