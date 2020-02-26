//
// Created by Matthias Hudelot on 08/02/2020.
//

#ifndef TEMPLATE_SIMPLETEXTUREDSKYBOXPROGRAM_HPP
#define TEMPLATE_SIMPLETEXTUREDSKYBOXPROGRAM_HPP

namespace glimac {

    /**
     * Represent a program for the Skybox
     * Use the right shaders
     */
    class SimpleTexturedSkyboxProgram {

    public:
        /**
         * Program to load the shaders
         */
        Program m_Program;

        /**
         * Model View Projection Matrix uniform
         */
        GLint uMVPMatrixId;

        /**
         * Model View Matrix uniform
         */
        GLint uMVMatrixId;

        /**
         * Normal Matrix uniform
         */
        GLint uNormalMatrixId;

        /**
         * Texture uniform
         */
        GLint uTextureId;

        /**
         * Constructor for the SimpleTexturedSkyboxProgram
         * @param applicationPath Path of the application
         */
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
