#pragma once


namespace glimac {
    class TrackBallCamera {
    private:
        float m_fDistance;
        float m_fAngleX;
        float m_fAngleY;
    public:
        TrackBallCamera() : m_fDistance(5), m_fAngleX(0), m_fAngleY(0) {

        }

        void moveFront(float delta) {
            m_fDistance -= delta;
        }

        void rotateLeft(float degrees) {
            m_fAngleY += glm::radians(degrees);
        }

        void rotateUp(float degrees) {
            m_fAngleX += glm::radians(degrees);
        }

        glm::mat4 getViewMatrix() const {
            glm::mat4 viewMatrix = glm::mat4(1.f);
            viewMatrix =  glm::translate(viewMatrix, glm::vec3(0,0,-m_fDistance));
            viewMatrix =  glm::rotate(viewMatrix,m_fAngleX, glm::vec3(1,0,0));
            viewMatrix =  glm::rotate(viewMatrix,m_fAngleY, glm::vec3(0,1,0));
            return viewMatrix;
        }
    };

}

