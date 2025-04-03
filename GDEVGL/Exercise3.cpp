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
#define WINDOW_TITLE  "That's sus"
GLFWwindow *pWindow;

glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f,  3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);

glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);

glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f); 
glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));

float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

float lastX = 320, lastY = 180;
float yaw = -90.0f, pitch = 0.0f;
float fov = 45.0f;
bool firstMouse = true;

// define a vertex array to hold our vertices
float vertices[] =
{
    // position (x, y, z)       color (r, g, b)
    // -0.50f, -0.50f, -1.00f,     1.0f, 1.0f, 1.0f,
    //  0.50f, -0.50f, -1.00f,     1.0f, 1.0f, 1.0f,
    // -0.50f,  0.50f, -1.00f,     1.0f, 1.0f, 1.0f,
    //  0.50f,  0.50f, -1.00f,     1.0f, 1.0f, 1.0f,
    // -0.50f,  0.50f, -1.00f,     1.0f, 1.0f, 1.0f,
    //  0.50f, -0.50f, -1.00f,     1.0f, 1.0f, 1.0f

    // -0.125f, 0.000f, 0.000f,    1.0f, 1.0f, 1.0f,
    // -0.250f, 0.217f, 0.000f,    1.0f, 1.0f, 1.0f,
    // -0.125f, 0.424f, 0.000f,    1.0f, 1.0f, 1.0f,
    //  0.125f, 0.000f, 0.000f,    1.0f, 1.0f, 1.0f,
    //  0.250f, 0.217f, 0.000f,    1.0f, 1.0f, 1.0f,
    //  0.125f, 0.424f, 0.000f,    1.0f, 1.0f, 1.0f,

    //leg
/*0*/   -0.125f,-0.500f, 0.000f,    1.0f, 1.0f, 1.0f,
/*1*/   -0.250f,-0.500f, 0.217f,    1.0f, 1.0f, 1.0f,
/*2*/   -0.125f,-0.500f, 0.424f,    1.0f, 1.0f, 1.0f,
/*3*/    0.125f,-0.500f, 0.000f,    1.0f, 1.0f, 1.0f,
/*4*/    0.250f,-0.500f, 0.217f,    1.0f, 1.0f, 1.0f,
/*5*/    0.125f,-0.500f, 0.424f,    1.0f, 1.0f, 1.0f,

/*6*/   -0.125f,-0.200f, 0.000f,    1.0f, 1.0f, 1.0f,
/*7*/   -0.250f,-0.200f, 0.217f,    1.0f, 1.0f, 1.0f,
/*8*/   -0.125f,-0.200f, 0.424f,    1.0f, 1.0f, 1.0f,
/*9*/    0.125f,-0.200f, 0.000f,    1.0f, 1.0f, 1.0f,
/*10*/   0.250f,-0.200f, 0.217f,    1.0f, 1.0f, 1.0f,
/*11*/   0.125f,-0.200f, 0.424f,    1.0f, 1.0f, 1.0f,




};

GLuint indices[] = {
    0, 1, 2, 
    0, 2, 3,
    2, 3, 5,
    3, 4, 5,
    0, 6, 9,
    0, 5, 9




};

// define OpenGL object IDs to represent the vertex array and the shader program in the GPU
GLuint vao;         // vertex array object (stores the render state for our vertex array)
GLuint vbo;         // vertex buffer object (reserves GPU memory for our vertex array)
GLuint ebo;        // vertex buffer object (reserves GPU memory for our vertex array)
GLuint shader;      // combined vertex and fragment shader

// called by the main function to do initial setup, such as uploading vertex
// arrays, shader programs, etc.; returns true if successful, false otherwise
bool setup()
{
    // generate the VAO and VBO objects and store their IDs in vao and vbo, respectively
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    // bind the newly-created VAO to make it the current one that OpenGL will apply state changes to
    glBindVertexArray(vao);

    // upload our vertex array data to the newly-created VBO
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //added ebo for uploading index array data to the newly create EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // on the VAO, register the current VBO with the following vertex attribute layout:
    // - layout location 0...
    // - ... shall consist of 3 GL_FLOATs (corresponding to x, y, and z coordinates)
    // - ... its values will NOT be normalized (GL_FALSE)
    // - ... the stride length is the number of bytes of all 3 floats of each vertex (hence, 3 * sizeof(float))
    // - ... and we start at the beginning of the array (hence, (void*) 0)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) 0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) (3 * sizeof(float)));

    // enable the newly-created layout location 0;
    // this shall be used by our vertex shader to read the vertex's x, y, and z
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    // important: if you have more vertex arrays to draw, make sure you separately define them
    // with unique VAO and VBO IDs, and follow the same process above to upload them to the GPU

    // load our shader program
    shader = gdevLoadShader("Exercise3.vs", "Exercise3.fs");
    if (! shader)
        return false;

    return true;
}

// called by the main function to do rendering per frame
void render()
{
    //camera
    glm::mat4 view;
    view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

    // clear the whole frame
    glClearColor(0.0f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // using our shader program...
    glUseProgram(shader);
    glEnable(GL_DEPTH_TEST); // enable OpenGL's hidden surface removal
    
    glm::mat4 matrix;
    matrix = glm::perspective(glm::radians(60.0f),
                             (float) WINDOW_WIDTH / WINDOW_HEIGHT,
                              0.1f, 100.0f);

    matrix *= view;

    glUniformMatrix4fv(glGetUniformLocation(shader, "matrix"),
                       1, GL_FALSE, glm::value_ptr(matrix));

    // ... draw our triangles
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(GLuint), GL_UNSIGNED_INT, 0);

    processInput(pWindow);
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
    glfwSetCursorPosCallback(pWindow, mouse_callback); 

    // don't miss any momentary keypresses
    glfwSetInputMode(pWindow, GLFW_STICKY_KEYS, GLFW_TRUE);
    glfwSetInputMode(pWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED); 

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

// moving camera
void processInput(GLFWwindow *window)
{
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;  
    const float cameraSpeed = 2.5f * deltaTime; // adjust accordingly
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates range from bottom to top
    lastX = xpos;
    lastY = ypos;

    const float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw   += xoffset;
    pitch += yoffset;

    if(pitch > 89.0f)
        pitch = 89.0f;
    if(pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(direction);
};