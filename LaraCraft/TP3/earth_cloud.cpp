#include <glimac/SDLWindowManager.hpp>
#include <GL/glew.h>
#include <iostream>
#include <glimac/Sphere.hpp>
#include <glimac/Program.hpp>
#include <glimac/Image.hpp>

using namespace glimac;

int main(int argc, char **argv) {
    // Initialize SDL and open a window

    int width = 800;
    int height = 800;
    SDLWindowManager windowManager(width, height, "GLImac");


    // Initialize glew for OpenGL3+ support
    GLenum glewInitError = glewInit();
    if (GLEW_OK != glewInitError) {
        std::cerr << glewGetErrorString(glewInitError) << std::endl;
        return EXIT_FAILURE;
    }

    FilePath applicationPath(argv[0]);
    Program program = loadProgram(applicationPath.dirPath() + "shaders/3D.vs.glsl",
                                  applicationPath.dirPath() + "shaders/multiTex3D.fs.glsl");
    program.use();


    /***************
     * TEXTURE INIT
     ***************/

    auto earthImagePtr = loadImage("/home/adam/Bureau/upem/M2/s1/synthese/tp1/GLImac-Template/TP3/assets/textures/EarthMap.jpg");
    auto moonImagePtr = loadImage("/home/adam/Bureau/upem/M2/s1/synthese/tp1/GLImac-Template/TP3/assets/textures/MoonMap.jpg");
    auto cloudImagePtr = loadImage("/home/adam/Bureau/upem/M2/s1/synthese/tp1/GLImac-Template/TP3/assets/textures/CloudMap.jpg");
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
    /*************
     * UNIFORM
     ************/
    int uMVPMatrixId = glGetUniformLocation(program.getGLId(), "uMVPMatrix");
    int uMVMatrixId = glGetUniformLocation(program.getGLId(), "uMVMatrix");
    int uNormalMatrixId = glGetUniformLocation(program.getGLId(), "uNormalMatrix");
    int uTextureId = glGetUniformLocation(program.getGLId(), "uTexture");
    int uTextureSecondId = glGetUniformLocation(program.getGLId(), "uTextureSecond");

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

    glBindBuffer(GL_ARRAY_BUFFER,0);

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



    // Application loop:
    bool done = false;
    while (!done) {
        // Event loop:
        SDL_Event e;
        while (windowManager.pollEvent(e)) {
            if (e.type == SDL_QUIT) {
                done = true; // Leave the loop after this iteration
            }
        }

        /*********************************
         * HERE SHOULD COME THE RENDERING CODE
         *********************************/

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glBindVertexArray(vao);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, earthTextureLocation);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, cloudTextureLocation);
        glUniform1i(uTextureId, 0);
        glUniform1i(uTextureSecondId, 1);

        glm::mat4 MVMatrix = glm::translate(glm::mat4(1.f), glm::vec3(0, 0, -5));
        MVMatrix = glm::rotate(MVMatrix, (float)glm::radians(23.), glm::vec3(0, 0, -1));
        MVMatrix = glm::rotate(MVMatrix, windowManager.getTime(), glm::vec3(0, 1, 0));

        glm::mat4 NormalMatrix = glm::transpose(glm::inverse(MVMatrix));

        glm::mat4 MVPMatrix = ProjMatrix * MVMatrix;

        glUniformMatrix4fv(uMVMatrixId, 1, GL_FALSE, glm::value_ptr(MVMatrix));
        glUniformMatrix4fv(uMVPMatrixId, 1, GL_FALSE, glm::value_ptr(MVPMatrix));
        glUniformMatrix4fv(uNormalMatrixId, 1, GL_FALSE, glm::value_ptr(NormalMatrix));
        glDrawArrays(GL_TRIANGLES, 0, sphere.getVertexCount());

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, 0);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, 0);


        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, moonTextureLocation);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, 0);
        glUniform1i(uTextureId, 0);
        glUniform1i(uTextureSecondId, 1);

        for (int i = 0; i < 32; ++i) { //Moons

            glm::mat4 MVMatrixMoon = glm::translate(glm::mat4(1.f), glm::vec3(0, 0, -5));

            MVMatrixMoon = glm::rotate(MVMatrixMoon, windowManager.getTime(),
                                       moonRandomRotation[i]); // Translation * Rotation
            MVMatrixMoon = glm::translate(MVMatrixMoon,
                                          -2.f * moonRandomTranslation[i]); // Translation * Rotation * Translation
            MVMatrixMoon = glm::scale(MVMatrixMoon,
                                      glm::vec3(0.2, 0.2, 0.2)); // Translation * Rotation * Translation * Scale

            glm::mat4 NormalMatrixMoon = glm::transpose(glm::inverse(MVMatrixMoon));
            glm::mat4 MVPMatrixMoon = ProjMatrix * MVMatrixMoon;


            glUniformMatrix4fv(uMVMatrixId, 1, GL_FALSE, glm::value_ptr(MVMatrixMoon));
            glUniformMatrix4fv(uMVPMatrixId, 1, GL_FALSE, glm::value_ptr(MVPMatrixMoon));
            glUniformMatrix4fv(uNormalMatrixId, 1, GL_FALSE, glm::value_ptr(NormalMatrixMoon));
            glDrawArrays(GL_TRIANGLES, 0, sphere.getVertexCount());
        }
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, 0);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, 0);



        glBindVertexArray(0);
        // Update the display
        windowManager.swapBuffers();
    }

    glDeleteTextures(1, &earthTextureLocation);
    glDeleteTextures(1, &moonTextureLocation);
    glDeleteTextures(1, &cloudTextureLocation);
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
    return EXIT_SUCCESS;
}
