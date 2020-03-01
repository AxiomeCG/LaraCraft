//
// Created by Matthias Hudelot on 04/02/2020.
//

#include "glimac/ConstrainedCamera.hpp"
#include "glimac/common.hpp"
#include <cmath>

namespace glimac {

void ConstrainedCamera::computeDirectionVectors() {
  m_FrontVector = glm::vec3(cos(m_fTheta) * sin(m_fPhi), sin(m_fTheta),
                            cos(m_fTheta) * cos(m_fPhi));
  m_LeftVector = glm::vec3(sin(m_fPhi + M_PI_2), 0, cos(m_fPhi + M_PI_2));
  m_UpVector = glm::cross(m_FrontVector, m_LeftVector);
}

void ConstrainedCamera::moveLeft(float t) {
  auto tmpPosition = m_Position + (t * m_LeftVector);
  if (m_isFlying) {
    if (m_movementChecker.isWithinBoundaries(tmpPosition)) {
      m_Position = tmpPosition;
    }
  } else {
    m_Position = m_movementChecker.leveledFuturePosition(
        m_Position, tmpPosition, m_heightOfEntity);
  }
}

void ConstrainedCamera::moveFront(float t) {
  auto tmpPosition = m_Position + (t * m_FrontVector);
  if (m_isFlying) {
    if (m_movementChecker.isWithinBoundaries(tmpPosition)) {
      m_Position = tmpPosition;
    }
  } else {
    m_Position = m_movementChecker.leveledFuturePosition(
        m_Position, tmpPosition, m_heightOfEntity);
  }
}

void ConstrainedCamera::rotateLeft(float degrees) {
  m_fPhi += glm::radians(degrees);
  computeDirectionVectors();
}

void ConstrainedCamera::rotateUp(float degrees) {
  float tmpTheta = m_fTheta + glm::radians(degrees);
  if (tmpTheta < (M_PI / 2) && tmpTheta > (-(M_PI / 2))) {
    m_fTheta = tmpTheta;
    computeDirectionVectors();
  }
}

glm::mat4 ConstrainedCamera::getViewMatrix() const {
  return glm::lookAt(m_Position, m_Position + m_FrontVector, m_UpVector);
}

glm::vec3 ConstrainedCamera::getPosition() const { return m_Position; }

} // namespace glimac