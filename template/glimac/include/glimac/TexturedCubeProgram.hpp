//
// Created by adam on 31/12/2019.
//

#ifndef IMACGL_TEXTUREDCUBEPROGRAM_HPP
#define IMACGL_TEXTUREDCUBEPROGRAM_HPP


#include "Cube.hpp"
#include "Program.hpp"
using namespace glimac;
class TexturedCubeProgram {

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

    GLint uTextureSideId;
    GLint uTextureTopId;
    GLint uTextureBottomId;

    TexturedCubeProgram(const FilePath &applicationPath) :
            m_Program(loadProgram(applicationPath.dirPath() + "shaders/3D.vs.glsl",
                                  applicationPath.dirPath() + "shaders/directionallight.fs.glsl")) {
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


#endif //IMACGL_TEXTUREDCUBEPROGRAM_HPP
