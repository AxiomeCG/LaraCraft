//
// Created by adam on 08/02/2020.
//


#include <glimac/Pnj.hpp>
#include <GLFW/glfw3.h>


mat4 Pnj::getModelMatrix() const {
    mat4 T = translate(mat4(),  position);
    mat4 S = scale(mat4(),  vec3(0.5f,0.5f,0.5f));
    mat4 R = rotate(mat4(),  (float)atan2(direction.z,direction.x), vec3(0.f,1.f,0.f));

    return T*R*S;
}

void Pnj::updatePosition() {


    auto tmpPosition = position + direction;
    bool willMove = movementChecker.willMoveToAnotherCube(position, tmpPosition);
    bool canMove = movementChecker.canMoveToTheOtherCube(position, tmpPosition);

    timeout -= glfwGetTime();
    std::cout << timeout << std::endl;
    if(timeout <= 0 || !canMove){
        generateNewRandomDirection();
        timeout = 5000.0f;
        tmpPosition = position + direction;
        willMove = movementChecker.willMoveToAnotherCube(position, tmpPosition);
        canMove = movementChecker.canMoveToTheOtherCube(position, tmpPosition);
    }
    std::cout << "P:" << std::endl;
    std::cout << position << std::endl;
    std::cout << position + direction << std::endl;
    if(willMove) {
        if (canMove) {
            position = movementChecker.leveledFuturePosition(tmpPosition, 1.);
        }
    }
    else {
        position = movementChecker.leveledFuturePosition(tmpPosition, 1.);
    }
}

void Pnj::generateNewRandomDirection() {
    float randomX = static_cast <float> (random()) / (static_cast <float> (RAND_MAX)) ;
    float randomZ = static_cast <float> (random()) / (static_cast <float> (RAND_MAX)) ;

    float x = randomX < 0.333f ? -1.f : (randomX < 0.666f ? 0.f : 1.f);
    float z = randomZ < 0.333f ? -1.f : (randomZ < 0.666f ? 0.f : 1.f);
    direction = vec3(x * 0.05f, 0, z * 0.05f);
}
