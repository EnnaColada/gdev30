/******************************************************************************
 * This demo draws a triangle by defining its vertices in 3 dimensions
 * (the 3rd dimension is currently ignored and is just set to 0).
 *
 * The drawing is accomplished by:
 * - Uploading the vertices to the GPU using a Vertex Buffer Object (VBO).
 * - Specifying the vertices' format using a Vertex Array Object (VAO).
 * - Using a GLSL shader program (consisting of a simple vertex shader and a
 *   simple fragment shader) to actually draw the vertices as a triangle.
 *
 * Happy hacking! - eric
 *****************************************************************************/

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include <gdev.h>

// change this to your desired window attributes
#define WINDOW_WIDTH  640
#define WINDOW_HEIGHT 360
#define WINDOW_TITLE  "Hello Explosion"
GLFWwindow *pWindow;

// define a vertex array to hold our vertices
float vertices[] =
{
    // position (x, y, z)       color (r, g, b)     texture coordinates (s, t)      piece (u)
    -0.30f, -0.50f, 0.00f,      1.0f, 1.0f, 1.0f,       0.0f, 0.0f,     0.0f,
    0.30f, -0.50f, 0.00f,       1.0f, 1.0f, 1.0f,       1.0f, 0.0f,     0.0f,
    -0.30f, 0.50f, 0.00f,       1.0f, 1.0f, 1.0f,       0.0f, 1.0f,     0.0f,
    
    0.30f, 0.50f, 0.00f,        1.0f, 1.0f, 1.0f,       1.0f, 1.0f,     1.0f,
    0.00f, 0.50f, 0.00f,        1.0f, 1.0f, 1.0f,       0.5f, 1.0f,     1.0f,
    0.30f, -0.50f, 0.00f,       1.0f, 1.0f, 1.0f,       1.0f, 0.0f,     1.0f,

    0.00f, 0.50f, 0.00f,        1.0f, 1.0f, 1.0f,       0.5f, 1.0f,     2.0f,
    -0.30f, 0.50f, 0.00f,       1.0f, 1.0f, 1.0f,       0.0f, 1.0f,     2.0f,
    0.30f, -0.50f, 0.00f,       1.0f, 1.0f, 1.0f,       1.0f, 0.0f,     2.0f
};
    

// define OpenGL object IDs to represent the vertex array and the shader program in the GPU
GLuint vao;         // vertex array object (stores the render state for our vertex array)
GLuint vbo;         // vertex buffer object (reserves GPU memory for our vertex array)
GLuint shader;      // combined vertex and fragment shader
GLuint texture1;
GLuint texture2;

// called by the main function to do initial setup, such as uploading vertex
// arrays, shader programs, etc.; returns true if successful, false otherwise
bool setup()
{
    // generate the VAO and VBO objects and store their IDs in vao and vbo, respectively
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    // bind the newly-created VAO to make it the current one that OpenGL will apply state changes to
    glBindVertexArray(vao);

    // upload our vertex array data to the newly-created VBO
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // on the VAO, register the current VBO with the following vertex attribute layout:
    // - layout location 0...
    // - ... shall consist of 3 GL_FLOATs (corresponding to x, y, and z coordinates)
    // - ... its values will NOT be normalized (GL_FALSE)
    // - ... the stride length is the number of bytes of all 3 floats of each vertex (hence, 3 * sizeof(float))
    // - ... and we start at the beginning of the array (hence, (void*) 0)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*) 0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*) (3 * sizeof(float)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*) (6 * sizeof(float)));
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*) (8 * sizeof(float)));

    // enable the newly-created layout location 0;
    // this shall be used by our vertex shader to read the vertex's x, y, and z
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);

    // important: if you have more vertex arrays to draw, make sure you separately define them
    // with unique VAO and VBO IDs, and follow the same process above to upload them to the GPU

    // load our shader program
    shader = gdevLoadShader("Exercise2.vs", "Exercise2.fs");
    if (! shader)
        return false;

    texture1 = gdevLoadTexture("herobrineface.png", GL_REPEAT, true, true);
    if (! texture1) return false;
    texture2 = gdevLoadTexture("pepe.jpg", GL_REPEAT, true, true);
    if (! texture2) return false;
    // gdevLoadTexture() is defined in gdev.h, and its parameters are:
    // - the texture filename
    // - the wrap mode (GL_REPEAT, GL_MIRRORED_REPEAT, ...)
    // - whether to enable linear filtering (more on this later)
    // - whether to enable mipmapping (also more on this later)

    return true;
}

// called by the main function to do rendering per frame
void render()
{
    // clear the whole frame
    glClearColor(0.0f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    float time = sin(glfwGetTime()) + 0.5;

    // using our shader program...
    glUseProgram(shader);

    //set the active texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);

    // then connect each texture unit to a sampler2D in the fragment shader
    glUniform1i(glGetUniformLocation(shader, "texture1"), 0);
    glUniform1i(glGetUniformLocation(shader, "texture2"), 1);
    glUniform1f(glGetUniformLocation(shader, "time"), time);


    // ... draw our triangles
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (9 * sizeof(float)));
}

/*****************************************************************************/

// handler called by GLFW when there is a keyboard event
void handleKeys(GLFWwindow* pWindow, int key, int scancode, int action, int mode)
{
    // pressing Esc closes the window
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(pWindow, GL_TRUE);
}

// handler called by GLFW when the window is resized
void handleResize(GLFWwindow* pWindow, int width, int height)
{
    // tell OpenGL to do its drawing within the entire "client area" (area within the borders) of the window
    glViewport(0, 0, width, height);
}

// main function
int main(int argc, char** argv)
{
    // initialize GLFW and ask for OpenGL 3.3 core
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // create a GLFW window with the specified width, height, and title
    pWindow = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, NULL, NULL);
    if (! pWindow)
    {
        // gracefully terminate if we cannot create the window
        std::cout << "Cannot create the GLFW window.\n";
        glfwTerminate();
        return -1;
    }

    // make the window the current context of subsequent OpenGL commands,
    // and enable vertical sync and aspect-ratio correction on the GLFW window
    glfwMakeContextCurrent(pWindow);
    glfwSwapInterval(1);
    glfwSetWindowAspectRatio(pWindow, WINDOW_WIDTH, WINDOW_HEIGHT);

    // set up callback functions to handle window system events
    glfwSetKeyCallback(pWindow, handleKeys);
    glfwSetFramebufferSizeCallback(pWindow, handleResize);

    // don't miss any momentary keypresses
    glfwSetInputMode(pWindow, GLFW_STICKY_KEYS, GLFW_TRUE);

    // initialize GLAD, which acts as a library loader for the current OS's native OpenGL library
    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);

    // if our initial setup is successful...
    if (setup())
    {
        // do rendering in a loop until the user closes the window
        while (! glfwWindowShouldClose(pWindow))
        {
            // render our next frame
            // (by default, GLFW uses double-buffering with a front and back buffer;
            // all drawing goes to the back buffer, so the frame does not get shown yet)
            render();

            // swap the GLFW front and back buffers to show the next frame
            glfwSwapBuffers(pWindow);

            // process any window events (such as moving, resizing, keyboard presses, etc.)
            glfwPollEvents();
        }
    }

    // gracefully terminate the program
    glfwTerminate();
    return 0;
}
