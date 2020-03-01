//
// Created by adam on 01/01/2020.
//

#ifndef IMACGL_GLOBALPROGRAM_HPP
#define IMACGL_GLOBALPROGRAM_HPP


#include "Cube.hpp"
#include "Program.hpp"
using namespace glimac;

class GlobalProgram {

public:
    Program m_Program;

    GLint uMVPMatrixId;
    GLint uMVMatrixId;
    GLint uNormalMatrixId;

    GLint uKdId;
    GLint uKsId;
    GLint uShininessId;
    GLint uLightDir_vsId;
    GLint uLightIntensityId;

    GLint uTextureId;

    GlobalProgram(const FilePath &applicationPath) :
            m_Program(loadProgram(applicationPath.dirPath() + "shaders/3D.vs.glsl",
                                  applicationPath.dirPath() + "shaders/simpleTexture3DLight.fs.glsl")) {
        uMVPMatrixId = glGetUniformLocation(m_Program.getGLId(), "uMVPMatrix");
        uMVMatrixId = glGetUniformLocation(m_Program.getGLId(), "uMVMatrix");
        uNormalMatrixId = glGetUniformLocation(m_Program.getGLId(), "uNormalMatrix");
        uKdId = glGetUniformLocation(m_Program.getGLId(), "uKd");
        uKsId = glGetUniformLocation(m_Program.getGLId(), "uKs");
        uShininessId = glGetUniformLocation(m_Program.getGLId(), "uShininess");
        uLightDir_vsId = glGetUniformLocation(m_Program.getGLId(), "uLightDir_vs");
        uLightIntensityId = glGetUniformLocation(m_Program.getGLId(), "uLightIntensity");
    }
};



#endif //IMACGL_GLOBALPROGRAM_HPP
