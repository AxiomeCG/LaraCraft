#include <glimac/SDLWindowManager.hpp>
#include <GL/glew.h>
#include <iostream>
#include <glimac/Sphere.hpp>
#include <glimac/Program.hpp>
#include <glimac/Image.hpp>
#include <glimac/FreeFlyCamera.hpp>

using namespace glimac;

struct EarthProgram {
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
    GLint uTextureIdSecond;


    EarthProgram(const FilePath &applicationPath) :
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

struct MoonProgram {
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
    GLint uTextureIdSecond;

    MoonProgram(const FilePath &applicationPath) :
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

int main(int argc, char **argv) {
    // Initialize SDL and open a window

    int width = 800;
    int height = 800;
    SDLWindowManager windowManager(width, height, "GLImac");

    SDL_ShowCursor(0);
    SDL_WM_GrabInput(SDL_GRAB_ON);
    // Initialize glew for OpenGL3+ support
    GLenum glewInitError = glewInit();
    if (GLEW_OK != glewInitError) {
        std::cerr << glewGetErrorString(glewInitError) << std::endl;
        return EXIT_FAILURE;
    }

    FilePath applicationPath(argv[0]);
    EarthProgram earthProgram(applicationPath);
    MoonProgram moonProgram(applicationPath);

    /***************
     * TEXTURE INIT
     ***************/

    auto earthImagePtr = loadImage(
            "/home/adam/Bureau/upem/M2/s1/synthese/tp1/GLImac-Template/TP3/assets/textures/EarthMap.jpg");
    auto moonImagePtr = loadImage(
            "/home/adam/Bureau/upem/M2/s1/synthese/tp1/GLImac-Template/TP3/assets/textures/MoonMap.jpg");
    auto cloudImagePtr = loadImage(
            "/home/adam/Bureau/upem/M2/s1/synthese/tp1/GLImac-Template/TP3/assets/textures/CloudMap.jpg");
    assert(earthImagePtr != nullptr);
    assert(moonImagePtr != nullptr);
    assert(cloudImagePtr != nullptr);

    GLuint earthTextureLocation;
    GLuint moonTextureLocation;
    GLuint cloudTextureLocation;
    glGenTextures(1, &earthTextureLocation);
    glGenTextures(1, &moonTextureLocation);
    glGenTextures(1, &cloudTextureLocation);

    /**
     * Earth texture
     */
    glBindTexture(GL_TEXTURE_2D, earthTextureLocation);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, earthImagePtr->getWidth(), earthImagePtr->getHeight(), 0, GL_RGBA, GL_FLOAT,
                 earthImagePtr->getPixels());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, earthTextureLocation);

    /**
     * Moon texture
     */
    glBindTexture(GL_TEXTURE_2D, moonTextureLocation);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, moonImagePtr->getWidth(), moonImagePtr->getHeight(), 0, GL_RGBA, GL_FLOAT,
                 moonImagePtr->getPixels());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, moonTextureLocation);

    /**
     * Cloud texture
     */
    glBindTexture(GL_TEXTURE_2D, cloudTextureLocation);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, cloudImagePtr->getWidth(), cloudImagePtr->getHeight(), 0, GL_RGBA, GL_FLOAT,
                 cloudImagePtr->getPixels());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, cloudTextureLocation);


    glEnable(GL_DEPTH_TEST);


    std::cout << "OpenGL Version : " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLEW Version : " << glewGetString(GLEW_VERSION) << std::endl;

    /*********************************
     * HERE SHOULD COME THE INITIALIZATION CODE
     *********************************/

    /**
     * VBO
     * Shape init
     */
    Sphere sphere = Sphere(1., 32, 16);

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferData(GL_ARRAY_BUFFER, sphere.getVertexCount() * sizeof(ShapeVertex), sphere.getDataPointer(),
                 GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    /**
     * VAO
     * Attribute indexing
     */

    GLuint vao;
    glGenBuffers(1, &vao);
    glBindVertexArray(vao);


    const GLuint VERTEX_ATTR_POSITION = 0;
    const GLuint VERTEX_ATTR_NORMAL = 1;
    const GLuint VERTEX_ATTR_TEXTURE = 2;

    glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
    glEnableVertexAttribArray(VERTEX_ATTR_NORMAL);
    glEnableVertexAttribArray(VERTEX_ATTR_TEXTURE);

    /**
     * VBO specification attributes
     * Attribution of the index to attributes
     */

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(VERTEX_ATTR_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(ShapeVertex),
                          (const GLvoid *) offsetof(ShapeVertex, position));
    glVertexAttribPointer(VERTEX_ATTR_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(ShapeVertex),
                          (const GLvoid *) offsetof(ShapeVertex, normal));
    glVertexAttribPointer(VERTEX_ATTR_TEXTURE, 2, GL_FLOAT, GL_FALSE, sizeof(ShapeVertex),
                          (const GLvoid *) offsetof(ShapeVertex, texCoords));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


    glm::mat4 ProjMatrix = glm::perspective(glm::radians(70.f), (float) width / (float) height, 0.1f, 100.f);


    //Moons generation of position and rotation random
    std::vector<glm::vec3> moonRandomRotation;
    std::vector<glm::vec3> moonRandomTranslation;
    for (int i = 0; i < 32; i++) {
        moonRandomRotation.emplace_back(glm::sphericalRand(1.));
        moonRandomTranslation.emplace_back(glm::sphericalRand(1.));
    }

    FreeFlyCamera camera;

    // Application loop:
    bool done = false;
    bool firstMouseLaunch = true;

    while (!done) {
        // Event loop:
        SDL_Event e;
        float cameraMoveForward = 0.f;
        float cameraMoveLeft = 0.f;
        float cameraRotateLeft = 0.f;
        float cameraRotateUp = 0.f;

        while (windowManager.pollEvent(e)) {
            switch (e.type) {
                case SDL_QUIT:
                    done = true; // Leave the loop after this iteration
                    break;
            }
        }

        int x = 0;
        int y = 0;

        if (firstMouseLaunch){
            firstMouseLaunch = false;
            SDL_WarpMouse(0,0);
        } else {
            SDL_GetRelativeMouseState(&x, &y);
        }

        cameraRotateLeft -= (float) x;
        cameraRotateUp -= (float) y;

        auto keyboardState = SDL_GetKeyState(nullptr);

        if (keyboardState[SDLK_z]) {
            cameraMoveForward++;
        }
        if (keyboardState[SDLK_q]) {
            cameraMoveLeft++;
        }
        if (keyboardState[SDLK_s]) {
            cameraMoveForward--;
        }
        if (keyboardState[SDLK_d]) {
            cameraMoveLeft--;
        }
        if (keyboardState[SDLK_ESCAPE]) {
            done = true;
        }


        /*********************************
         * HERE SHOULD COME THE RENDERING CODE
         *********************************/

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glBindVertexArray(vao);

        /************
         * EARTH CODE
         ************/

        earthProgram.m_Program.use();

        camera.moveFront(cameraMoveForward / 10);
        camera.moveLeft(cameraMoveLeft / 10);
        camera.rotateLeft(cameraRotateLeft / 10);
        camera.rotateUp(cameraRotateUp / 10);

        // Matrix calculations
        glm::mat4 VMatrix = camera.getViewMatrix();
        glm::mat4 MVMatrix = VMatrix;


        glm::mat4 lightMVMatrix = glm::rotate(camera.getViewMatrix(), windowManager.getTime(), glm::vec3(0, 1, 0));

        MVMatrix = glm::rotate(MVMatrix, (float) glm::radians(23.), glm::vec3(0., 0., -1.));
        MVMatrix = glm::rotate(MVMatrix, windowManager.getTime(), glm::vec3(0., 1., 0.));
        glm::mat4 NormalMatrix = glm::transpose(glm::inverse(MVMatrix));
        glm::mat4 MVPMatrix = ProjMatrix * MVMatrix;

        // Uniform value attributions
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, earthTextureLocation);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, cloudTextureLocation);
        glUniform1i(earthProgram.uTextureId, 0);


        glUniformMatrix4fv(earthProgram.uMVMatrixId, 1, GL_FALSE, glm::value_ptr(MVMatrix));
        glUniformMatrix4fv(earthProgram.uMVPMatrixId, 1, GL_FALSE, glm::value_ptr(MVPMatrix));
        glUniformMatrix4fv(earthProgram.uNormalMatrixId, 1, GL_FALSE, glm::value_ptr(NormalMatrix));

        glUniform3fv(earthProgram.uKdId, 1, glm::value_ptr(glm::vec3(1., 1., 1.)));
        glUniform3fv(earthProgram.uKsId, 1, glm::value_ptr(glm::vec3(1., 1., 1.)));
        glUniform1f(earthProgram.uShininessId, 10.);

        glUniform3fv(earthProgram.uLightDir_vsId, 1, glm::value_ptr(glm::vec3(glm::vec4(1., 1., 1., 0.) * lightMVMatrix)));
        glUniform3fv(earthProgram.uLightIntensityId, 1, glm::value_ptr(glm::vec3(1., 1., 1.)));

        glDrawArrays(GL_TRIANGLES, 0, sphere.getVertexCount());

        // Flush textures
         glActiveTexture(GL_TEXTURE0);
         glBindTexture(GL_TEXTURE_2D, 0);
         glActiveTexture(GL_TEXTURE1);
         glBindTexture(GL_TEXTURE_2D, 0);


        /************
         * MOON CODE
         ************/
        moonProgram.m_Program.use();

        //Texture attribution to uniform variable
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, moonTextureLocation);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, 0);
        glUniform1i(moonProgram.uTextureId, 0);


        for (int i = 0; i < 32; ++i) { //Moons loop

            // Matrix generation
            glm::mat4 MVMatrixMoon = VMatrix;

            MVMatrixMoon = glm::rotate(MVMatrixMoon, windowManager.getTime(),
                                       moonRandomRotation[i]); // Translation * Rotation
            MVMatrixMoon = glm::translate(MVMatrixMoon,
                                          -2.f * moonRandomTranslation[i]); // Translation * Rotation * Translation
            MVMatrixMoon = glm::scale(MVMatrixMoon,
                                      glm::vec3(0.2, 0.2, 0.2)); // Translation * Rotation * Translation * Scale


            glm::mat4 NormalMatrixMoon = glm::transpose(glm::inverse(MVMatrixMoon));
            glm::mat4 MVPMatrixMoon = ProjMatrix * MVMatrixMoon;

            // Attribution to uniform variables
            glUniformMatrix4fv(moonProgram.uMVMatrixId, 1, GL_FALSE, glm::value_ptr(MVMatrixMoon));
            glUniformMatrix4fv(moonProgram.uMVPMatrixId, 1, GL_FALSE, glm::value_ptr(MVPMatrixMoon));
            glUniformMatrix4fv(moonProgram.uNormalMatrixId, 1, GL_FALSE, glm::value_ptr(NormalMatrixMoon));

            glUniform3fv(moonProgram.uKdId, 1, glm::value_ptr(glm::vec3(1., 1., 1.)));
            glUniform3fv(moonProgram.uKsId, 1, glm::value_ptr(glm::vec3(0.5, 0.5, 0.5)));
            glUniform1f(moonProgram.uShininessId, 64.);
            glUniform3fv(moonProgram.uLightDir_vsId, 1, glm::value_ptr(glm::vec3(glm::vec4(1., 1., 1., 0.) * lightMVMatrix)));
            glUniform3fv(moonProgram.uLightIntensityId, 1, glm::value_ptr(glm::vec3(1., 1., 1.)));


            glDrawArrays(GL_TRIANGLES, 0, sphere.getVertexCount());
        }
        //Flush texture
        /*glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, 0);*/


        //Flush VAO
        glBindVertexArray(0);
        // Update the display
        windowManager.swapBuffers();
    }

    // Memory cleaning
    /*glDeleteTextures(1, &earthTextureLocation);
    glDeleteTextures(1, &moonTextureLocation);
    glDeleteTextures(1, &cloudTextureLocation);*/
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
    return EXIT_SUCCESS;
}
