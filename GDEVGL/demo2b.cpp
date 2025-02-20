/******************************************************************************
 * This demo is a modification of demo2.cpp with a significant change:
 * - Instead of defining the same vertices multiple times for different
 *   triangles, we now define the triangles using vertex indices instead,
 *   thereby making more efficient use of GPU memory. The indices are
 *   uploaded to the GPU using an Element Buffer Object (EBO).
 *
 * Happy hacking! - eric
 *****************************************************************************/

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include <gdev.h>

// change this to your desired window attributes
#define WINDOW_WIDTH  750
#define WINDOW_HEIGHT 750
#define WINDOW_TITLE  "Hello Eikh"
GLFWwindow *pWindow;

// define a vertex array to hold our vertices
// CHANGED FOR DEMO2B: we're specifying each unique vertex only once now
float vertices[] =
{
        // position (x, y, z)       color (r, g, b)                 origin of rotation (x,y)      effects (glow, rotate)
        //                                                                                  0=noglow, 1=glow  |  0=no rot, 1=ctrclk, 2=clkwise
        //1st flower bud
/*0*/  -0.640f, 0.525f, 0.00f,      1.0f, 0.6196f, 0.5686f,         -0.380f, 0.290f,        0.00f, 1.00f,
/*1*/  -0.585f, 0.425f, 0.00f,      1.0f, 0.6196f, 0.5686f,         -0.380f, 0.290f,        0.00f, 1.00f,
/*2*/  -0.570f, 0.300f, 0.00f,      1.0f, 0.6196f, 0.5686f,         -0.380f, 0.290f,        0.00f, 1.00f,
/*3*/  -0.460f, 0.490f, 0.00f,      1.0f, 0.6196f, 0.5686f,         -0.380f, 0.290f,        0.00f, 1.00f,
/*4*/  -0.380f, 0.290f, 0.00f,      1.0f, 0.6196f, 0.5686f,         -0.380f, 0.290f,        0.00f, 1.00f,
/*5*/  -0.460f, 0.490f, 0.00f,      0.8863f, 0.4824f, 0.4275f,      -0.380f, 0.290f,        0.00f, 2.00f,
/*6*/  -0.380f, 0.290f, 0.00f,      0.8863f, 0.4824f, 0.4275f,      -0.380f, 0.290f,        0.00f, 2.00f,
/*7*/  -0.355f, 0.625f, 0.00f,      0.8863f, 0.4824f, 0.4275f,      -0.380f, 0.290f,        0.00f, 2.00f,
/*8*/  -0.350f, 0.530f, 0.00f,      0.8863f, 0.4824f, 0.4275f,      -0.380f, 0.290f,        0.00f, 2.00f,
/*9*/  -0.270f, 0.400f, 0.00f,      0.8863f, 0.4824f, 0.4275f,      -0.380f, 0.290f,        0.00f, 2.00f,
/*10*/ -0.585f, 0.425f, 0.00f,      0.8f, 0.4157f, 0.3608f,          0.000f, 0.000f,        1.00f, 0.00f,
/*11*/ -0.350f, 0.530f, 0.00f,      0.8f, 0.4157f, 0.3608f,          0.000f, 0.000f,        1.00f, 0.00f,
/*12*/ -0.500f, 0.580f, 0.00f,      0.8f, 0.4157f, 0.3608f,          0.000f, 0.000f,        1.00f, 0.00f,

    //2nd flower bud
/*13*/ -0.270f, 0.540f, 0.00f,      1.0f, 0.6196f, 0.5686f,         0.000f, 0.000f,         1.00f, 0.00f,
/*14*/ -0.260f, 0.370f, 0.00f,      1.0f, 0.6196f, 0.5686f,         0.000f, 0.000f,         1.00f, 0.00f,         
/*15*/ -0.210f, 0.615f, 0.00f,      1.0f, 0.6196f, 0.5686f,         0.000f, 0.000f,         1.00f, 0.00f,
/*16*/ -0.125f, 0.585f, 0.00f,      1.0f, 0.6196f, 0.5686f,         0.000f, 0.000f,         1.00f, 0.00f,
/*17*/ -0.075f, 0.420f, 0.00f,      1.0f, 0.6196f, 0.5686f,         0.000f, 0.000f,         1.00f, 0.00f,  
/*18*/ -0.295f, 0.580f, 0.00f,      0.8863f, 0.4824f, 0.4275f,      0.000f, 0.000f,         1.00f, 0.00f,
/*19*/ -0.270f, 0.540f, 0.00f,      0.8863f, 0.4824f, 0.4275f,      0.000f, 0.000f,         1.00f, 0.00f,
/*20*/ -0.260f, 0.370f, 0.00f,      0.8863f, 0.4824f, 0.4275f,      0.000f, 0.000f,         1.00f, 0.00f,
/*21*/ -0.120f, 0.340f, 0.00f,      0.8863f, 0.4824f, 0.4275f,      0.000f, 0.000f,         1.00f, 0.00f,
/*22*/ -0.125f, 0.585f, 0.00f,      0.8863f, 0.4824f, 0.4275f,      0.000f, 0.000f,         1.00f, 0.00f,
/*23*/ -0.075f, 0.420f, 0.00f,      0.8863f, 0.4824f, 0.4275f,      0.000f, 0.000f,         1.00f, 0.00f,
/*24*/ -0.050f, 0.650f, 0.00f,      0.8863f, 0.4824f, 0.4275f,      0.000f, 0.000f,         1.00f, 0.00f,
/*25*/ -0.050f, 0.590f, 0.00f,      0.8863f, 0.4824f, 0.4275f,      0.000f, 0.000f,         1.00f, 0.00f, 
/*26*/  0.025f, 0.450f, 0.00f,      0.8863f, 0.4824f, 0.4275f,      0.000f, 0.000f,         1.00f, 0.00f, 
/*27*/  0.025f, 0.450f, 0.00f,      0.8f, 0.4157f, 0.3608f,         0.000f, 0.000f,         1.00f, 0.00f,
/*28*/ -0.075f, 0.420f, 0.00f,      0.8f, 0.4157f, 0.3608f,         0.000f, 0.000f,         1.00f, 0.00f,
/*29*/ -0.120f, 0.340f, 0.00f,      0.8f, 0.4157f, 0.3608f,         0.000f, 0.000f,         1.00f, 0.00f,

    //3rd flower bud
/*30*/  0.150f, 0.790f, 0.00f,      1.0f, 0.6196f, 0.5686f,         0.200f, 0.445f,         0.00f, 1.00f,
/*31*/  0.145f, 0.730f, 0.00f,      1.0f, 0.6196f, 0.5686f,         0.200f, 0.445f,         0.00f, 1.00f,
/*32*/  0.060f, 0.540f, 0.00f,      1.0f, 0.6196f, 0.5686f,         0.200f, 0.445f,         0.00f, 1.00f,
/*33*/  0.265f, 0.660f, 0.00f,      1.0f, 0.6196f, 0.5686f,         0.200f, 0.445f,         0.00f, 1.00f,
/*34*/  0.200f, 0.445f, 0.00f,      1.0f, 0.6196f, 0.5686f,         0.200f, 0.445f,         0.00f, 1.00f,
/*35*/  0.265f, 0.660f, 0.00f,      0.8863f, 0.4824f, 0.4275f,      0.200f, 0.445f,         0.00f, 2.00f,
/*36*/  0.200f, 0.445f, 0.00f,      0.8863f, 0.4824f, 0.4275f,      0.200f, 0.445f,         0.00f, 2.00f,
/*37*/  0.440f, 0.710f, 0.00f,      0.8863f, 0.4824f, 0.4275f,      0.200f, 0.445f,         0.00f, 2.00f,
/*38*/  0.400f, 0.650f, 0.00f,      0.8863f, 0.4824f, 0.4275f,      0.200f, 0.445f,         0.00f, 2.00f,
/*39*/  0.370f, 0.440f, 0.00f,      0.8863f, 0.4824f, 0.4275f,      0.200f, 0.445f,         0.00f, 2.00f,
/*40*/  0.295f, 0.750f, 0.00f,      0.8f, 0.4157f, 0.3608f,         0.000f, 0.000f,         1.00f, 0.00f,
/*41*/  0.150f, 0.660f, 0.00f,      0.8f, 0.4157f, 0.3608f,         0.000f, 0.000f,         1.00f, 0.00f,
/*42*/  0.330f, 0.655f, 0.00f,      0.8f, 0.4157f, 0.3608f,         0.000f, 0.000f,         1.00f, 0.00f,

    //4th flower bud
/*43*/  0.450f, 0.535f, 0.00f,      1.0f, 0.6196f, 0.5686f,         0.000f, 0.000f,         1.00f, 0.00f,
/*44*/  0.350f, 0.390f, 0.00f,      1.0f, 0.6196f, 0.5686f,         0.000f, 0.000f,         1.00f, 0.00f,
/*45*/  0.595f, 0.570f, 0.00f,      1.0f, 0.6196f, 0.5686f,         0.000f, 0.000f,         1.00f, 0.00f,
/*46*/  0.650f, 0.440f, 0.00f,      1.0f, 0.6196f, 0.5686f,         0.000f, 0.000f,         1.00f, 0.00f,
/*47*/  0.590f, 0.270f, 0.00f,      1.0f, 0.6196f, 0.5686f,         0.000f, 0.000f,         1.00f, 0.00f,
/*48*/  0.460f, 0.600f, 0.00f,      0.8863f, 0.4824f, 0.4275f,      0.000f, 0.000f,         1.00f, 0.00f,
/*49*/  0.450f, 0.535f, 0.00f,      0.8863f, 0.4824f, 0.4275f,      0.000f, 0.000f,         1.00f, 0.00f,
/*50*/  0.520f, 0.550f, 0.00f,      0.8863f, 0.4824f, 0.4275f,      0.000f, 0.000f,         1.00f, 0.00f,
/*51*/  0.350f, 0.390f, 0.00f,      0.8863f, 0.4824f, 0.4275f,      0.000f, 0.000f,         1.00f, 0.00f,
/*52*/  0.440f, 0.270f, 0.00f,      0.8863f, 0.4824f, 0.4275f,      0.000f, 0.000f,         1.00f, 0.00f,
/*53*/  0.590f, 0.270f, 0.00f,      0.8863f, 0.4824f, 0.4275f,      0.000f, 0.000f,         1.00f, 0.00f,
/*54*/  0.620f, 0.510f, 0.00f,      0.8863f, 0.4824f, 0.4275f,      0.000f, 0.000f,         1.00f, 0.00f,
/*55*/  0.650f, 0.440f, 0.00f,      0.8863f, 0.4824f, 0.4275f,      0.000f, 0.000f,         1.00f, 0.00f,
/*56*/  0.700f, 0.495f, 0.00f,      0.8863f, 0.4824f, 0.4275f,      0.000f, 0.000f,         1.00f, 0.00f,

    //1st & 2nd leaf
/*57*/ -0.610f, 0.190f, 0.00f,      0.6706f, 0.7373f, 0.5412f,      -0.235f,-0.070f,        0.00f, 2.00f,
/*58*/ -0.500f, 0.020f, 0.00f,      0.6706f, 0.7373f, 0.5412f,      -0.235f,-0.070f,        0.00f, 2.00f,
/*59*/ -0.430f, 0.150f, 0.00f,      0.6706f, 0.7373f, 0.5412f,      -0.235f,-0.070f,        0.00f, 2.00f,
/*60*/ -0.380f, 0.060f, 0.00f,      0.6706f, 0.7373f, 0.5412f,      -0.235f,-0.070f,        0.00f, 2.00f,
/*61*/ -0.500f, 0.020f, 0.00f,      0.5176f, 0.6431f, 0.549f,       -0.235f,-0.070f,        0.00f, 2.00f,
/*62*/ -0.380f, 0.060f, 0.00f,      0.5176f, 0.6431f, 0.549f,       -0.235f,-0.070f,        0.00f, 2.00f,
/*63*/ -0.235f,-0.070f, 0.00f,      0.5176f, 0.6431f, 0.549f,       -0.235f,-0.070f,        0.00f, 2.00f,
/*64*/ -0.430f, 0.150f, 0.00f,      0.4667f, 0.6118f, 0.5412f,      -0.235f,-0.070f,        0.00f, 2.00f,
/*65*/ -0.380f, 0.060f, 0.00f,      0.4667f, 0.6118f, 0.5412f,      -0.235f,-0.070f,        0.00f, 2.00f,
/*66*/ -0.300f, 0.030f, 0.00f,      0.4667f, 0.6118f, 0.5412f,      0.000f, 0.000f,         0.00f, 0.00f,
/*67*/ -0.235f,-0.070f, 0.00f,      0.4667f, 0.6118f, 0.5412f,      0.000f, 0.000f,         0.00f, 0.00f,
/*68*/ -0.245f, 0.190f, 0.00f,      0.4667f, 0.6118f, 0.5412f,      0.000f, 0.000f,         0.00f, 0.00f,
/*69*/ -0.090f,-0.100f, 0.00f,      0.4667f, 0.6118f, 0.5412f,      0.000f, 0.000f,         0.00f, 0.00f,

    //3rd leaf
/*70*/ -0.170f, 0.460f, 0.00f,      0.6706f, 0.7373f, 0.5412f,      0.040f,-0.180f,         0.00f, 1.00f, 
/*71*/ -0.160f, 0.105f, 0.00f,      0.6706f, 0.7373f, 0.5412f,      0.040f,-0.180f,         0.00f, 1.00f,
/*72*/ -0.130f, 0.130f, 0.00f,      0.6706f, 0.7373f, 0.5412f,      0.040f,-0.180f,         0.00f, 1.00f,
/*73*/  0.040f,-0.180f, 0.00f,      0.6706f, 0.7373f, 0.5412f,      0.040f,-0.180f,         0.00f, 1.00f,
/*74*/ -0.170f, 0.460f, 0.00f,      0.5176f, 0.6431f, 0.549f,       0.040f,-0.180f,         0.00f, 1.00f,
/*75*/ -0.010f, 0.220f, 0.00f,      0.5176f, 0.6431f, 0.549f,       0.040f,-0.180f,         0.00f, 1.00f,
/*76*/  0.040f,-0.180f, 0.00f,      0.5176f, 0.6431f, 0.549f,       0.040f,-0.180f,         0.00f, 1.00f,
/*77*/  0.045f,-0.060f, 0.00f,      0.5176f, 0.6431f, 0.549f,       0.040f,-0.180f,         0.00f, 1.00f,
/*78*/ -0.170f, 0.460f, 0.00f,      0.4667f, 0.6118f, 0.5412f,      0.040f,-0.180f,         0.00f, 1.00f,
/*79*/ -0.130f, 0.130f, 0.00f,      0.4667f, 0.6118f, 0.5412f,      0.040f,-0.180f,         0.00f, 1.00f,
/*80*/ -0.070f, 0.200f, 0.00f,      0.4667f, 0.6118f, 0.5412f,      0.040f,-0.180f,         0.00f, 1.00f,
/*81*/  0.040f,-0.180f, 0.00f,      0.4667f, 0.6118f, 0.5412f,      0.040f,-0.180f,         0.00f, 1.00f,

    //4th leaf
/*82*/ 0.045f,-0.060f, 0.00f,       0.6706f, 0.7373f, 0.5412f,      0.000f,-0.250f,         0.00f, 2.00f,
/*83*/ 0.190f, 0.180f, 0.00f,       0.6706f, 0.7373f, 0.5412f,      0.000f,-0.250f,         0.00f, 2.00f,
/*84*/ 0.220f, 0.140f, 0.00f,       0.6706f, 0.7373f, 0.5412f,      0.000f,-0.250f,         0.00f, 2.00f,
/*85*/ 0.390f, 0.310f, 0.00f,       0.6706f, 0.7373f, 0.5412f,      0.000f,-0.250f,         0.00f, 2.00f,
/*86*/ 0.000f,-0.250f, 0.00f,       0.5176f, 0.6431f, 0.549f,       0.000f,-0.250f,         0.00f, 2.00f,
/*87*/ 0.220f, 0.140f, 0.00f,       0.5176f, 0.6431f, 0.549f,       0.000f,-0.250f,         0.00f, 2.00f,
/*88*/ 0.340f, 0.060f, 0.00f,       0.5176f, 0.6431f, 0.549f,       0.000f,-0.250f,         0.00f, 2.00f,
/*89*/ 0.220f, 0.140f, 0.00f,       0.4667f, 0.6118f, 0.5412f,      0.000f,-0.250f,         0.00f, 2.00f,
/*90*/ 0.340f, 0.060f, 0.00f,       0.4667f, 0.6118f, 0.5412f,      0.000f,-0.250f,         0.00f, 2.00f,
/*91*/ 0.390f, 0.310f, 0.00f,       0.4667f, 0.6118f, 0.5412f,      0.000f,-0.250f,         0.00f, 2.00f,

    //5th leaf
/*92*/ 0.250f,-0.150f, 0.00f,       0.6706f, 0.7373f, 0.5412f,      0.250f,-0.150f,         0.00f, 2.00f,
/*93*/ 0.500f, 0.200f, 0.00f,       0.6706f, 0.7373f, 0.5412f,      0.250f,-0.150f,         0.00f, 2.00f,
/*94*/ 0.560f, 0.125f, 0.00f,       0.6706f, 0.7373f, 0.5412f,      0.250f,-0.150f,         0.00f, 2.00f,
/*95*/ 0.500f, 0.200f, 0.00f,       0.4667f, 0.6118f, 0.5412f,      0.250f,-0.150f,         0.00f, 2.00f,
/*96*/ 0.560f, 0.125f, 0.00f,       0.4667f, 0.6118f, 0.5412f,      0.250f,-0.150f,         0.00f, 2.00f,
/*97*/ 0.800f, 0.140f, 0.00f,       0.4667f, 0.6118f, 0.5412f,      0.250f,-0.150f,         0.00f, 2.00f,

    //1st stem
/*98*/-0.420f, 0.300f, 0.00f,       0.4667f, 0.6118f, 0.5412f,      0.000f, 0.000f,         0.00f, 0.00f,
/*99*/-0.370f, 0.300f, 0.00f,       0.4667f, 0.6118f, 0.5412f,      0.000f, 0.000f,         0.00f, 0.00f,
/*100*/-0.40f, 0.260f, 0.00f,       0.4667f, 0.6118f, 0.5412f,      0.000f, 0.000f,         0.00f, 0.00f,
/*101*/-0.37f, 0.300f, 0.00f,       0.7373f, 0.7725f, 0.5882f,      0.000f, 0.000f,         0.00f, 0.00f,
/*102*/-0.40f, 0.260f, 0.00f,       0.7373f, 0.7725f, 0.5882f,      0.000f, 0.000f,         0.00f, 0.00f,
/*103*/-0.20f,-0.100f, 0.00f,       0.7373f, 0.7725f, 0.5882f,      0.000f, 0.000f,         0.00f, 0.00f,
/*104*/-.255f,-0.100f, 0.00f,       0.7373f, 0.7725f, 0.5882f,      0.000f, 0.000f,         0.00f, 0.00f,

    //2nd stem
/*105*/-0.14f, 0.400f, 0.00f,       0.4667f, 0.6118f, 0.5412f,      0.000f, 0.000f,         0.00f, 0.00f,
/*106*/-0.10f, 0.400f, 0.00f,       0.4667f, 0.6118f, 0.5412f,      0.000f, 0.000f,         0.00f, 0.00f,
/*107*/-0.04f,-0.200f, 0.00f,       0.4667f, 0.6118f, 0.5412f,      0.000f, 0.000f,         0.00f, 0.00f,
/*108*/ 0.00f,-0.160f, 0.00f,       0.4667f, 0.6118f, 0.5412f,      0.000f, 0.000f,         0.00f, 0.00f,

    //3rd stem
/*109*/ 0.19f, 0.500f, 0.00f,       0.4667f, 0.6118f, 0.5412f,      0.000f, 0.000f,         0.00f, 0.00f,
/*110*/ 0.22f, 0.445f, 0.00f,       0.4667f, 0.6118f, 0.5412f,      0.000f, 0.000f,         0.00f, 0.00f,
/*111*/ 0.16f, 0.420f, 0.00f,       0.4667f, 0.6118f, 0.5412f,      0.000f, 0.000f,         0.00f, 0.00f,
/*112*/ 0.22f, 0.445f, 0.00f,       0.7373f, 0.7725f, 0.5882f,      0.000f, 0.000f,         0.00f, 0.00f,
/*113*/ 0.16f, 0.420f, 0.00f,       0.7373f, 0.7725f, 0.5882f,      0.000f, 0.000f,         0.00f, 0.00f,
/*114*/ 0.00f,-0.130f, 0.00f,       0.7373f, 0.7725f, 0.5882f,      0.000f, 0.000f,         0.00f, 0.00f,
/*115*/ .055f,-0.130f, 0.00f,       0.7373f, 0.7725f, 0.5882f,      0.000f, 0.000f,         0.00f, 0.00f,

    //4th stem
/*116*/ 0.43f, 0.300f, 0.00f,       0.4667f, 0.6118f, 0.5412f,      0.000f, 0.000f,         0.00f, 0.00f,
/*117*/ 0.47f, 0.270f, 0.00f,       0.4667f, 0.6118f, 0.5412f,      0.000f, 0.000f,         0.00f, 0.00f,
/*118*/ .405f, 0.250f, 0.00f,       0.4667f, 0.6118f, 0.5412f,      0.000f, 0.000f,         0.00f, 0.00f,
/*119*/ 0.47f, 0.270f, 0.00f,       0.7373f, 0.7725f, 0.5882f,      0.000f, 0.000f,         0.00f, 0.00f,
/*120*/ .405f, 0.250f, 0.00f,       0.7373f, 0.7725f, 0.5882f,      0.000f, 0.000f,         0.00f, 0.00f,
/*121*/ .235f,-0.200f, 0.00f,       0.7373f, 0.7725f, 0.5882f,      0.000f, 0.000f,         0.00f, 0.00f,
/*122*/ 0.20f,-0.160f, 0.00f,       0.7373f, 0.7725f, 0.5882f,      0.000f, 0.000f,         0.00f, 0.00f,      

    //box
/*123*/-0.50f, 0.020f, 0.00f,       0.5647f, 0.3294f, 0.2902f,      0.000f, 0.000f,         0.00f, 0.00f,
/*124*/-.185f, 0.080f, 0.00f,       0.5647f, 0.3294f, 0.2902f,      0.000f, 0.000f,         0.00f, 0.00f, 
/*125*/ 0.13f,-0.150f, 0.00f,       0.5647f, 0.3294f, 0.2902f,      0.000f, 0.000f,         0.00f, 0.00f, 
/*126*/ 0.47f,-0.010f, 0.00f,       0.5647f, 0.3294f, 0.2902f,      0.000f, 0.000f,         0.00f, 0.00f,
/*127*/-0.50f, 0.020f, 0.00f,       0.7804f, 0.5216f, 0.4275f,      0.000f, 0.000f,         0.00f, 0.00f,
/*128*/ 0.13f,-0.150f, 0.00f,       0.7804f, 0.5216f, 0.4275f,      0.000f, 0.000f,         0.00f, 0.00f,
/*129*/-0.50f,-0.560f, 0.00f,       0.8275f, 0.5882f, 0.4784f,      0.000f, 0.000f,         0.00f, 0.00f,//0.7804f, 0.5216f, 0.4275f, 
/*130*/ 0.13f,-0.800f, 0.00f,       0.8275f, 0.5882f, 0.4784f,      0.000f, 0.000f,         0.00f, 0.00f,//0.7804f, 0.5216f, 0.4275f, 
/*131*/-0.50f, 0.020f, 0.00f,       0.6510f, 0.3843f, 0.3412f,      0.000f, 0.000f,         0.00f, 0.00f,
/*132*/ 0.13f,-0.150f, 0.00f,       0.6510f, 0.3843f, 0.3412f,      0.000f, 0.000f,         0.00f, 0.00f,
/*133*/-0.50f,-0.350f, 0.00f,       0.7804f, 0.5216f, 0.4275f,      0.000f, 0.000f,         0.00f, 0.00f,//0.651f, 0.3843f, 0.3412f, 
/*134*/ .055f,-0.550f, 0.00f,       0.7804f, 0.5216f, 0.4275f,      0.000f, 0.000f,         0.00f, 0.00f,//0.651f, 0.3843f, 0.3412f,  
/*135*/-0.50f, 0.020f, 0.00f,       0.8275f, 0.5882f, 0.4784f,      0.000f, 0.000f,         0.00f, 0.00f,
/*136*/ 0.13f,-0.150f, 0.00f,       0.8275f, 0.5882f, 0.4784f,      0.000f, 0.000f,         0.00f, 0.00f,
/*137*/-0.61f,-0.210f, 0.00f,       0.8275f, 0.5882f, 0.4784f,      0.000f, 0.000f,         0.00f, 0.00f,
/*138*/ 0.01f,-0.430f, 0.00f,       0.8275f, 0.5882f, 0.4784f,      0.000f, 0.000f,         0.00f, 0.00f,
/*139*/ 0.13f,-0.800f, 0.00f,       0.7804f, 0.5216f, 0.4275f,      0.000f, 0.000f,         0.00f, 0.00f,//0.651f, 0.3843f, 0.3412f,
/*140*/ 0.47f,-0.010f, 0.00f,       0.6510f, 0.3843f, 0.3412f,      0.000f, 0.000f,         0.00f, 0.00f,   
/*141*/ 0.47f,-0.530f, 0.00f,       0.7804f, 0.5216f, 0.4275f,      0.000f, 0.000f,         0.00f, 0.00f,//0.651f, 0.3843f, 0.3412f,
/*142*/ 0.28f,-0.390f, 0.00f,       0.7804f, 0.5216f, 0.4275f,      0.000f, 0.000f,         0.00f, 0.00f,
/*143*/ 0.47f,-0.010f, 0.00f,       0.7804f, 0.5216f, 0.4275f,      0.000f, 0.000f,         0.00f, 0.00f,
/*144*/ 0.57f,-0.180f, 0.00f,       0.7804f, 0.5216f, 0.4275f,      0.000f, 0.000f,         0.00f, 0.00f,

};

// ADDED FOR DEMO2B:
// define the triangles as triplets of indices into the vertex array
GLuint indices[] =
{
    //back of the box
    123, 124, 125,
    124, 125, 126,

    //first & second leaf
    57, 58, 59,
    58, 59, 60,
    61, 62, 63,
    64, 65, 66,
    65, 66, 67,
    67, 68, 69,

    //first stem
    98, 99, 100,
    101, 102, 103,
    102, 103, 104,

    //first flower bud
    10, 11, 6,
    10, 11, 12,
    0,  1,  3,
    1,  2,  4,
    1,  3,  4,
    5,  6,  9,
    5,  8,  9,
    5,  7,  8,
     
    
    //second stem
    105, 106, 107,
    106, 107, 108,

    //second flower bud
    18, 19, 26,
    13, 15, 16,
    13, 14, 17,
    13, 16, 17,
    20, 21, 23,
    22, 24, 25,
    22, 23, 25,
    23, 25, 26,
    27, 28, 29,

    //third leaf
    70, 71, 72,
    71, 72, 73,
    74, 75, 76,
    75, 76, 77,
    78, 79, 80,
    79, 80, 81,

    //fourth leaf
    86, 82, 84,
    82, 83, 84,
    83, 84, 85,
    86, 87, 88,
    89, 90, 91,

    //third stem
    109, 110, 111,
    112, 113, 114,
    112, 114, 115,
    
    //third flower bud
    40, 41, 42,
    41, 42, 36,
    30, 31, 33,
    31, 32, 34,
    31, 33, 34,
    35, 36, 38,
    35, 37, 38,
    36, 38, 39,

    //fourth stem
    116, 117, 118,
    119, 120, 121,
    120, 121, 122,

    //fourth flower bud
    48, 49, 50,
    51, 52, 53,
    54, 55, 56,
    43, 44, 46,
    43, 45, 46,
    44, 46, 47,

    //fifth leaf
    92, 93, 94,
    95, 96, 97,

    //box front
    127, 128, 129,
    128, 129, 130,
    //shadow
    131, 132, 133,
    132, 133, 134,
    //box front flap
    135, 136, 137,
    136, 137, 138,

    //box side
    132, 139, 140,
    139, 140, 141,
    //box side flap
    128, 142, 143,
    142, 143, 144

};

// define OpenGL object IDs to represent the vertex array and the shader program in the GPU
GLuint vao;         // vertex array object (stores the render state for our vertex array)
GLuint vbo;         // vertex buffer object (reserves GPU memory for our vertex array)
GLuint ebo;         // ADDED FOR DEMO2B: element buffer object (stores the indices of the vertices to form triangles)
GLuint shader;      // combined vertex and fragment shader

// called by the main function to do initial setup, such as uploading vertex
// arrays, shader programs, etc.; returns true if successful, false otherwise
bool setup()
{
    // generate the VAO, VBO, and EBO objects and store their IDs in vao, vbo, and ebo, respectively
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);   // ADDED FOR DEMO2B

    // bind the newly-created VAO to make it the current one that OpenGL will apply state changes to
    glBindVertexArray(vao);

    // upload our vertex array data to the newly-created VBO
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // ADDED FOR DEMO2B:
    // upload our index array data to the newly-created EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // on the VAO, register the current VBO with the following vertex attribute layout:
    // - the stride length of the vertex array is 6 floats (6 * sizeof(float))
    // - layout location 0 (position) is 3 floats and starts at the first float of the vertex array (offset 0)
    // - layout location 1 (color) is also 3 floats but starts at the fourth float (offset 3 * sizeof(float))
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*) 0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*) (3 * sizeof(float)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*) (6 * sizeof(float)));
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*) (8 * sizeof(float)));

    // enable the layout locations so they can be used by the vertex shader
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);

    // important: if you have more vertex arrays to draw, make sure you separately define them
    // with unique VAO, VBO, and EBO IDs, and follow the same process above to upload them to the GPU

    // load our shader program
    shader = gdevLoadShader("demo2.vs", "demo2.fs");
    if (! shader)
        return false;

    return true;
}

// called by the main function to do rendering per frame
void render()
{
    // clear the whole frame
    glClearColor(0.0f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // compute a value for the glow amount for this frame
    float glow = (fabs(sin(glfwGetTime() * 1.75f)) / 1.25f)*-1 + 1.65f;
    float time = glfwGetTime();
    
    // using our shader program...
    glUseProgram(shader);

    // ... set the uniform variables of the shader...
    // (in this case, simply set the value of the glow)
    glUniform1f(glGetUniformLocation(shader, "glow"), glow);
    glUniform1f(glGetUniformLocation(shader,"time"), time);

    // ... then draw our triangles
    glBindVertexArray(vao);
    // CHANGED FOR DEMO2B (from glDrawArrays to glDrawElements)
    glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(GLuint), GL_UNSIGNED_INT, 0);
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
