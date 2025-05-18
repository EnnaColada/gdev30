/******************************************************************************
*   Controls: 
*     W - up
*     A - left
*     S - down
*     D - right
*     mouse - look around
*     [ - decrease light level
*     ] - increase light level
*     - - decrease specularity
*     + - increase specularity
*     F - kill amogus/reset scene
******************************************************************************/

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include <gdev.h>

// change this to your desired window attributes
#define WINDOW_WIDTH  1280
#define WINDOW_HEIGHT 720
#define WINDOW_TITLE  "That's Sus part 2"
GLFWwindow *pWindow;

glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f,  3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);

float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

float lastX = 320, lastY = 180;
float yaw = -90.0f, pitch = 0.0f;
float fov = 45.0f;
bool firstMouse = true;

glm::vec3 lightPosition;
glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
float specularity = 0.5f;
float lightHeight = 2.0f;
int pauseLight = 1;
int halfAppear = 1;

// knife animation
bool isStabbing = false;
float animationStartTime = 0.0f;
bool fKeyPressed = false;

int bloodCount = 0;
bool spawnBlood = false;
glm::mat4 bloodMatrices[10];


float botHalfVertices[] = {

            // position (x, y, z)            color (r, g, b)                normals(x, y, z)
            // base feet
    /*0*/     -0.525f, -1.000f,  0.000f,      0.961f,  0.961f,  0.341f,     -0.664f, -0.596f, -0.452f,
    /*1*/     -0.650f, -1.000f,  0.217f,      0.961f,  0.961f,  0.341f,     -0.785f, -0.620f,  0.007f,
    /*2*/     -0.525f, -1.000f,  0.424f,      0.961f,  0.961f,  0.341f,     -0.609f, -0.694f,  0.384f,
    /*3*/     -0.275f, -1.000f,  0.000f,      0.961f,  0.961f,  0.341f,     -0.491f, -0.695f, -0.525f,
    /*4*/     -0.150f, -1.000f,  0.217f,      0.961f,  0.961f,  0.341f,     -0.543f, -0.840f,  0.018f,
    /*5*/     -0.275f, -1.000f,  0.424f,      0.961f,  0.961f,  0.341f,     -0.314f, -0.641f,  0.700f,
    /*6*/      0.275f, -1.000f,  0.000f,      0.961f,  0.961f,  0.341f,      0.470f, -0.598f, -0.649f,
    /*7*/      0.150f, -1.000f,  0.217f,      0.961f,  0.961f,  0.341f,      0.523f, -0.813f, -0.256f,
    /*8*/      0.275f, -1.000f,  0.424f,      0.961f,  0.961f,  0.341f,      0.473f, -0.663f,  0.580f,
    /*9*/      0.525f, -1.000f,  0.000f,      0.961f,  0.961f,  0.341f,      0.582f, -0.743f, -0.331f,
    /*10*/     0.650f, -1.000f,  0.217f,      0.961f,  0.961f,  0.341f,      0.855f, -0.518f,  0.009f,
    /*11*/     0.525f, -1.000f,  0.424f,      0.961f,  0.961f,  0.341f,      0.419f, -0.763f,  0.492f,

            // base torso
    /*12*/    -0.625f, -0.600f, -0.030f,      0.961f,  0.961f,  0.341f,     -0.547f, -0.185f, -0.816f,
    /*13*/    -0.750f, -0.600f,  0.217f,      0.961f,  0.961f,  0.341f,     -0.945f, -0.321f, -0.067f,
    /*14*/    -0.625f, -0.600f,  0.454f,      0.961f,  0.961f,  0.341f,     -0.399f, -0.211f,  0.892f,
    /*15*/     0.625f, -0.600f, -0.030f,      0.961f,  0.961f,  0.341f,      0.626f, -0.196f, -0.754f,
    /*16*/     0.750f, -0.600f,  0.217f,      0.961f,  0.961f,  0.341f,      0.954f, -0.278f,  0.111f,
    /*17*/     0.625f, -0.600f,  0.454f,      0.961f,  0.961f,  0.341f,      0.434f, -0.217f,  0.874f,
    /*18*/    -0.225f, -0.600f, -0.030f,      0.801f,  0.612f,  0.133f,     -0.254f, -0.466f, -0.848f,
    /*19*/    -0.100f, -0.600f,  0.217f,      0.801f,  0.612f,  0.133f,     -0.047f, -0.999f,  0.005f,
    /*20*/    -0.225f, -0.600f,  0.454f,      0.801f,  0.612f,  0.133f,     -0.231f, -0.572f,  0.787f,
    /*21*/     0.225f, -0.600f, -0.030f,      0.801f,  0.612f,  0.133f,      0.265f, -0.584f, -0.767f,
    /*22*/     0.100f, -0.600f,  0.217f,      0.801f,  0.612f,  0.133f,      0.195f, -0.969f,  0.152f,
    /*23*/     0.225f, -0.600f,  0.454f,      0.801f,  0.612f,  0.133f,      0.184f, -0.495f,  0.849f,

            // torso 2nd layer
    /*24*/    -0.675f, -0.300f, -0.100f,      0.961f,  0.961f,  0.341f,     -0.806f,  0.211f, -0.552f,
    /*25*/    -0.800f, -0.300f,  0.217f,      0.961f,  0.961f,  0.341f,     -0.886f,  0.463f,  0.014f,
    /*26*/    -0.675f, -0.300f,  0.524f,      0.961f,  0.961f,  0.341f,     -0.681f,  0.190f,  0.708f,
    /*27*/     0.675f, -0.300f, -0.100f,      0.961f,  0.961f,  0.341f,      0.638f, -0.181f, -0.748f,
    /*28*/     0.800f, -0.300f,  0.217f,      0.961f,  0.961f,  0.341f,      0.890f,  0.456f, -0.006f,
    /*29*/     0.675f, -0.300f,  0.524f,      0.961f,  0.961f,  0.341f,      0.735f,  0.289f,  0.613f,
    /*30*/    -0.225f, -0.300f, -0.200f,      0.961f,  0.961f,  0.341f,     -0.398f,  0.496f, -0.772f,
    /*31*/    -0.225f, -0.300f,  0.624f,      0.961f,  0.961f,  0.341f,     -0.340f,  0.364f,  0.867f,
    /*32*/     0.225f, -0.300f, -0.200f,      0.961f,  0.961f,  0.341f,      0.321f,  0.252f, -0.913f,
    /*33*/     0.225f, -0.300f,  0.624f,      0.961f,  0.961f,  0.341f,      0.253f,  0.211f,  0.944f,

            // torso cut half
    /*34*/    -0.600f, -0.300f, -0.010f,      0.801f,  0.612f,  0.133f,     -0.611f,  0.685f,  0.396f,
    /*35*/    -0.690f, -0.300f,  0.217f,      0.801f,  0.612f,  0.133f,     -0.851f,  0.517f,  0.092f,
    /*36*/    -0.600f, -0.300f,  0.434f,      0.801f,  0.612f,  0.133f,     -0.607f,  0.729f, -0.317f,
    /*37*/     0.600f, -0.300f, -0.010f,      0.801f,  0.612f,  0.133f,      0.552f,  0.767f,  0.328f,
    /*38*/     0.690f, -0.300f,  0.217f,      0.801f,  0.612f,  0.133f,      0.850f,  0.517f, -0.103f,
    /*39*/     0.600f, -0.300f,  0.434f,      0.801f,  0.612f,  0.133f,      0.661f,  0.593f, -0.460f,
    /*40*/    -0.225f, -0.300f, -0.100f,      0.801f,  0.612f,  0.133f,     -0.273f,  0.683f,  0.677f,
    /*41*/    -0.225f, -0.300f,  0.524f,      0.801f,  0.612f,  0.133f,     -0.176f,  0.703f, -0.690f,
    /*42*/     0.225f, -0.300f, -0.100f,      0.801f,  0.612f,  0.133f,      0.176f,  0.703f,  0.690f,
    /*43*/     0.225f, -0.300f,  0.524f,      0.801f,  0.612f,  0.133f,      0.378f,  0.658f, -0.652f,
    /*44*/    -0.600f, -0.350f, -0.010f,      0.801f,  0.612f,  0.133f,     -0.670f,  0.564f,  0.482f,
    /*45*/    -0.690f, -0.350f,  0.217f,      0.801f,  0.612f,  0.133f,     -0.562f,  0.823f, -0.082f,
    /*46*/    -0.600f, -0.350f,  0.434f,      0.801f,  0.612f,  0.133f,     -0.427f,  0.589f, -0.686f,
    /*47*/     0.600f, -0.350f, -0.010f,      0.801f,  0.612f,  0.133f,      0.428f,  0.591f,  0.684f,
    /*48*/     0.690f, -0.350f,  0.217f,      0.801f,  0.612f,  0.133f,      0.563f,  0.823f,  0.073f,
    /*49*/     0.600f, -0.350f,  0.434f,      0.801f,  0.612f,  0.133f,      0.668f,  0.562f, -0.488f,
    /*50*/    -0.225f, -0.350f, -0.100f,      0.801f,  0.612f,  0.133f,     -0.422f,  0.509f,  0.750f,
    /*51*/    -0.225f, -0.350f,  0.524f,      0.801f,  0.612f,  0.133f,     -0.002f,  0.558f, -0.830f,
    /*52*/     0.225f, -0.350f, -0.100f,      0.801f,  0.612f,  0.133f,     -0.018f,  0.710f,  0.704f,
    /*53*/     0.225f, -0.350f,  0.524f,      0.801f,  0.612f,  0.133f,      0.351f,  0.668f, -0.656f,
    /*54*/    -0.600f, -0.350f, -0.010f,      0.322f,  0.000f,  0.217f,     -0.670f,  0.564f,  0.482f,
    /*55*/    -0.690f, -0.350f,  0.217f,      0.322f,  0.000f,  0.217f,     -0.562f,  0.823f, -0.082f,
    /*56*/    -0.600f, -0.350f,  0.434f,      0.322f,  0.000f,  0.217f,     -0.427f,  0.589f, -0.686f,
    /*57*/     0.600f, -0.350f, -0.010f,      0.322f,  0.000f,  0.217f,      0.428f,  0.591f,  0.684f,
    /*58*/     0.690f, -0.350f,  0.217f,      0.322f,  0.000f,  0.217f,      0.563f,  0.823f,  0.073f,
    /*59*/     0.600f, -0.350f,  0.434f,      0.322f,  0.000f,  0.217f,      0.668f,  0.562f, -0.488f,
    /*60*/    -0.225f, -0.350f, -0.100f,      0.322f,  0.000f,  0.217f,     -0.422f,  0.509f,  0.750f,
    /*61*/    -0.225f, -0.350f,  0.524f,      0.322f,  0.000f,  0.217f,     -0.002f,  0.558f, -0.830f,
    /*62*/     0.225f, -0.350f, -0.100f,      0.322f,  0.000f,  0.217f,     -0.018f,  0.710f,  0.704f,
    /*63*/     0.225f, -0.350f,  0.524f,      0.322f,  0.000f,  0.217f,      0.351f,  0.668f, -0.656f,
    /*64*/    -0.083f, -0.350f,  0.072f,      0.322f,  0.000f,  0.217f,     -0.302f,  0.853f, -0.426f,
    /*65*/    -0.167f, -0.350f,  0.217f,      0.322f,  0.000f,  0.217f,     -0.473f,  0.855f, -0.214f,
    /*66*/    -0.083f, -0.350f,  0.362f,      0.322f,  0.000f,  0.217f,     -0.131f,  0.920f,  0.369f,
    /*67*/     0.083f, -0.350f,  0.072f,      0.322f,  0.000f,  0.217f,      0.296f,  0.854f, -0.428f,
    /*68*/     0.167f, -0.350f,  0.217f,      0.322f,  0.000f,  0.217f,      0.473f,  0.855f,  0.214f,
    /*69*/     0.083f, -0.350f,  0.362f,      0.322f,  0.000f,  0.217f,      0.401f,  0.777f,  0.486f,

            // bone
    /*70*/    -0.083f, -0.350f,  0.072f,      0.957f,  0.949f,  0.925f,     -0.302f,  0.853f, -0.426f,
    /*71*/    -0.167f, -0.350f,  0.217f,      0.957f,  0.949f,  0.925f,     -0.473f,  0.855f, -0.214f,
    /*72*/    -0.083f, -0.350f,  0.362f,      0.957f,  0.949f,  0.925f,     -0.131f,  0.920f,  0.369f,
    /*73*/     0.083f, -0.350f,  0.072f,      0.957f,  0.949f,  0.925f,      0.296f,  0.854f, -0.428f,
    /*74*/     0.167f, -0.350f,  0.217f,      0.957f,  0.949f,  0.925f,      0.473f,  0.855f,  0.214f,
    /*75*/     0.083f, -0.350f,  0.362f,      0.957f,  0.949f,  0.925f,      0.401f,  0.777f,  0.486f,
    /*76*/    -0.083f, -0.050f,  0.072f,      0.890f,  0.850f,  0.790f,     -0.217f, -0.269f, -0.938f,
    /*77*/    -0.167f, -0.050f,  0.217f,      0.890f,  0.850f,  0.790f,     -0.341f, -0.905f,  0.256f,
    /*78*/    -0.083f, -0.050f,  0.362f,      0.890f,  0.850f,  0.790f,     -0.053f, -0.144f,  0.988f,
    /*79*/     0.083f, -0.050f,  0.072f,      0.890f,  0.850f,  0.790f,      0.063f, -0.074f, -0.995f,
    /*80*/     0.167f, -0.050f,  0.217f,      0.890f,  0.850f,  0.790f,      0.300f, -0.929f, -0.218f,
    /*81*/     0.083f, -0.050f,  0.362f,      0.890f,  0.850f,  0.790f,      0.197f, -0.278f,  0.940f,

            // bone left buttcheek 
            // layer 1
    /*82*/    -0.282f,  0.050f,  0.054f,      0.890f,  0.850f,  0.790f,     -0.233f, -0.265f, -0.936f,
    /*83*/    -0.376f,  0.050f,  0.217f,      0.890f,  0.850f,  0.790f,     -0.453f, -0.858f, -0.242f,
    /*84*/    -0.282f,  0.050f,  0.380f,      0.890f,  0.850f,  0.790f,     -0.346f, -0.456f,  0.820f,
    /*85*/    -0.094f,  0.050f,  0.054f,      0.890f,  0.850f,  0.790f,     -0.107f, -0.056f, -0.993f,
    /*86*/     0.000f,  0.050f,  0.136f,      0.890f,  0.850f,  0.790f,     -0.003f,  0.123f, -0.992f,
    /*87*/     0.000f,  0.050f,  0.325f,      0.890f,  0.850f,  0.790f,     -0.019f,  0.048f,  0.999f,
    /*88*/    -0.094f,  0.050f,  0.380f,      0.890f,  0.850f,  0.790f,     -0.155f, -0.058f,  0.986f,

            // layer 2
    /*89*/    -0.282f,  0.250f,  0.054f,      0.957f,  0.949f,  0.925f,     -0.342f,  0.374f, -0.862f,
    /*90*/    -0.376f,  0.250f,  0.217f,      0.957f,  0.949f,  0.925f,     -0.695f,  0.718f,  0.038f,
    /*91*/    -0.282f,  0.250f,  0.380f,      0.957f,  0.949f,  0.925f,     -0.263f,  0.377f,  0.888f,
    /*92*/    -0.094f,  0.250f,  0.054f,      0.957f,  0.949f,  0.925f,     -0.228f,  0.276f, -0.934f,
    /*93*/     0.000f,  0.200f,  0.136f,      0.890f,  0.850f,  0.790f,     -0.060f,  0.597f, -0.800f,
    /*94*/     0.000f,  0.200f,  0.325f,      0.890f,  0.850f,  0.790f,      0.023f,  0.653f,  0.757f,
    /*95*/    -0.094f,  0.250f,  0.380f,      0.957f,  0.949f,  0.925f,     -0.179f,  0.340f,  0.923f,

            // layer 3
    /*96*/    -0.251f,  0.325f,  0.108f,      0.957f,  0.949f,  0.925f,     -0.298f,  0.903f, -0.309f,
    /*97*/    -0.314f,  0.325f,  0.217f,      0.957f,  0.949f,  0.925f,     -0.358f,  0.924f,  0.136f,
    /*98*/    -0.251f,  0.325f,  0.326f,      0.957f,  0.949f,  0.925f,     -0.202f,  0.861f,  0.466f,
    /*99*/    -0.126f,  0.325f,  0.108f,      0.957f,  0.949f,  0.925f,     -0.185f,  0.858f, -0.480f,
    /*100*/   -0.063f,  0.325f,  0.217f,      0.890f,  0.850f,  0.790f,     -0.016f,  1.000f, -0.007f,
    /*101*/   -0.126f,  0.325f,  0.326f,      0.957f,  0.949f,  0.925f,     -0.133f,  0.937f,  0.324f,

            // bone right buttcheek 
            // layer 1
    /*102*/    0.084f,  0.000f,  0.072f,      0.890f,  0.850f,  0.790f,      0.108f,  0.002f, -0.994f,
    /*103*/    0.084f,  0.000f,  0.362f,      0.890f,  0.850f,  0.790f,      0.088f,  0.002f,  0.996f,
    /*104*/    0.250f,  0.000f,  0.072f,      0.890f,  0.850f,  0.790f,      0.292f, -0.472f, -0.832f,
    /*105*/    0.334f,  0.000f,  0.217f,      0.890f,  0.850f,  0.790f,      0.438f, -0.875f,  0.205f,
    /*106*/    0.250f,  0.000f,  0.362f,      0.890f,  0.850f,  0.790f,      0.215f, -0.244f,  0.946f,

            // layer 2
    /*107*/    0.084f,  0.175f,  0.072f,      0.957f,  0.949f,  0.925f,      0.123f,  0.484f, -0.866f,
    /*108*/    0.084f,  0.175f,  0.362f,      0.957f,  0.949f,  0.925f,      0.189f,  0.292f,  0.937f,
    /*109*/    0.250f,  0.175f,  0.072f,      0.957f,  0.949f,  0.925f,      0.186f,  0.329f, -0.926f,
    /*110*/    0.334f,  0.175f,  0.217f,      0.957f,  0.949f,  0.925f,      0.629f,  0.674f, -0.388f,
    /*111*/    0.250f,  0.175f,  0.362f,      0.957f,  0.949f,  0.925f,      0.367f,  0.375f,  0.851f,
    
            // layer 3
    /*112*/    0.112f,  0.250f,  0.121f,      0.957f,  0.949f,  0.925f,      0.152f,  0.901f, -0.407f,
    /*113*/    0.056f,  0.250f,  0.217f,      0.957f,  0.949f,  0.925f,      0.081f,  0.983f, -0.163f,
    /*114*/    0.112f,  0.250f,  0.313f,      0.957f,  0.949f,  0.925f,      0.125f,  0.902f,  0.414f,
    /*115*/    0.223f,  0.250f,  0.121f,      0.957f,  0.949f,  0.925f,      0.293f,  0.867f, -0.403f,
    /*116*/    0.279f,  0.250f,  0.217f,      0.957f,  0.949f,  0.925f,      0.137f,  0.978f,  0.158f,
    /*117*/    0.223f,  0.250f,  0.313f,      0.957f,  0.949f,  0.925f,      0.234f,  0.886f,  0.401f,
};

GLuint botHalfIndices[] = {
    //left foot base
    0, 2, 1,
    0, 3, 2,
    2, 3, 5,
    3, 4, 5,

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

    //body outline
    24, 34, 30,
    24, 25, 34,
    25, 35, 34,
    25, 36, 35,
    25, 26, 36,
    26, 31, 36,
    31, 41, 36,
    31, 33, 41,
    33, 43, 41,
    33, 29, 43,
    29, 39, 43,
    29, 28, 39,
    28, 38, 39,
    28, 37, 38,
    28, 27, 37,
    27, 32, 37,
    32, 42, 37,
    30, 42, 32,
    30, 40, 42,
    24, 40, 30,
    24, 34, 40,

    34, 44, 50,
    34, 35, 44,
    35, 45, 44,
    35, 36, 45,
    36, 46, 45,
    36, 41, 46,
    41, 51, 46,
    41, 43, 51,
    43, 53, 51,
    43, 39, 53,
    39, 49, 53,
    39, 38, 49,
    38, 48, 49,
    38, 37, 48,
    37, 47, 48,
    37, 42, 47,
    42, 52, 47,
    42, 40, 52,
    40, 50, 52,
    40, 34, 50,

    //insides
    54, 55, 64,
    55, 65, 64,
    55, 66, 65,
    55, 56, 66,
    56, 61, 66,
    61, 63, 66,
    63, 69, 66,
    63, 59, 69,
    59, 58, 69,
    58, 68, 69,
    58, 67, 68,
    58, 57, 67,
    57, 62, 67,
    62, 64, 67,
    62, 60, 64,
    60, 54, 64,

    //bone stem
    70, 71, 76,
    71, 77, 76,
    71, 72, 77,
    72, 78, 77,
    72, 75, 78,
    75, 81, 78,
    75, 74, 81,
    74, 80, 81,
    74, 73, 80,
    73, 79, 80,
    73, 70, 79,
    70, 76, 79,

    //bone left buttcheek
    //layer 1
    78, 81, 87,
    78, 87, 88,
    78, 88, 84,
    77, 78, 84,
    77, 84, 83,
    76, 77, 83,
    76, 83, 82,
    76, 82, 85,
    76, 85, 86,
    76, 86, 79,

    //layer 2
    86, 85, 93,
    85, 92, 93,
    85, 82, 92,
    82, 89, 92,
    82, 83, 89,
    83, 90, 89,
    83, 84, 90,
    84, 91, 90,
    84, 88, 91,
    88, 95, 91,
    88, 87, 95,
    87, 94, 95,

    //layer 3
    89,  90,  96,
    90,  97,  96,
    90,  91,  97,
    91,  98,  97,
    91,  95,  98,
    95, 101,  98,
    95,  94, 101,
    94, 100, 101,
    94,  93, 100,
    93,  99, 100,
    93,  92,  99,
    92,  89,  99,
    89,  96,  99,

    //close
    96,  97,  98,
    96,  98, 101,
    96, 101,  99,
    99, 101, 100,

    //bone right buttcheek
    //layer 1
    81, 103,  87,
    81, 106, 103,
    81, 105, 106,
    81,  80, 105,
    80, 104, 105,
    80,  79, 104,
    79, 102, 104,
    79,  86, 102,

    //layer 2
     87, 103,  94,
    103, 108,  94,
    103, 106, 108,
    106, 111, 108,
    106, 105, 111,
    105, 110, 111,
    105, 104, 110,
    104, 109, 110,
    104, 102, 109,
    102, 107, 109,
    102,  93, 107,
    102,  86,  93,

    //layer 3
    93,   94, 113,
    94,  114, 113,
    94,  108, 114,
    108, 117, 114,
    108, 111, 117,
    111, 116, 117,
    111, 110, 116,
    110, 115, 116,
    110, 109, 115,
    109, 112, 115,
    109, 107, 112,
    107, 113, 112,
    107,  93, 113,

    //close
    112, 113, 114,
    112, 114, 117,
    112, 117, 115,
    115, 117, 116,
};


float topHalfVertices[] = {

            // position (x, y, z)             color (r, g, b)                normals (x, y, z)      
            // torso 2nd layer
    /*0*/     -0.675f, -0.300f, -0.100f,      0.961f,  0.961f,  0.341f,     -0.806f,  0.211f, -0.552f,
    /*1*/     -0.800f, -0.300f,  0.217f,      0.961f,  0.961f,  0.341f,     -0.886f,  0.463f,  0.014f,
    /*2*/     -0.675f, -0.300f,  0.524f,      0.961f,  0.961f,  0.341f,     -0.681f,  0.190f,  0.708f,
    /*3*/      0.675f, -0.300f, -0.100f,      0.961f,  0.961f,  0.341f,      0.638f, -0.181f, -0.748f,
    /*4*/      0.800f, -0.300f,  0.217f,      0.961f,  0.961f,  0.341f,      0.890f,  0.456f, -0.006f,
    /*5*/      0.675f, -0.300f,  0.524f,      0.961f,  0.961f,  0.341f,      0.735f,  0.289f,  0.613f,
    /*6*/     -0.225f, -0.300f, -0.200f,      0.961f,  0.961f,  0.341f,     -0.398f,  0.496f, -0.772f,
    /*7*/     -0.225f, -0.300f,  0.624f,      0.961f,  0.961f,  0.341f,     -0.340f,  0.364f,  0.867f,
    /*8*/      0.225f, -0.300f, -0.200f,      0.961f,  0.961f,  0.341f,      0.321f,  0.252f, -0.913f,
    /*9*/      0.225f, -0.300f,  0.624f,      0.961f,  0.961f,  0.341f,      0.253f,  0.211f,  0.944f,

            // torso 3rd layer
    /*10*/    -0.675f,  0.100f, -0.100f,      0.961f,  0.961f,  0.341f,     -0.813f,  0.016f, -0.582f,
    /*11*/    -0.800f,  0.100f,  0.217f,      0.961f,  0.961f,  0.341f,     -0.998f,  0.056f,  0.008f,
    /*12*/    -0.675f,  0.100f,  0.524f,      0.961f,  0.961f,  0.341f,     -0.601f,  0.021f,  0.799f,
    /*13*/     0.675f,  0.100f, -0.100f,      0.961f,  0.961f,  0.341f,      0.740f,  0.056f, -0.670f,
    /*14*/     0.800f,  0.100f,  0.217f,      0.961f,  0.961f,  0.341f,      0.999f,  0.054f,  0.007f,
    /*15*/     0.675f,  0.100f,  0.524f,      0.961f,  0.961f,  0.341f,      0.614f,  0.007f,  0.789f,
    /*16*/    -0.225f,  0.100f, -0.200f,      0.961f,  0.961f,  0.341f,      0.000f, -0.012f, -1.000f,
    /*17*/    -0.325f,  0.100f,  0.624f,      0.801f,  0.612f,  0.133f,     -0.132f, -0.163f,  0.978f,
    /*18*/     0.225f,  0.100f, -0.200f,      0.961f,  0.961f,  0.341f,      0.163f, -0.013f, -0.987f,
    /*19*/     0.325f,  0.100f,  0.624f,      0.801f,  0.612f,  0.133f,      0.299f, -0.314f,  0.901f,

            // torso 4th layer
    /*20*/    -0.650f,  0.400f, -0.100f,      0.961f,  0.961f,  0.341f,     -0.865f,  0.116f, -0.489f,
    /*21*/    -0.775f,  0.400f,  0.217f,      0.961f,  0.961f,  0.341f,     -0.970f,  0.129f,  0.204f,
    /*22*/    -0.650f,  0.400f,  0.524f,      0.961f,  0.961f,  0.341f,     -0.625f,  0.097f,  0.775f,
    /*23*/     0.650f,  0.400f, -0.100f,      0.961f,  0.961f,  0.341f,      0.660f,  0.126f, -0.740f,
    /*24*/     0.775f,  0.400f,  0.217f,      0.961f,  0.961f,  0.341f,      0.975f,  0.123f, -0.185f,
    /*25*/     0.650f,  0.400f,  0.524f,      0.961f,  0.961f,  0.341f,      0.753f,  0.118f,  0.647f,
    /*26*/    -0.325f,  0.400f, -0.200f,      0.961f,  0.961f,  0.341f,     -0.202f,  0.089f, -0.975f,
    /*27*/    -0.525f,  0.400f,  0.564f,      0.961f,  0.961f,  0.341f,     -0.490f,  0.012f,  0.872f,
    /*28*/     0.325f,  0.400f, -0.200f,      0.961f,  0.961f,  0.341f,     -0.021f,  0.082f, -0.996f,
    /*29*/     0.525f,  0.400f,  0.564f,      0.961f,  0.961f,  0.341f,      0.506f,  0.017f,  0.862f,

            // head 5th layer
    /*30*/    -0.625f,  0.750f, -0.040f,      0.961f,  0.961f,  0.341f,     -0.499f,  0.516f, -0.696f,
    /*31*/    -0.750f,  0.700f,  0.217f,      0.961f,  0.961f,  0.341f,     -0.807f,  0.577f, -0.126f,
    /*32*/    -0.625f,  0.700f,  0.464f,      0.961f,  0.961f,  0.341f,     -0.576f,  0.351f,  0.738f,
    /*33*/     0.625f,  0.750f, -0.040f,      0.961f,  0.961f,  0.341f,      0.580f,  0.656f, -0.483f,
    /*34*/     0.750f,  0.700f,  0.217f,      0.961f,  0.961f,  0.341f,      0.806f,  0.552f,  0.215f,
    /*35*/     0.625f,  0.700f,  0.464f,      0.961f,  0.961f,  0.341f,      0.496f,  0.321f,  0.807f,
    /*36*/    -0.225f,  0.750f, -0.140f,      0.961f,  0.961f,  0.341f,     -0.051f,  0.355f, -0.933f,
    /*37*/    -0.325f,  0.700f,  0.524f,      0.961f,  0.961f,  0.341f,     -0.314f,  0.561f,  0.766f,
    /*38*/     0.225f,  0.750f, -0.140f,      0.961f,  0.961f,  0.341f,      0.294f,  0.323f, -0.899f,
    /*39*/     0.325f,  0.700f,  0.524f,      0.961f,  0.961f,  0.341f,      0.007f,  0.630f,  0.777f,

            // head 6th layer
    /*40*/    -0.325f,  1.000f,  0.000f,      0.961f,  0.961f,  0.341f,     -0.478f,  0.834f, -0.276f,
    /*41*/    -0.450f,  1.000f,  0.217f,      0.961f,  0.961f,  0.341f,     -0.468f,  0.878f,  0.099f,
    /*42*/    -0.325f,  1.000f,  0.424f,      0.961f,  0.961f,  0.341f,     -0.461f,  0.798f,  0.389f,
    /*43*/     0.325f,  1.000f,  0.000f,      0.961f,  0.961f,  0.341f,      0.141f,  0.859f, -0.493f,
    /*44*/     0.450f,  1.000f,  0.217f,      0.961f,  0.961f,  0.341f,      0.548f,  0.836f, -0.038f,
    /*45*/     0.325f,  1.000f,  0.424f,      0.961f,  0.961f,  0.341f,      0.444f,  0.837f,  0.320f,
    /*46*/    -0.125f,  1.000f,  0.000f,      0.961f,  0.961f,  0.341f,     -0.250f,  0.741f, -0.623f,
    /*47*/    -0.225f,  1.000f,  0.424f,      0.961f,  0.961f,  0.341f,      0.047f,  0.802f,  0.595f,
    /*48*/     0.125f,  1.000f,  0.000f,      0.961f,  0.961f,  0.341f,      0.078f,  0.899f, -0.431f,
    /*49*/     0.225f,  1.000f,  0.424f,      0.961f,  0.961f,  0.341f,      0.309f,  0.668f,  0.677f,

            // glass
    /*50*/    -0.325f,  0.100f,  0.624f,      0.000f,  0.500f,  0.500f,     -0.132f, -0.163f,  0.978f,
    /*51*/     0.325f,  0.100f,  0.624f,      0.000f,  0.500f,  0.500f,      0.299f, -0.314f,  0.901f,
    /*52*/    -0.525f,  0.400f,  0.564f,      0.000f,  0.500f,  0.500f,     -0.490f,  0.012f,  0.872f,
    /*53*/     0.525f,  0.400f,  0.564f,      0.000f,  0.500f,  0.500f,      0.506f,  0.017f,  0.862f,
    /*54*/    -0.325f,  0.700f,  0.524f,      0.000f,  1.000f,  1.000f,     -0.314f,  0.561f,  0.766f,
    /*55*/     0.325f,  0.700f,  0.524f,      0.000f,  1.000f,  1.000f,      0.007f,  0.630f,  0.777f,
    /*56*/    -0.225f,  0.250f,  0.774f,      0.000f,  1.000f,  1.000f,     -0.201f, -0.535f,  0.821f,
    /*57*/     0.225f,  0.250f,  0.774f,      0.000f,  1.000f,  1.000f,     -0.182f, -0.380f,  0.907f,
    /*58*/    -0.375f,  0.400f,  0.774f,      0.000f,  1.000f,  1.000f,      0.166f,  0.136f,  0.977f,
    /*59*/     0.375f,  0.400f,  0.774f,      0.000f,  1.000f,  1.000f,     -0.089f,  0.180f,  0.980f,
    /*60*/    -0.225f,  0.650f,  0.724f,      0.000f,  1.000f,  1.000f,      0.379f,  0.645f,  0.663f,
    /*61*/     0.225f,  0.650f,  0.724f,      0.000f,  1.000f,  1.000f,     -0.031f,  0.703f,  0.711f,
   
            // backpack
    /*62*/    -0.675f, -0.300f, -0.500f,      0.801f,  0.612f,  0.133f,      0.289f, -0.856f, -0.428f,
    /*63*/     0.675f, -0.300f, -0.500f,      0.801f,  0.612f,  0.133f,     -0.517f, -0.383f, -0.766f,
    /*64*/    -0.675f,  0.100f, -0.500f,      0.961f,  0.961f,  0.341f,     -0.391f,  0.051f, -0.919f,
    /*65*/     0.675f,  0.100f, -0.500f,      0.961f,  0.961f,  0.341f,      0.410f,  0.025f, -0.912f,
    /*66*/    -0.650f,  0.400f, -0.500f,      0.961f,  0.961f,  0.341f,      0.008f,  0.121f, -0.993f,
    /*67*/     0.650f,  0.400f, -0.500f,      0.961f,  0.961f,  0.341f,      0.025f,  0.172f, -0.985f,
    /*68*/    -0.625f,  0.750f, -0.450f,      0.961f,  0.961f,  0.341f,      0.383f,  0.707f, -0.595f,
    /*69*/     0.625f,  0.750f, -0.450f,      0.961f,  0.961f,  0.341f,      0.626f,  0.618f, -0.477f,
};

GLuint topHalfIndices[] = {
    //3rd layer
     0,  1, 10,
     1, 11, 10,
     1,  2, 11,
     2, 12, 11,
     2,  7, 12,
     7, 17, 12,
     7,  9, 17,
     9, 19, 17,
     5, 15,  9,
     9, 15, 19,
     4, 15,  5,
     4, 14, 15,
     3, 14,  4,
     3, 13, 14,
     3, 8,  13,
     8, 18, 13,
     6, 18,  8,
     6, 16, 18,
     0, 16,  6,
     0, 10, 16,

    //4th layer
    15, 29, 19,
    15, 25, 29,
    14, 25, 15,
    14, 24, 25,
    13, 24, 14,
    13, 23, 24,
    13, 18, 23,
    18, 28, 23,
    16, 28, 18,
    16, 26, 28,
    10, 26, 16,
    10, 20, 26,
    10, 11, 20,
    11, 21, 20,
    11, 12, 21,
    22, 21, 12,
    12, 17, 22,
    22, 17, 27,

    //5th layer
    37, 32, 27,
    22, 27, 32,
    21, 22, 32,
    21, 32, 31,
    20, 21, 31,
    20, 31, 30,
    20, 30, 26,
    26, 30, 36,
    36, 28, 26,
    28, 36, 38,
    23, 28, 38,
    23, 38, 33,
    33, 24, 23,
    24, 33, 34,
    34, 25, 24,
    25, 34, 35,
    25, 35, 29,
    29, 35, 39,

    //6th layer
    32, 37, 47,
    37, 39, 47,
    39, 49, 47,
    35, 49, 39,
    35, 45, 49,
    34, 45, 35,
    34, 44, 45,
    33, 44, 34,
    33, 43, 44,
    33, 38, 43,
    48, 43, 38,
    36, 48, 38,
    36, 46, 48,
    30, 46, 36,
    30, 40, 46,
    30, 31, 40,
    31, 41, 40,
    31, 32, 41,
    32, 42, 41,
    42, 32, 47,

    //top
    40, 41, 42,
    40, 42, 47,
    40, 47, 46,
    46, 47, 48,
    49, 48, 47,
    45, 48, 49,
    45, 43, 48,
    45, 44, 43,

    //glass
    50, 51, 56,
    51, 57, 56,
    52, 50, 56,
    52, 56, 58,
    51, 53, 57,
    53, 59, 57,
    54, 52, 58,
    54, 58, 60,
    53, 55, 59,
    55, 61, 59,
    55, 54, 60,
    55, 60, 61,
    56, 57, 58,
    59, 58, 57,
    58, 59, 60,
    61, 60, 59,

    //backpack
    0, 10, 62,
    10, 64, 62,
    10, 20, 64,
    20, 66, 64,
    20, 30, 66,
    30, 68, 66,
    30, 33, 68,
    33, 69, 68,
    23, 69, 33,
    23, 67, 69,
    23, 13, 67,
    13, 65, 67,
    13,  3, 65,
    63, 65,  3,
     3,  0, 62,
     3, 62, 63,
    64, 63, 62,
    63, 64, 65,
    65, 64, 66,
    65, 66, 67,
    68, 67, 66,
    69, 67, 68,
};


float knifeVertices[] = {
    
            // position (x, y, z)           color (r, g, b)                normals (x, y, z)      
            //knife blade
    /*0*/   -0.743f,  0.000f,  1.500f,      0.827f,  0.831f,  0.824f,     -0.946f,  0.124f,  0.298f,
    /*1*/   -0.743f, -0.100f,  1.400f,      0.827f,  0.831f,  0.824f,     -0.823f, -0.414f,  0.389f,
    /*2*/   -0.743f,  0.000f,  1.075f,      0.827f,  0.831f,  0.824f,     -0.823f,  0.315f,  0.473f,
    /*3*/   -0.743f, -0.100f,  1.075f,      0.827f,  0.831f,  0.824f,     -0.828f, -0.076f,  0.555f,
    /*4*/   -0.750f, -0.150f,  1.350f,      0.965f,  0.965f,  0.965f,     -0.914f, -0.372f,  0.160f,
    /*5*/   -0.750f, -0.125f,  1.075f,      0.965f,  0.965f,  0.965f,     -0.782f, -0.112f,  0.614f,
    /*6*/   -0.757f,  0.000f,  1.500f,      0.827f,  0.831f,  0.824f,     -0.926f,  0.144f,  0.349f,
    /*7*/   -0.757f, -0.100f,  1.400f,      0.827f,  0.831f,  0.824f,     -0.873f, -0.360f,  0.328f,
    /*8*/   -0.757f,  0.000f,  1.075f,      0.827f,  0.831f,  0.824f,     -0.862f,  0.226f,  0.453f,
    /*9*/   -0.757f, -0.100f,  1.075f,      0.827f,  0.831f,  0.824f,     -0.834f, -0.075f,  0.547f,

            //knife guard
    /*10*/   -0.743f,  0.000f,  1.075f,      0.145f,  0.145f,  0.145f,     -0.823f,  0.315f,  0.473f,
    /*11*/   -0.743f, -0.100f,  1.075f,      0.145f,  0.145f,  0.145f,     -0.828f, -0.076f,  0.555f,
    /*12*/   -0.750f, -0.125f,  1.075f,      0.145f,  0.145f,  0.145f,     -0.782f, -0.112f,  0.614f,
    /*13*/   -0.757f,  0.000f,  1.075f,      0.145f,  0.145f,  0.145f,     -0.862f,  0.226f,  0.453f,
    /*14*/   -0.757f, -0.100f,  1.075f,      0.145f,  0.145f,  0.145f,     -0.834f, -0.075f,  0.547f,

    /*15*/   -0.725f,  0.025f,  1.075f,      0.145f,  0.145f,  0.145f,     -0.814f,  0.184f,  0.551f,
    /*16*/   -0.725f, -0.175f,  1.075f,      0.145f,  0.145f,  0.145f,     -0.761f, -0.205f,  0.615f,
    /*17*/   -0.775f,  0.025f,  1.075f,      0.145f,  0.145f,  0.145f,     -0.806f,  0.265f,  0.530f,
    /*18*/   -0.775f, -0.175f,  1.075f,      0.145f,  0.145f,  0.145f,     -0.781f, -0.347f,  0.520f,
    /*19*/   -0.725f,  0.025f,  1.000f,      0.075f,  0.075f,  0.075f,     -0.760f,  0.290f, -0.581f,
    /*20*/   -0.725f, -0.175f,  1.000f,      0.075f,  0.075f,  0.075f,     -0.820f, -0.317f, -0.476f,
    /*21*/   -0.775f,  0.025f,  1.000f,      0.075f,  0.075f,  0.075f,     -0.773f,  0.201f, -0.602f,
    /*22*/   -0.775f, -0.175f,  1.000f,      0.075f,  0.075f,  0.075f,     -0.860f, -0.228f, -0.457f,

            //knife handle
    /*23*/   -0.735f,  0.010f,  1.000f,      0.075f,  0.075f,  0.075f,     -0.850f,  0.381f, -0.362f,
    /*24*/   -0.735f, -0.100f,  1.000f,      0.075f,  0.075f,  0.075f,     -0.775f, -0.209f, -0.596f,
    /*25*/   -0.765f,  0.010f,  1.000f,      0.075f,  0.075f,  0.075f,     -0.770f,  0.205f, -0.604f,
    /*26*/   -0.765f, -0.100f,  1.000f,      0.075f,  0.075f,  0.075f,     -0.824f, -0.270f, -0.498f,

    /*27*/   -0.725f,  0.010f,  0.800f,      0.145f,  0.145f,  0.145f,     -0.851f,  0.381f, -0.362f,
    /*28*/   -0.725f, -0.125f,  0.800f,      0.145f,  0.145f,  0.145f,     -0.869f, -0.380f, -0.316f,
    /*29*/   -0.775f,  0.010f,  0.800f,      0.145f,  0.145f,  0.145f,     -0.807f,  0.428f, -0.407f,
    /*30*/   -0.775f, -0.125f,  0.800f,      0.145f,  0.145f,  0.145f,     -0.866f, -0.384f, -0.320f,

};

GLuint knifeIndices[] = {
    //knife blade
    0, 1, 2,
    2, 1, 3,
    3, 1, 4,
    3, 4, 5,

    6, 8, 7,
    7, 8, 9,
    7, 9, 4,
    4, 9, 5,

    0, 6, 1,
    1, 6, 7,
    1, 7, 4,
    0, 2, 6,
    6, 2, 8, 

    //knife guard
    17, 13, 10,
    17, 10, 15,
    17, 14, 13,
    17, 18, 14,
    12, 14, 18,
    12, 18, 16,
    11, 12, 16,
    11, 16, 15,
    11, 15, 10,

    15, 16, 20,
    15, 20, 19,
    15, 19, 17,
    17, 19, 21,
    17, 22, 18,
    17, 21, 22,
    18, 20, 16,
    18, 22, 20,
    19, 23, 25,
    19, 25, 21,
    21, 25, 26,
    21, 26, 22,
    20, 22, 26,
    20, 26, 24,
    19, 20, 24,
    19, 24, 23,

    //knife handle
    23, 24, 28,
    23, 28, 27,
    24, 26, 28,
    26, 30, 28,
    25, 29, 26,
    26, 29, 30,
    23, 27, 29,
    23, 29, 25,
    27, 28, 29,
    28, 30, 29,
};


float bgVertices[] {

            // position (x, y, z)                color (r, g, b)                
    /*0*/     -50.000f,  50.000f, -50.000f,      0.000f,  0.000f,  0.000f, 
    /*1*/     -50.000f, -50.000f, -50.000f,      0.000f,  0.000f,  0.000f, 
    /*2*/      50.000f,  50.000f, -50.000f,      0.000f,  0.000f,  0.000f,
    
    /*1*/     -50.000f, -50.000f, -50.000f,      0.000f,  0.000f,  0.000f, 
    /*3*/      50.000f, -50.000f, -50.000f,      0.000f,  0.000f,  0.000f,
    /*2*/      50.000f,  50.000f, -50.000f,      0.000f,  0.000f,  0.000f,

};


float bloodVertices[]{

           // position (x, y, z)           color (r, g, b)                normals (x, y, z)      
    /*0*/     0.049f, -0.300f, -0.500f,      1.000f,  1.000f,  1.000f,      0.289f, -0.856f, -0.428f,
    /*1*/     0.049f,  0.100f, -0.500f,      1.000f,  1.000f,  1.000f,     -0.391f,  0.051f, -0.919f,
    /*2*/     0.450f, -0.300f, -0.500f,      1.000f,  1.000f,  1.000f,     -0.517f, -0.383f, -0.766f,
    
    /*2*/     0.450f, -0.300f, -0.500f,      1.000f,  1.000f,  1.000f,     -0.517f, -0.383f, -0.766f,
    /*1*/     0.049f,  0.100f, -0.500f,      1.000f,  1.000f,  1.000f,     -0.391f,  0.051f, -0.919f,
    /*3*/     0.450f,  0.100f, -0.500f,      1.000f,  1.000f,  1.000f,      0.410f,  0.025f, -0.912f,

};


// define OpenGL object IDs to represent the vertex array and the shader program in the GPU
GLuint vaoBot, vaoTop, vaoKnife, vaoBG, vaoBlood;         // vertex array object (stores the render state for our vertex array)
GLuint vboBot, vboTop, vboKnife, vboBG, vboBlood;         // vertex buffer object (reserves GPU memory for our vertex array)
GLuint eboBot, eboTop, eboKnife;         // vertex buffer object (reserves GPU memory for our vertex array)
GLuint shader;      // combined vertex and fragment shader
GLuint texture1;
GLuint texture2;
GLuint texture3;
GLuint texture4;

// called by the main function to do initial setup, such as uploading vertex
// arrays, shader programs, etc.; returns true if successful, false otherwise
bool setup()
{
    //bot half   
    // generate the VAO and VBO objects and store their IDs in vao and vbo, respectively
    glGenVertexArrays(1, &vaoBot);
    glGenBuffers(1, &vboBot);
    glGenBuffers(1, &eboBot);

    // bind the newly-created VAO to make it the current one that OpenGL will apply state changes to
    glBindVertexArray(vaoBot);

    // upload our vertex array data to the newly-created VBO
    glBindBuffer(GL_ARRAY_BUFFER, vboBot);
    glBufferData(GL_ARRAY_BUFFER, sizeof(botHalfVertices), botHalfVertices, GL_STATIC_DRAW);

     //added ebo for uploading index array data to the newly create EBO
     glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboBot);
     glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(botHalfIndices), botHalfIndices, GL_STATIC_DRAW);
 
    // on the VAO, register the current VBO with the following vertex attribute layout:
    // - layout location 0...
    // - ... shall consist of 3 GL_FLOATs (corresponding to x, y, and z coordinates)
    // - ... its values will NOT be normalized (GL_FALSE)
    // - ... the stride length is the number of bytes of all 3 floats of each vertex (hence, 3 * sizeof(float))
    // - ... and we start at the beginning of the array (hence, (void*) 0)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*) 0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*) (3 * sizeof(float)));
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*) (6 * sizeof(float)));

    // enable the newly-created layout location 0;
    // this shall be used by our vertex shader to read the vertex's x, y, and z
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);


    //top half    
    glGenVertexArrays(1, &vaoTop);
    glGenBuffers(1, &vboTop);
    glGenBuffers(1, &eboTop);

    glBindVertexArray(vaoTop);

    glBindBuffer(GL_ARRAY_BUFFER, vboTop);
    glBufferData(GL_ARRAY_BUFFER, sizeof(topHalfVertices), topHalfVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboTop);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(topHalfIndices), topHalfIndices, GL_STATIC_DRAW);
 
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*) 0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*) (3 * sizeof(float)));
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*) (6 * sizeof(float)));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);


    //bg
    glGenVertexArrays(1, &vaoBG);
    glGenBuffers(1, &vboBG);

    glBindVertexArray(vaoBG);

    glBindBuffer(GL_ARRAY_BUFFER, vboBG);
    glBufferData(GL_ARRAY_BUFFER, sizeof(bgVertices), bgVertices, GL_STATIC_DRAW);
 
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) 0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) (3 * sizeof(float)));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);


    // knife
    glGenVertexArrays(1, &vaoKnife);
    glGenBuffers(1, &vboKnife);
    glGenBuffers(1, &eboKnife);

    glBindVertexArray(vaoKnife);

    glBindBuffer(GL_ARRAY_BUFFER, vboKnife);
    glBufferData(GL_ARRAY_BUFFER, sizeof(knifeVertices), knifeVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboKnife);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(knifeIndices), knifeIndices, GL_STATIC_DRAW);
 
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*) 0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*) (3 * sizeof(float)));
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*) (6 * sizeof(float)));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    // Blood
    glGenVertexArrays(1, &vaoBlood);
    glGenBuffers(1, &vboBlood);

    glBindVertexArray(vaoBlood);

    glBindBuffer(GL_ARRAY_BUFFER, vboBlood);
    glBufferData(GL_ARRAY_BUFFER, sizeof(bloodVertices), bloodVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*) 0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*) (3 * sizeof(float)));
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*) (6 * sizeof(float)));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
    

    // important: if you have more vertex arrays to draw, make sure you separately define them
    // with unique VAO and VBO IDs, and follow the same process above to upload them to the GPU


    // load our shader program
    shader = gdevLoadShader("FinalProject.vs", "FinalProject.fs");
    if (! shader)
        return false;
    texture1 = gdevLoadTexture("cloth.jpg", GL_REPEAT, true, true);
    if (! texture1) return false;
    texture2 = gdevLoadTexture("glass.png", GL_REPEAT, true, true);
    if (! texture2) return false;
    texture3 = gdevLoadTexture("stars.png", GL_REPEAT, true, true);
    if (! texture3) return false;
    texture4 = gdevLoadTexture("blood.png", GL_REPEAT, true, true);
    if (! texture4) return false;

    return true;
}

// called by the main function to do rendering per frame
void render()
{
    // clear the whole frame
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    float time = glfwGetTime();

    lightPosition = cameraPos + (cameraFront * 1.0f) + glm::vec3(0.0f, 1.0f, 0.0f); //light follows camera

    // using our shader program...
    glUseProgram(shader);
    glEnable(GL_DEPTH_TEST); // enable OpenGL's hidden surface removal
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, texture3);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, texture4);
    
    glUniform1i(glGetUniformLocation(shader, "texture1"), 0);
    glUniform1i(glGetUniformLocation(shader, "texture2"), 1);
    glUniform1i(glGetUniformLocation(shader, "texture3"), 2);
    glUniform1i(glGetUniformLocation(shader, "texture4"), 3);
    glUniform1f(glGetUniformLocation(shader, "time"), time);
    glUniform3fv(glGetUniformLocation(shader, "lightPosition"), 1, glm::value_ptr(lightPosition));
    glUniform3f(glGetUniformLocation(shader, "lightColor"), lightColor.x, lightColor.y, lightColor.z);
    glUniform3fv(glGetUniformLocation(shader, "cameraPos"), 1, glm::value_ptr(cameraPos));
    glUniform1f(glGetUniformLocation(shader, "specColor"), specularity);


    glm::mat4 projectionViewMatrix;
    projectionViewMatrix = glm::perspective(glm::radians(60.0f),
                             (float) WINDOW_WIDTH / WINDOW_HEIGHT,
                              0.1f, 100.0f);

    //camera
    glm::mat4 view;
    view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

    projectionViewMatrix *= view;

    
    glm::mat4 modelMatrix = glm::mat4(1.0f); // set to identity first!
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 0.0f, 0.0f));

    glm::mat4 normalMatrix;
    normalMatrix = glm::transpose(glm::inverse(modelMatrix));


    //bot half amogus
    glBindVertexArray(vaoBot);

    for (int i = 0; i < 2; i++) {
        if (i == 0) {
            modelMatrix = glm::mat4(1.0f); // red model
            glUniform1i(glGetUniformLocation(shader, "forceRed"), 1);
        } else {
            modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f, 0.0f, 3.0f)); // yellow model
            glUniform1i(glGetUniformLocation(shader, "forceRed"), 0);
        }

        normalMatrix = glm::transpose(glm::inverse(modelMatrix));

        glUniformMatrix4fv(glGetUniformLocation(shader, "projMatrix"), 1, GL_FALSE, glm::value_ptr(projectionViewMatrix));
        glUniformMatrix4fv(glGetUniformLocation(shader, "modMatrix"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
        glUniformMatrix4fv(glGetUniformLocation(shader, "norMatrix"), 1, GL_FALSE, glm::value_ptr(normalMatrix));

        glDrawElements(GL_TRIANGLES, sizeof(botHalfIndices) / sizeof(GLuint), GL_UNSIGNED_INT, 0);
    }


    //top half amogus
    glBindVertexArray(vaoTop);

    // render 2 top halves
    for (int i = 0; i < 2; i++) {
        if (i == 0) {
            modelMatrix = glm::mat4(1.0f); // red model
            glUniform1i(glGetUniformLocation(shader, "forceRed"), 1);
        } else {
            modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f, 0.0f, 3.0f)); // yellow model
            glUniform1i(glGetUniformLocation(shader, "forceRed"), 0);
        }

        normalMatrix = glm::transpose(glm::inverse(modelMatrix));

        glUniformMatrix4fv(glGetUniformLocation(shader, "projMatrix"), 1, GL_FALSE, glm::value_ptr(projectionViewMatrix));
        glUniformMatrix4fv(glGetUniformLocation(shader, "modMatrix"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
        glUniformMatrix4fv(glGetUniformLocation(shader, "norMatrix"), 1, GL_FALSE, glm::value_ptr(normalMatrix));

        glDrawElements(GL_TRIANGLES, sizeof(topHalfIndices) / sizeof(GLuint), GL_UNSIGNED_INT, 0);
    

    if (halfAppear != 1) break;
    }

    
    //knife
    glBindVertexArray(vaoKnife); 

    modelMatrix = glm::mat4(1.0f);
    normalMatrix = glm::transpose(glm::inverse(modelMatrix));

    if (isStabbing) {
        float elapsed = glfwGetTime() - animationStartTime;
        float offset;
        if (elapsed >= 3/2) {
            isStabbing = false;
            halfAppear = -1;
            offset = 0.0f;
            bloodCount = 0;
        } else {
            offset = abs(sin(elapsed * 2 * 3.14f)) * 1.45f;
            modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 0.0f, offset));
            if (offset > 1.445f)
                spawnBlood = true;
        }
    }

    glUniformMatrix4fv(glGetUniformLocation(shader, "modMatrix"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
    glUniformMatrix4fv(glGetUniformLocation(shader, "norMatrix"), 1, GL_FALSE, glm::value_ptr(normalMatrix));
    glBindVertexArray(vaoKnife);
    glDrawElements(GL_TRIANGLES, sizeof(knifeIndices) / sizeof(GLuint), GL_UNSIGNED_INT, 0);


    //bg
    glBindVertexArray(vaoBG);

    modelMatrix = glm::mat4(1.0f);
    for (int i = 0; i < 6; i++) {
        if (i < 4)  
            modelMatrix = glm::rotate(modelMatrix, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));                //rotate along the y-axis and get the first 4 sides of the cube      
        else
            modelMatrix = glm::rotate(modelMatrix, glm::radians(90.0f * (i - 3)), glm::vec3(1.0f, 0.0f, 0.0f));      //rotate along the x-axis and get the top and bottom sides of the cube

        glUniformMatrix4fv(glGetUniformLocation(shader, "modMatrix"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
        glUniformMatrix4fv(glGetUniformLocation(shader, "norMatrix"), 1, GL_FALSE, glm::value_ptr(normalMatrix));
        glDrawArrays(GL_TRIANGLES, 0, sizeof(bgVertices) / (3 * sizeof(float)));
    }


    //blood
    glBindVertexArray(vaoBlood);
    if (spawnBlood){     
        float scale = 0.5f + ((float)rand() / RAND_MAX) * 0.5f;
        float rotation = ((float)rand() / RAND_MAX) * 360.0f;

        modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(-1.0f, 0.0f, 2.999f));
        modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation), glm::vec3(0, 0, 1));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(scale, scale, 1));
        
        bloodMatrices[bloodCount++] = modelMatrix;
        spawnBlood = false;
    }

    if (isStabbing && halfAppear == 1) {
        for (int i = 0; i < bloodCount; i++) {

            normalMatrix = glm::transpose(glm::inverse(bloodMatrices[i]));
            glUniformMatrix4fv(glGetUniformLocation(shader, "projMatrix"), 1, GL_FALSE, glm::value_ptr(projectionViewMatrix));
            glUniformMatrix4fv(glGetUniformLocation(shader, "modMatrix"), 1, GL_FALSE, glm::value_ptr(bloodMatrices[i]));
            glUniformMatrix4fv(glGetUniformLocation(shader, "norMatrix"), 1, GL_FALSE, glm::value_ptr(normalMatrix));

            glDrawArrays(GL_TRIANGLES, 0, sizeof(bloodVertices) / (3 * sizeof(float)));
        }
    }
    
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
    if (glfwGetKey(window, GLFW_KEY_LEFT_BRACKET) == GLFW_PRESS)
        lightColor -= 0.05f;
    if (glfwGetKey(window, GLFW_KEY_RIGHT_BRACKET) == GLFW_PRESS)
        lightColor += 0.05f;
    if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS)
        specularity -= 0.05f;
    if (glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS)
        specularity += 0.05;

    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS && !fKeyPressed){
        fKeyPressed = true;
        if(halfAppear == 1){
            isStabbing = true;
            animationStartTime = glfwGetTime();
        }
        else{
            halfAppear = 1;
        }
    }

    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_RELEASE)
        fKeyPressed = false;
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