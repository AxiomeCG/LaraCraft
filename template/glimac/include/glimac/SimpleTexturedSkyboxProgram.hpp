//
// Created by Matthias Hudelot on 08/02/2020.
//

#ifndef TEMPLATE_SIMPLETEXTUREDSKYBOXPROGRAM_HPP
#define TEMPLATE_SIMPLETEXTUREDSKYBOXPROGRAM_HPP

namespace glimac {

class SimpleTexturedSkyboxProgram {

public:
    Program m_Program;

    GLint uMVPMatrixId;
    GLint uMVMatrixId;
    GLint uNormalMatrixId;

    GLint uTextureId;

    SimpleTexturedSkyboxProgram(const FilePath &applicationPath) :
            m_Program(loadProgram(applicationPath.dirPath() + "shaders/3D.vs.glsl",
                                  applicationPath.dirPath() + "shaders/tex3D.fs.glsl")) {
        uMVPMatrixId = glGetUniformLocation(m_Program.getGLId(), "uMVPMatrix");
        uMVMatrixId = glGetUniformLocation(m_Program.getGLId(), "uMVMatrix");
        uNormalMatrixId = glGetUniformLocation(m_Program.getGLId(), "uNormalMatrix");
        uTextureId = glGetUniformLocation(m_Program.getGLId(), "uTexture");
    }
};
}

#endif //TEMPLATE_SIMPLETEXTUREDSKYBOXPROGRAM_HPP
