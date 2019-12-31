#include <glimac/SDLWindowManager.hpp>
#include <GL/glew.h>
#include <iostream>
#include <glimac/Sphere.hpp>
#include <glimac/Program.hpp>
#include <glimac/FreeflyCamera.hpp>
#include <GLFW/glfw3.h>
#include <glimac/GLFWWindowManager.hpp>
#include <glimac/Cube.hpp>
#include <glimac/TrackballCamera.hpp>


using namespace glimac;

void errorCallback(int, const char* err_str)
{
    std::cout << "GLFW Error: " << err_str << std::endl;
}

int main(int argc, char **argv) {
    // Initialize SDL and open a window
    glfwSetErrorCallback(errorCallback);


    int width = 800;
    int height = 800;
    //SDLWindowManager windowManager(width, height, "GLImac");

    GLFWWindowManager window(width, height, "LaraCraft", windowModes::Windowed);


    std::cout << "Before GLEW initialisation" << std::endl;
    // Initialize glew for OpenGL3+ support
    glewExperimental = GL_TRUE;
    GLenum glewInitError = glewInit();
    if (GLEW_OK != glewInitError) {
        std::cerr << glewGetErrorString(glewInitError) << std::endl;
        return EXIT_FAILURE;
    }


    //SDL_ShowCursor(0);
    //SDL_WM_GrabInput(SDL_GRAB_ON);
    FilePath applicationPath(argv[0]);
    Program program = loadProgram(applicationPath.dirPath() + "shaders/3D.vs.glsl",
                                  applicationPath.dirPath() + "shaders/normals.fs.glsl");
    program.use();

    std::cout << "OpenGL Version : " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLEW Version : " << glewGetString(GLEW_VERSION) << std::endl;

    /*********************************
     * HERE SHOULD COME THE INITIALIZATION CODE
     *********************************/
    int uMVPMatrixId = glGetUniformLocation(program.getGLId(),"uMVPMatrix");
    int uMVMatrixId = glGetUniformLocation(program.getGLId(),"uMVMatrix");
    int uNormalMatrixId =glGetUniformLocation(program.getGLId(),"uNormalMatrix");
    glEnable(GL_DEPTH_TEST);
   // glEnable(GL_CULL_FACE);


    /**
     * VBO
     * Shape init
     */
    Cube cube;

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferData(GL_ARRAY_BUFFER, cube.getVertexCount() * sizeof(ShapeVertex), cube.getDataPointer(),
                 GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);


    /**
     * IBO
     * Index vertices
     */

    /*GLuint ibo;
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

    const uint32_t *indicePointerArray = cube.getIBOIndices();

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, cube.getIndiceCount() * sizeof(uint32_t), indicePointerArray, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);*/

    /**
     * VAO
     * Attribute indexing
     */

    GLuint vao;
    glGenBuffers(1, &vao);
    glBindVertexArray(vao);

    // Declaration of the ibo in the vao
   // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);


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

    FreeflyCamera camera;


    glm::mat4 ProjMatrix = glm::perspective(glm::radians(70.f), (float)width/(float)height,0.1f,100.f);

    double xpos, ypos;
    xpos = ypos = 0;

    // Application loop:
    //bool done = false;
    //bool firstMouseLaunch = true;
    while (!window.windowShouldClose()) {
        // Event loop:
        //SDL_Event e;
        /*float cameraMoveForward = 0.f;
        float cameraMoveLeft = 0.f;
        float cameraRotateLeft = 0.f;
        float cameraRotateUp = 0.f;*/

        /*while (windowManager.pollEvent(e)) {
            switch (e.type) {
                case SDL_QUIT:
                    done = true; // Leave the loop after this iteration
                    break;
            }
        }*/

        /*int x = 0;
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
        }*/

        /*********************************
         * HERE SHOULD COME THE RENDERING CODE
         *********************************/


        /*camera.moveFront(cameraMoveForward / 10);
        camera.moveLeft(cameraMoveLeft / 10);
        camera.rotateLeft(cameraRotateLeft / 10);
        camera.rotateUp(cameraRotateUp / 10);*/

        // Matrix calculations
        glm::mat4 VMatrix = camera.getViewMatrix();
        glm::mat4 MVMatrix = VMatrix;

        glm::mat4 NormalMatrix = glm::transpose(glm::inverse(MVMatrix));

        glm::mat4 MVPMatrix = ProjMatrix * MVMatrix;
        glBindVertexArray(vao);

        glUniformMatrix4fv(uMVMatrixId,1,GL_FALSE,glm::value_ptr(MVMatrix));
        glUniformMatrix4fv(uMVPMatrixId,1,GL_FALSE,glm::value_ptr(MVPMatrix));
        glUniformMatrix4fv(uNormalMatrixId,1,GL_FALSE,glm::value_ptr(NormalMatrix));
        std::cout << cube.getVertexCount()<< std::endl;
        glDrawArrays(GL_TRIANGLES, 0, cube.getVertexCount());

        //Flush VAO
        glBindVertexArray(0);
        // Update the display
        window.swapBuffers();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        window.pollEvent();
        int stateUpKey = window.getKey(GLFW_KEY_UP);
        if(stateUpKey == GLFW_PRESS) {
            camera.moveFront(0.01f);
        }
        int stateDownKey = window.getKey(GLFW_KEY_DOWN);
        if(stateDownKey == GLFW_PRESS) {
            camera.moveFront(-0.01f);
        }
        double tmpxPos, tmpyPos;
        window.getCursorPosition(&tmpxPos, &tmpyPos);
        float xrel = xpos - tmpxPos;
        float yrel = ypos - tmpyPos;
        int stateMouseClick = window.getMouseButton(GLFW_MOUSE_BUTTON_LEFT);
        if(stateMouseClick == GLFW_PRESS) {
            camera.rotateUp(yrel);
            camera.rotateLeft(xrel);
        }
        xpos = tmpxPos;
        ypos = tmpyPos;
    }

    // Memory cleaning
    //glDeleteBuffers(1, &ibo);
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
    return EXIT_SUCCESS;
}
