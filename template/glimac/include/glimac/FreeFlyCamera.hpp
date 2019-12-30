#pragma once


using namespace glm;
namespace glimac {
    class FreeFlyCamera {
    private:
        vec3 m_Position;
        float m_fPhi;
        float m_fTheta;

        vec3 m_FrontVector;
        vec3 m_LeftVector;
        vec3 m_UpVector;

        void computeDirectionVectors() {
            m_FrontVector = vec3(
                    cos(m_fTheta) * sin(m_fPhi),
                    sin(m_fTheta),
                    cos(m_fTheta) * cos(m_fPhi));
            m_LeftVector = vec3(
                    sin(m_fPhi + M_PI_2),
                    0,
                    cos(m_fPhi + M_PI_2));
            m_UpVector = cross(m_FrontVector, m_LeftVector);

        }

    public:
        FreeFlyCamera() : m_Position(vec3(0, 0, 5)), m_fPhi(M_PI), m_fTheta(0) {
            computeDirectionVectors();
        }

        void moveLeft(float t) {
            m_Position += t * m_LeftVector;
        }

        void moveFront(float t) {
            m_Position += t * m_FrontVector;
        }

        void rotateLeft(float degrees) {
            m_fPhi += radians(degrees);
            computeDirectionVectors();
        }

        void rotateUp(float degrees) {
            m_fTheta += radians(degrees);
            computeDirectionVectors();
        }

        glm::mat4 getViewMatrix() const {
            return glm::lookAt(m_Position, m_Position + m_FrontVector, m_UpVector);
        }
    };

}

