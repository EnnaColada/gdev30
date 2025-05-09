/******************************************************************************
*   Controls: 
*     W - up
*     A - left
*     S - down
*     D - right
*     mouse - look around
******************************************************************************/

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include <gdev.h>

// change this to your desired window attributes
#define WINDOW_WIDTH  640
#define WINDOW_HEIGHT 360
#define WINDOW_TITLE  "That's Sus"
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
    // position (x, y, z)                   color (r, g, b)                     piece (s,t)
    //base feet
/*0*/   -0.125f-0.400f,-1.000f, 0.000f,         0.702f, 0.129f, 0.075f,         0.0f,
/*1*/   -0.250f-0.400f,-1.000f, 0.217f,         0.702f, 0.129f, 0.075f,         0.0f,
/*2*/   -0.125f-0.400f,-1.000f, 0.424f,         0.702f, 0.129f, 0.075f,         0.0f,
/*3*/    0.125f-0.400f,-1.000f, 0.000f,         0.702f, 0.129f, 0.075f,         0.0f,
/*4*/    0.250f-0.400f,-1.000f, 0.217f,         0.702f, 0.129f, 0.075f,         0.0f,
/*5*/    0.125f-0.400f,-1.000f, 0.424f,         0.702f, 0.129f, 0.075f,         0.0f,

/*6*/   -0.125f+0.400f,-1.000f, 0.000f,         0.702f, 0.129f, 0.075f,         0.0f,
/*7*/   -0.250f+0.400f,-1.000f, 0.217f,         0.702f, 0.129f, 0.075f,         0.0f,
/*8*/   -0.125f+0.400f,-1.000f, 0.424f,         0.702f, 0.129f, 0.075f,         0.0f,
/*9*/    0.125f+0.400f,-1.000f, 0.000f,         0.702f, 0.129f, 0.075f,         0.0f,
/*10*/   0.250f+0.400f,-1.000f, 0.217f,         0.702f, 0.129f, 0.075f,         0.0f,
/*11*/   0.125f+0.400f,-1.000f, 0.424f,         0.702f, 0.129f, 0.075f,         0.0f,

    //base torso
/*12*/  -0.125f-0.500f,-0.600f, 0.000f-0.03f,   0.702f, 0.129f, 0.075f,         0.0f,
/*13*/  -0.250f-0.500f,-0.600f, 0.217f,         0.702f, 0.129f, 0.075f,         0.0f,
/*14*/  -0.125f-0.500f,-0.600f, 0.424f+0.03f,   0.702f, 0.129f, 0.075f,         0.0f,
/*15*/   0.125f+0.500f,-0.600f, 0.000f-0.03f,   0.702f, 0.129f, 0.075f,         0.0f,
/*16*/   0.250f+0.500f,-0.600f, 0.217f,         0.702f, 0.129f, 0.075f,         0.0f,
/*17*/   0.125f+0.500f,-0.600f, 0.424f+0.03f,   0.702f, 0.129f, 0.075f,         0.0f,

/*18*/   0.125f-0.350f,-0.600f, 0.000f-0.03f,   0.561f, 0.086f, 0.000f,         0.0f,
/*19*/   0.250f-0.350f,-0.600f, 0.217f,         0.561f, 0.086f, 0.000f,         0.0f,
/*20*/   0.125f-0.350f,-0.600f, 0.424f+0.03f,   0.561f, 0.086f, 0.000f,         0.0f,
/*21*/  -0.125f+0.350f,-0.600f, 0.000f-0.03f,   0.561f, 0.086f, 0.000f,         0.0f,
/*22*/  -0.250f+0.350f,-0.600f, 0.217f,         0.561f, 0.086f, 0.000f,         0.0f,
/*23*/  -0.125f+0.350f,-0.600f, 0.424f+0.03f,   0.561f, 0.086f, 0.000f,         0.0f,


    //torso 2nd layer
/*24*/  -0.125f-0.550f,-0.300f, 0.000f-0.1f,    0.702f, 0.129f, 0.075f,         0.0f,
/*25*/  -0.250f-0.550f,-0.300f, 0.217f,         0.702f, 0.129f, 0.075f,         0.0f,
/*26*/  -0.125f-0.550f,-0.300f, 0.424f+0.1f,    0.702f, 0.129f, 0.075f,         0.0f,
/*27*/   0.125f+0.550f,-0.300f, 0.000f-0.1f,    0.702f, 0.129f, 0.075f,         0.0f,
/*28*/   0.250f+0.550f,-0.300f, 0.217f,         0.702f, 0.129f, 0.075f,         0.0f,
/*29*/   0.125f+0.550f,-0.300f, 0.424f+0.1f,    0.702f, 0.129f, 0.075f,         0.0f,
/*30*/   0.125f-0.350f,-0.300f, 0.000f-0.2f,    0.702f, 0.129f, 0.075f,         0.0f,
/*31*/   0.125f-0.350f,-0.300f, 0.424f+0.2f,    0.702f, 0.129f, 0.075f,         0.0f,
/*32*/  -0.125f+0.350f,-0.300f, 0.000f-0.2f,    0.702f, 0.129f, 0.075f,         0.0f,
/*33*/  -0.125f+0.350f,-0.300f, 0.424f+0.2f,    0.702f, 0.129f, 0.075f,         0.0f,

    //torso 3rd layer
/*34*/  -0.125f-0.550f, 0.100f, 0.000f-0.1f,    0.702f, 0.129f, 0.075f,         0.0f,
/*35*/  -0.250f-0.550f, 0.100f, 0.217f,         0.702f, 0.129f, 0.075f,         0.0f,
/*36*/  -0.125f-0.550f, 0.100f, 0.424f+0.1f,    0.702f, 0.129f, 0.075f,         0.0f,
/*37*/   0.125f+0.550f, 0.100f, 0.000f-0.1f,    0.702f, 0.129f, 0.075f,         0.0f,
/*38*/   0.250f+0.550f, 0.100f, 0.217f,         0.702f, 0.129f, 0.075f,         0.0f,
/*39*/   0.125f+0.550f, 0.100f, 0.424f+0.1f,    0.702f, 0.129f, 0.075f,         0.0f,
/*40*/   0.125f-0.350f, 0.100f, 0.000f-0.2f,    0.702f, 0.129f, 0.075f,         0.0f,
/*41*/   0.125f-0.450f, 0.100f, 0.424f+0.2f,    0.561f, 0.086f, 0.000f,         0.0f,
/*42*/  -0.125f+0.350f, 0.100f, 0.000f-0.2f,    0.702f, 0.129f, 0.075f,         0.0f,
/*43*/  -0.125f+0.450f, 0.100f, 0.424f+0.2f,    0.561f, 0.086f, 0.000f,         0.0f,

    //head 4th layer 
/*44*/  -0.125f-0.525f, 0.400f, 0.000f-0.1f,    0.702f, 0.129f, 0.075f,         0.0f,
/*45*/  -0.250f-0.525f, 0.400f, 0.217f,         0.702f, 0.129f, 0.075f,         0.0f,
/*46*/  -0.125f-0.525f, 0.400f, 0.424f+0.1f,    0.702f, 0.129f, 0.075f,         0.0f,
/*47*/   0.125f+0.525f, 0.400f, 0.000f-0.1f,    0.702f, 0.129f, 0.075f,         0.0f,
/*48*/   0.250f+0.525f, 0.400f, 0.217f,         0.702f, 0.129f, 0.075f,         0.0f,
/*49*/   0.125f+0.525f, 0.400f, 0.424f+0.1f,    0.702f, 0.129f, 0.075f,         0.0f,
/*50*/   0.125f-0.450f, 0.400f, 0.000f-0.2f,    0.702f, 0.129f, 0.075f,         0.0f,
/*51*/   0.125f-0.650f, 0.400f, 0.424f+0.14f,   0.702f, 0.129f, 0.075f,         0.0f,
/*52*/  -0.125f+0.450f, 0.400f, 0.000f-0.2f,    0.702f, 0.129f, 0.075f,         0.0f,
/*53*/  -0.125f+0.650f, 0.400f, 0.424f+0.14f,   0.702f, 0.129f, 0.075f,         0.0f,

    //head 5th layer
/*54*/  -0.125f-0.500f, 0.750f, 0.000f-0.04f,   0.702f, 0.129f, 0.075f,         0.0f,
/*55*/  -0.250f-0.500f, 0.700f, 0.217f,         0.702f, 0.129f, 0.075f,         0.0f,
/*56*/  -0.125f-0.500f, 0.700f, 0.424f+0.04f,   0.702f, 0.129f, 0.075f,         0.0f,
/*57*/   0.125f+0.500f, 0.750f, 0.000f-0.04f,   0.702f, 0.129f, 0.075f,         0.0f,
/*58*/   0.250f+0.500f, 0.700f, 0.217f,         0.702f, 0.129f, 0.075f,         0.0f,
/*59*/   0.125f+0.500f, 0.700f, 0.424f+0.04f,   0.702f, 0.129f, 0.075f,         0.0f,
/*60*/   0.125f-0.350f, 0.750f, 0.000f-0.14f,   0.702f, 0.129f, 0.075f,         0.0f,
/*61*/   0.125f-0.450f, 0.700f, 0.424f+0.10f,   0.702f, 0.129f, 0.075f,         0.0f,
/*62*/  -0.125f+0.350f, 0.750f, 0.000f-0.14f,   0.702f, 0.129f, 0.075f,         0.0f,
/*63*/  -0.125f+0.450f, 0.700f, 0.424f+0.10f,   0.702f, 0.129f, 0.075f,         0.0f,

    //head 6th layer
/*64*/  -0.125f-0.200f, 1.000f, 0.000f,         0.702f, 0.129f, 0.075f,         0.0f,
/*65*/  -0.250f-0.200f, 1.000f, 0.217f,         0.702f, 0.129f, 0.075f,         0.0f,
/*66*/  -0.125f-0.200f, 1.000f, 0.424f,         0.702f, 0.129f, 0.075f,         0.0f,
/*67*/   0.125f+0.200f, 1.000f, 0.000f,         0.702f, 0.129f, 0.075f,         0.0f,
/*68*/   0.250f+0.200f, 1.000f, 0.217f,         0.702f, 0.129f, 0.075f,         0.0f,
/*69*/   0.125f+0.200f, 1.000f, 0.424f,         0.702f, 0.129f, 0.075f,         0.0f,
/*70*/   0.125f-0.250f, 1.000f, 0.000f,         0.702f, 0.129f, 0.075f,         0.0f,
/*71*/   0.125f-0.350f, 1.000f, 0.424f,         0.702f, 0.129f, 0.075f,         0.0f,
/*72*/  -0.125f+0.250f, 1.000f, 0.000f,         0.702f, 0.129f, 0.075f,         0.0f,
/*73*/  -0.125f+0.350f, 1.000f, 0.424f,         0.702f, 0.129f, 0.075f,         0.0f,

    //glass
/*74*/   0.125f-0.450f, 0.100f, 0.424f+0.2f,    0.000f, 1.000f, 1.000f,         1.0f,
/*75*/  -0.125f+0.450f, 0.100f, 0.424f+0.2f,    0.000f, 1.000f, 1.000f,         1.0f,
/*76*/   0.125f-0.650f, 0.400f, 0.424f+0.14f,   0.000f, 1.000f, 1.000f,         1.0f,
/*77*/  -0.125f+0.650f, 0.400f, 0.424f+0.14f,   0.000f, 1.000f, 1.000f,         1.0f,
/*78*/   0.125f-0.450f, 0.700f, 0.424f+0.10f,   0.000f, 1.000f, 1.000f,         1.0f,
/*79*/  -0.125f+0.450f, 0.700f, 0.424f+0.10f,   0.000f, 1.000f, 1.000f,         1.0f,

/*80*/   0.125f-0.350f, 0.250f, 0.424f+0.35f,   0.000f, 1.000f, 1.000f,         1.0f,
/*81*/  -0.125f+0.350f, 0.250f, 0.424f+0.35f,   0.000f, 1.000f, 1.000f,         1.0f,
/*82*/   0.125f-0.500f, 0.400f, 0.424f+0.35f,   0.000f, 1.000f, 1.000f,         1.0f,

/*83*/  -0.125f+0.500f, 0.400f, 0.424f+0.35f,   0.000f, 1.000f, 1.000f,         1.0f,
/*84*/   0.125f-0.350f, 0.650f, 0.424f+0.30f,   0.000f, 1.000f, 1.000f,         1.0f,
/*85*/  -0.125f+0.350f, 0.650f, 0.424f+0.30f,   0.000f, 1.000f, 1.000f,         1.0f,

    //backpack
/*86*/  -0.125f-0.550f,-0.300f, 0.000f-0.5f,    0.561f, 0.086f, 0.000f,         0.0f,
/*87*/   0.125f+0.550f,-0.300f, 0.000f-0.5f,    0.702f, 0.129f, 0.075f,         0.0f,
/*88*/  -0.125f-0.550f, 0.100f, 0.000f-0.5f,    0.702f, 0.129f, 0.075f,         0.0f,
/*89*/   0.125f+0.550f, 0.100f, 0.000f-0.5f,    0.702f, 0.129f, 0.075f,         0.0f,
/*90*/  -0.125f-0.525f, 0.400f, 0.000f-0.5f,    0.702f, 0.129f, 0.075f,         0.0f,
/*91*/   0.125f+0.525f, 0.400f, 0.000f-0.5f,    0.702f, 0.129f, 0.075f,         0.0f,
/*92*/  -0.125f-0.500f, 0.750f, 0.000f-0.45f,   0.561f, 0.086f, 0.000f,         0.0f,
/*93*/   0.125f+0.500f, 0.750f, 0.000f-0.45f,   0.561f, 0.086f, 0.000f,         0.0f


};

GLuint indices[] = {
    //left foot base
     0,  2,  1,
     0,  3,  2,
     2,  3,  5,
     3,  4,  5,
    
    //right foot base
     6,  8,  7,
     6,  9,  8,
     8,  9, 11,
     9, 10, 11,

    //connecting feet to torso
     0,  1, 13,
     0, 12,  3,
     0, 13, 12,
     1,  2, 13,
     2, 14, 13,
     2,  5, 14,

     8, 11, 17,
     6, 15,  9,
     9, 15, 10,
    10, 15, 16,
    10, 17, 11,
    10, 16, 17,

     3, 12, 18,
     3, 18,  4,
     4, 18, 19,
     4, 19, 20,
     4, 20,  5,
     5, 20, 14,
     6, 21, 15,
     6,  7, 21,
     7, 22, 21,
     7,  8, 22,
     8, 23, 22,
     8, 17, 23,

    18, 21, 19,
    19, 21, 22,
    19, 22, 20,
    20, 22, 23,

    //2nd layer
    12, 13, 24,
    13, 25, 24,
    13, 14, 25,
    14, 26, 25,
    14, 20, 26,
    20, 31, 26,
    20, 23, 33,
    20, 33, 31,
    23, 29, 33, 
    17, 29, 23,
    16, 29, 17,
    16, 28, 29,
    15, 28, 16,
    15, 27, 28,
    15, 21, 27,
    21, 32, 27,
    18, 32, 21,
    18, 30, 32,
    18, 24, 30,
    12, 24, 18,

    //3rd layer
    24, 25, 34,
    25, 35, 34,
    25, 26, 35,
    26, 36, 35,
    26, 31, 36,
    31, 41, 36,
    31, 33, 41,
    33, 43, 41,
    29, 39, 33,
    33, 39, 43,
    28, 39, 29,
    28, 38, 39,
    27, 38, 28,
    27, 37, 38,
    27, 32, 37,
    32, 42, 37,
    30, 42, 32,
    30, 40, 42,
    24, 40, 30,
    24, 34, 40,

    //4th layer
    39, 53, 43,
    39, 49, 53,
    38, 49, 39,
    38, 48, 49,
    37, 48, 38,
    37, 47, 48,
    37, 42, 47,
    42, 52, 47,
    40, 52, 42,
    40, 50, 52,
    34, 50, 40,
    34, 44, 50, 
    34, 35, 44,
    35, 45, 44,
    35, 36, 45,
    46, 45, 36,
    36, 41, 46,
    46, 41, 51,

    // //5th layer
    61, 56, 51,
    46, 51, 56,
    45, 46, 56,
    45, 56, 55,
    44, 45, 55,
    44, 55, 54,
    44, 54, 50,
    50, 54, 60,
    60, 52, 50,
    52, 60, 62,
    47, 52, 62,
    47, 62, 57,
    57, 48, 47,
    48, 57, 58,
    58, 49, 48,
    49, 58, 59,
    49, 59, 53,
    53, 59, 63,

    //6th layer
    56, 61, 71,
    61, 63, 71,
    63, 73, 71,
    59, 73, 63,
    59, 69, 73,
    58, 69, 59,
    58, 68, 69,
    57, 68, 58,
    57, 67, 68,
    57, 62, 67,
    72, 67, 62,
    60, 72, 62,
    60, 70, 72,
    54, 70, 60,
    54, 64, 70,
    54, 55, 64,
    55, 65, 64,
    55, 56, 65,
    56, 66, 65,
    66, 56, 71,

    //top
    64, 65, 66,
    64, 66, 71,
    64, 71, 70,
    70, 71, 72,
    73, 72, 71,
    69, 72, 73,
    69, 67, 72, 
    69, 68, 67,

    //glass
    74, 75, 80,
    75, 81, 80,
    76, 74, 80,
    76, 80, 82,
    75, 77, 81,
    77, 83, 81,
    78, 76, 82,
    78, 82, 84,
    77, 79, 83,
    79, 85, 83,
    79, 78, 84,
    79, 84, 85,
    80, 81, 82,
    83, 82, 81,
    82, 83, 84,
    85, 84, 83,//

    //backpack
    24, 34, 86,
    34, 88, 86,
    34, 44, 88,
    44, 90, 88,
    44, 54, 90,
    54, 92, 90,
    54, 57, 92,
    57, 93, 92,
    47, 93, 57,
    47, 91, 93,
    47, 37, 91,
    37, 89, 91,//
    37, 27, 89,
    87, 89, 27,
    27, 24, 86,
    27, 86, 87,
    88, 87, 86,
    87, 88, 89,
    89, 88, 90,
    89, 90, 91,
    92, 91, 90,
    93, 91, 92, 

};

// define OpenGL object IDs to represent the vertex array and the shader program in the GPU
GLuint vao;         // vertex array object (stores the render state for our vertex array)
GLuint vbo;         // vertex buffer object (reserves GPU memory for our vertex array)
GLuint ebo;        // vertex buffer object (reserves GPU memory for our vertex array)
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*) 0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*) (3 * sizeof(float)));
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*) (6 * sizeof(float)));


    // enable the newly-created layout location 0;
    // this shall be used by our vertex shader to read the vertex's x, y, and z
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glEnable(GL_CULL_FACE); 

    // important: if you have more vertex arrays to draw, make sure you separately define them
    // with unique VAO and VBO IDs, and follow the same process above to upload them to the GPU


    // load our shader program
    shader = gdevLoadShader("Exercise3.vs", "Exercise3.fs");
    if (! shader)
        return false;

    texture1 = gdevLoadTexture("cloth.jpg", GL_MIRRORED_REPEAT, true, true);
    if (! texture1) return false;
    texture2 = gdevLoadTexture("glass.png", GL_MIRRORED_REPEAT, true, true);
    if (! texture2) return false;

    return true;
}

// called by the main function to do rendering per frame
void render()
{
    float timer = glfwGetTime();

    //camera
    glm::mat4 view;
    view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

    // clear the whole frame
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    float time = glfwGetTime();


    // using our shader program...
    glUseProgram(shader);
    glEnable(GL_DEPTH_TEST); // enable OpenGL's hidden surface removal
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, 2);
    

    glUniform1i(glGetUniformLocation(shader, "texture1"), 0);
    glUniform1i(glGetUniformLocation(shader, "texture2"), 1);
    glUniform1f(glGetUniformLocation(shader, "time"), time);


    glm::mat4 matrix;
    matrix = glm::perspective(glm::radians(60.0f),
                             (float) WINDOW_WIDTH / WINDOW_HEIGHT,
                              0.1f, 100.0f);

    //set the default view of the camera
    matrix *= view;
    
    //middle amogus
    matrix = glm::rotate(matrix, glm::radians(timer*100), glm::vec3(0.0f, 1.0f, 0.0f)); //rotate along the y axis

    glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(GLuint), GL_UNSIGNED_INT, 0);
    glUniformMatrix4fv(glGetUniformLocation(shader, "matrix"), 1, GL_FALSE, glm::value_ptr(matrix));

    //right amogus
    matrix = glm::rotate(matrix, -glm::radians(timer*100), glm::vec3(0.0f, 1.0f, 0.0f)); //reset rotation

    matrix = glm::translate(matrix, glm::vec3(3.0f, 0.0f, 0.0f)); //move 3 units right
    matrix = glm::rotate(matrix, glm::radians(timer*100), glm::vec3(0.0f, 0.0f, 1.0f)); //rotate along the z axis
    matrix = glm::scale(matrix, glm::vec3(1.0f, 2.0f, 1.0f)); //scale to 1.5 times the size
    
    glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(GLuint), GL_UNSIGNED_INT, 0);
    glUniformMatrix4fv(glGetUniformLocation(shader, "matrix"), 1, GL_FALSE, glm::value_ptr(matrix));

    //left amogus
    matrix = glm::scale(matrix, glm::vec3(1.0f, 0.5f, 1.0f)); //reset the scale
    matrix = glm::rotate(matrix, -glm::radians(timer*100), glm::vec3(0.0f, 0.0f, 1.0f)); //reset rotation
    
    matrix = glm::translate(matrix, glm::vec3(-6.0f, 0.0f, 0.0f)); //move 6 units left (3 units left of middle amogus)
    matrix = glm::rotate(matrix, glm::radians(-timer*100), glm::vec3(1.0f, 0.0f, 0.0f)); //rotate along the x axis
    matrix = glm::scale(matrix, glm::vec3(1.0f, 0.3f, 0.5f)); //scale to 0.5 the size
    
    glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(GLuint), GL_UNSIGNED_INT, 0);
    glUniformMatrix4fv(glGetUniformLocation(shader, "matrix"), 1, GL_FALSE, glm::value_ptr(matrix));

    // ... draw our triangles
    glBindVertexArray(vao);    
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