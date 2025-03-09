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
#define WINDOW_HEIGHT 640
#define WINDOW_TITLE  "Hello Explosion"
GLFWwindow *pWindow;

// define a vertex array to hold our vertices
float vertices[] =
{
    // position (x, y, z)       color (r, g, b)     texture coordinates (s, t)      piece (u)
    //left gray side (poly2)
/*0,A*/   -1.00f, -1.00f, 0.00f,        1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,
/*1,I*/   -0.504f, -1.00f, 0.00f,       1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,   
/*2,J*/   -0.504f, 0.712f, 0.00f,       1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,
/*3,H*/   -1.00f, 1.00f, 0.00f,         1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,
/*4,G*/   -0.20f, 1.00f, 0.00f,         1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,

    //right gray side (poly1)
/*5,C*/   1.00f, 1.00f, 0.00f,          1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,
/*6,D*/   0.20f, 1.00f, 0.00f,          1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,
/*7,E*/   0.504f, 0.712f, 0.00f,        1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,
/*8,B*/   1.00f, -1.00f, 0.00f,         1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,
/*9,F*/   0.504f, -1.00f, 0.00f,        1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,

    //top right piece (poly3), reuses 6,D and 7,E
/*10,K*/  0.004f, 1.00f, 0.00f,         1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,
/*11,L*/  0.003f, 0.95f, 0.00f,         1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,
/*12,M*/  0.072f, 0.753f, 0.00f,        1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,
/*13,N*/  0.107f, 0.739f, 0.00f,        1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,
/*14,O*/  0.299f, 0.828f, 0.00f,        1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,
/*15,P*/  0.192f, 0.649f, 0.00f,        1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,
/*16,Q*/  0.206f, 0.616f, 0.00f,        1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,
/*17,R*/  0.442f, 0.535f, 0.00f,        1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,
/*18,S*/  0.504f, 0.535f, 0.00f,        1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,
/*19,D*/  0.20f, 1.00f, 0.00f,          1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,
/*20,E*/  0.504f, 0.712f, 0.00f,        1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,

    //middle top right piece (poly4), reuses 17,R and 18,S
/*21,R*/  0.442f, 0.535f, 0.00f,        1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,
/*22,S*/  0.504f, 0.535f, 0.00f,        1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,
/*23,T*/  0.205f, 0.484f, 0.00f,        1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,
/*24,U*/  0.186f, 0.450f, 0.00f,        1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,
/*25,V*/  0.240f, 0.296f, 0.00f,        1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,
/*26,W*/  0.504f, 0.043f, 0.00f,        1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,

    //middle bottom right piece (poly5), reuses 21,V and 22,W
/*27,Z*/  0.106f, 0.366f, 0.00f,        1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,
/*28,A1*/ 0.070f, 0.349f, 0.00f,        1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,
/*29,B1*/ 0.005f, 0.082f, 0.00f,        1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,
/*30,C1*/ 0.135f, -0.186f, 0.00f,       1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,
/*31,D1*/ 0.305f, -0.027f, 0.00f,       1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,
/*32,E1*/ 0.504f, -0.435f, 0.00f,       1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,
/*33,V*/  0.240f, 0.296f, 0.00f,        1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,
/*34,W*/  0.504f, 0.043f, 0.00f,        1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,
/*35,F1*/ 0.435f, -0.292f, 0.00f,       1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,

    //middle piece with diamond (poly6), reuses 25,B1 26,C1 and 27,D1
/*36,F1*/ 0.435f, -0.292f, 0.00f,       1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,
/*37,G1*/ 0.136f, -0.685f, 0.00f,       1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,
/*38,H1*/ 0.048f, -0.541f, 0.00f,       1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,
/*39,I1*/ 0.005f, -0.628f, 0.00f,       1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,
/*40,J1*/ 0.006f, -0.301f, 0.00f,       1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,
/*41,K1*/ -0.049f, -0.179f, 0.00f,      1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,
/*42,L1*/ 0.004f, -0.054f, 0.00f,       1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,
/*43,B1*/ 0.005f, 0.082f, 0.00f,        1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,
/*44,C1*/ 0.135f, -0.186f, 0.00f,       1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,
/*45,D1*/ 0.305f, -0.027f, 0.00f,       1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,

    //bottom middle piece (poly7), reuses 30,G1 31,H1 32,I1
/*46,M1*/ 0.006f, -0.508f, 0.00f,       1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,
/*47,N1*/ -0.186f, -0.558f, 0.00f,      1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,
/*48,O1*/ -0.093f, -0.728f, 0.00f,      1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,
/*49,P1*/ -0.074f, -0.694f, 0.00f,      1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,
/*50,Q1*/ 0.004f, -0.806f, 0.00f,       1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,
/*51,R1*/ 0.004f, -1.00f, 0.00f,        1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,
/*52,S1*/ 0.105f, -1.00f, 0.00f,        1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,
/*53,T1*/ 0.181f, -0.856f, 0.00f,       1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,
/*54,G1*/ 0.136f, -0.685f, 0.00f,       1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,
/*55,H1*/ 0.048f, -0.541f, 0.00f,       1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,
/*56,I1*/ 0.005f, -0.628f, 0.00f,       1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,

    //bottom right piece (poly8), reuses 28,E1 29,F1 30,G1 43,T1 42,S1 9,F
/*57,E1*/ 0.504f, -0.435f, 0.00f,       1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,
/*58,F1*/ 0.435f, -0.292f, 0.00f,       1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,
/*59,G1*/ 0.136f, -0.685f, 0.00f,       1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,
/*60,S1*/ 0.105f, -1.00f, 0.00f,        1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,
/*61,T1*/ 0.181f, -0.856f, 0.00f,       1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,
/*62,F*/  0.504f, -1.00f, 0.00f,        1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,


    //bottom left piece (poly9), reuses 41,R1 40,Q1 39,P1 38,O1
/*63,U1*/ -0.187f, -0.94f, 0.00f,       1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,
/*64,V1*/ -0.325f, -0.684f, 0.00f,      1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,
/*65,W1*/ -0.332f, -0.803f, 0.00f,      1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,
/*66,Z1*/ -0.501f, -0.646f, 0.00f,      1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,
/*67,O1*/ -0.093f, -0.728f, 0.00f,      1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,
/*68,P1*/ -0.074f, -0.694f, 0.00f,      1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,
/*69,Q1*/ 0.004f, -0.806f, 0.00f,       1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,
/*70,R1*/ 0.004f, -1.00f, 0.00f,        1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,
/*71,I*/  -0.504f, -1.00f, 0.00f,       1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,

    //middle bottom left inner piece (poly10), reuses 36,M1 37,N1 38,O1 44,U1 45,V1
/*72,A2*/ -0.366f, -0.60f, 0.00f,       1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,
/*73,B2*/ -0.164f, -0.273f, 0.00f,      1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,
/*74,C2*/ -0.113f, -0.342f, 0.00f,      1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,
/*75,M1*/ 0.006f, -0.508f, 0.00f,       1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,
/*76,N1*/ -0.186f, -0.558f, 0.00f,      1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,
/*77,O1*/ -0.093f, -0.728f, 0.00f,      1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,
/*78,U1*/ -0.187f, -0.94f, 0.00f,       1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,
/*79,V1*/ -0.325f, -0.684f, 0.00f,      1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,
/*80,K1*/ -0.049f, -0.179f, 0.00f,      1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,
/*81,J1*/ 0.006f, -0.301f, 0.00f,       1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,


    //middle piece without diamond (poly11), reuses B2 C2 B1 K1 L1 
/*82,D2*/ -0.058f, 0.349f, 0.00f,       1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,
/*83,E2*/ -0.094f, 0.371f, 0.00f,       1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,
/*84,F2*/ -0.231f, 0.298f, 0.00f,       1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,
/*85,G2*/ -0.09f, -0.041f, 0.00f,       1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,
/*86,H2*/ -0.215f, -0.207f, 0.00f,      1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,
/*87,B2*/ -0.164f, -0.273f, 0.00f,      1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,
/*88,C2*/ -0.113f, -0.342f, 0.00f,      1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,
/*89,K1*/ -0.049f, -0.179f, 0.00f,      1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,
/*90,L1*/ 0.004f, -0.054f, 0.00f,       1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,
/*91,B1*/ 0.005f, 0.082f, 0.00f,        1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,


    //left middle bottom piece (poly12), reuses A2 B2 V1 W1 Z1 H2
/*92,I2*/ -0.504f, 0.031f, 0.00f,       1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,
/*93,J2*/ -0.433f, 0.095f, 0.00f,       1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,
/*94,A2*/ -0.366f, -0.60f, 0.00f,       1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,
/*95,B2*/ -0.164f, -0.273f, 0.00f,      1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,
/*96,H2*/ -0.215f, -0.207f, 0.00f,      1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,
/*97,V1*/ -0.325f, -0.684f, 0.00f,      1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,
/*98,W1*/ -0.332f, -0.803f, 0.00f,      1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,
/*99,Z1*/ -0.501f, -0.646f, 0.00f,      1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,

    //left middle top piece (poly13), reuses H2 G2 F2 I2 J2
/*100,K2*/-0.182f, 0.448f, 0.00f,       1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,
/*101,L2*/-0.196f, 0.488f, 0.00f,       1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,
/*102,M2*/-0.438f, 0.537f, 0.00f,       1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,
/*103,N2*/-0.506f, 0.537f, 0.00f,       1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,
/*104,F2*/-0.231f, 0.298f, 0.00f,       1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,
/*105,G2*/-0.09f, -0.041f, 0.00f,       1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,
/*106,H2*/-0.215f, -0.207f, 0.00f,      1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,
/*107,I2*/-0.504f, 0.031f, 0.00f,       1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,
/*108,J2*/-0.433f, 0.095f, 0.00f,       1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,

    //top left piece (poly14), reuses M2 G J N2
/*109,O2*/-0.20f, 0.61f, 0.00f,         1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,
/*110,P2*/-0.185f, 0.623f, 0.00f,       1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,
/*111,Q2*/-0.288f, 0.83f, 0.00f,        1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,
/*112,M2*/-0.438f, 0.537f, 0.00f,       1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,
/*113,N2*/-0.506f, 0.537f, 0.00f,       1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,
/*114,J*/ -0.504f, 0.712f, 0.00f,       1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,
/*115,G*/ -0.20f, 1.00f, 0.00f,         1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,

    //topmost left piece (poly15), reuses Q2 L K G
/*116,R2*/-0.099f, 0.739f, 0.00f,       1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,
/*117,S2*/-0.062f, 0.753f, 0.00f,       1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,
/*118,K*/ 0.004f, 1.00f, 0.00f,         1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,
/*119,L*/ 0.003f, 0.95f, 0.00f,         1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,
/*120,G*/ -0.20f, 1.00f, 0.00f,         1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,
/*121,Q2*/-0.288f, 0.83f, 0.00f,        1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,

    //sun piece (poly16), reuses M L S2 R2 Q2 P2 O2 M2 L2 K2 F2 E2 D2 B1 A1 Z V U T R Q P O N
/*122,T2*/ 0.0f, 0.55f, 0.00f,          1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,
/*123,L*/  0.003f, 0.95f, 0.00f,        1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,
/*124,M*/  0.072f, 0.753f, 0.00f,       1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,
/*125,R2*/-0.099f, 0.739f, 0.00f,       1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,
/*126,S2*/-0.062f, 0.753f, 0.00f,       1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,
/*127,O2*/-0.20f, 0.61f, 0.00f,         1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,
/*128,P2*/-0.185f, 0.623f, 0.00f,       1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,
/*129,Q2*/-0.288f, 0.83f, 0.00f,        1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,
/*130,M2*/-0.438f, 0.537f, 0.00f,       1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,
/*131,K2*/-0.182f, 0.448f, 0.00f,       1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,
/*132,L2*/-0.196f, 0.488f, 0.00f,       1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,
/*133,D2*/-0.058f, 0.349f, 0.00f,       1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,
/*134,E2*/-0.094f, 0.371f, 0.00f,       1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,
/*135,F2*/-0.231f, 0.298f, 0.00f,       1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,
/*136,A1*/0.070f, 0.349f, 0.00f,        1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,
/*137,B1*/0.005f, 0.082f, 0.00f,        1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,
/*138,U*/ 0.186f, 0.450f, 0.00f,        1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,
/*139,V*/ 0.240f, 0.296f, 0.00f,        1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,
/*140,Z*/ 0.106f, 0.366f, 0.00f,        1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,
/*141,T*/ 0.205f, 0.484f, 0.00f,        1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,
/*142,R*/ 0.442f, 0.535f, 0.00f,        1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,
/*143,Q*/ 0.206f, 0.616f, 0.00f,        1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,
/*144,N*/ 0.107f, 0.739f, 0.00f,        1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,
/*145,O*/ 0.299f, 0.828f, 0.00f,        1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f,
/*146,P*/ 0.192f, 0.649f, 0.00f,        1.0f, 1.0f, 1.0f,       0.0f, 0.0f,         0.0f

    // -0.30f, -0.50f, 0.00f,      1.0f, 1.0f, 1.0f,       0.0f, 0.0f,     0.0f,
    // 0.30f, -0.50f, 0.00f,       1.0f, 1.0f, 1.0f,       1.0f, 0.0f,     0.0f,
    // -0.30f, 0.50f, 0.00f,       1.0f, 1.0f, 1.0f,       0.0f, 1.0f,     0.0f,
    
    // 0.30f, 0.50f, 0.00f,        1.0f, 1.0f, 1.0f,       1.0f, 1.0f,     1.0f,
    // 0.00f, 0.50f, 0.00f,        1.0f, 1.0f, 1.0f,       0.5f, 1.0f,     1.0f,
    // 0.30f, -0.50f, 0.00f,       1.0f, 1.0f, 1.0f,       1.0f, 0.0f,     1.0f,

    // 0.00f, 0.50f, 0.00f,        1.0f, 1.0f, 1.0f,       0.5f, 1.0f,     2.0f,
    // -0.30f, 0.50f, 0.00f,       1.0f, 1.0f, 1.0f,       0.0f, 1.0f,     2.0f,
    // 0.30f, -0.50f, 0.00f,       1.0f, 1.0f, 1.0f,       1.0f, 0.0f,     2.0f
};

GLuint indices[] = {

    //left gray side (poly2)
    0, 1, 2, //A, I, J
    2, 0, 3, //J, A, H
    3, 2, 4, //H, I, G

    //rigt gray side (poly1)
    5, 6, 7, //C, D, E
    7, 5, 8, //E, C, B
    7, 8, 9, //E, B, F

    //top right piece (poly3)
    19, 10, 11, //D, K, L
    19, 11, 12, //D, L, M
    19, 12, 13, //D, M, N
    19, 13, 14, //D, N, O
    19, 14, 20, //D, O, E
    20, 14, 15, //E, O, P
    15, 20, 16, //P, E, Q
    15, 16, 17, //P, Q, R
    20, 17, 18, //E, R, S
    16, 17, 20, //Q, R, E

    //middle top right piece (poly4)
    21, 22, 26, //R, S, W
    21, 26, 25, //R, W, V
    21, 25, 24, //R, V, U
    21, 24, 23, //R, U, T

    //middle bottom right piece (poly5)
    27, 28, 31, //Z, A1, D1
    28, 29, 31, //A1, B1, D1
    29, 31, 30, //B1, D1, C1
    27, 31, 33, //Z, D1, V
    33, 31, 34, //V, D1, W
    31, 34, 35, //D1, W, F1
    35, 34, 32, //F1, W, E1

    //middle piece with diamond (poly6)
    45, 36, 44, //D1, F1, C1
    44, 43, 42, //C1, B1, L1
    44, 42, 40, //C1, L1, J1
    41, 42, 40, //K1, L1, J1
    44, 40, 36, //C1, J1, F1
    36, 40, 38, //F1, J1, H1
    40, 38, 39, //J1, H1, I1
    38, 36, 37, //H1, F1, G1

    //bottom middle piece (poly7)
    55, 54, 56, //H1, G1, I1
    54, 53, 50, //G1, T1, Q1
    50, 52, 53, //Q1, S1, T1
    50, 51, 52, //Q1, R1, S1
    50, 56, 49, //Q1, I1, P1
    49, 46, 56, //P1, M1, I1
    46, 47, 49, //M1, N1, P1
    48, 49, 47, //O1, P1, N1
    56, 54, 50, //I1, G1, Q1

    //bottom right piece (poly8)
    57, 58, 59, //E1, F1, G1
    59, 57, 61, //G1, E1, T1
    61, 62, 57, //T1, F, E1
    62, 61, 60, //F, T1, S1

    //bottom left piece (poly9)
    68, 67, 69, //P1, O1, Q1
    67, 69, 63, //O1, Q1, U1
    69, 63, 70, //Q1, U1, R1
    63, 70, 71, //U1, R1, I
    71, 63, 65, //I, U1, W1
    63, 65, 64, //U1, W1, V1
    71, 65, 66, //I, W1, Z1

    //middle bottom left inner piece (poly10)
    77, 78, 79, //O1, U1, V1
    77, 79, 72, //O1, V1, A2
    72, 76, 77, //A2, N1, O1
    72, 73, 76, //A2, B2, N1
    73, 76, 74, //B2, N1, C2
    74, 76, 75, //C2, N1, M1
    75, 74, 81, //M1, C2, J1
    74, 81, 80, //C2, J1, K1

    //middle piece without diamond (poly11)
    87, 88, 89, //B2, C2, K1
    87, 89, 86, //B2, K1, H2
    86, 89, 90, //H2, K1, L1
    90, 86, 85, //L1, H2, G2
    85, 90, 91, //G2, L1, B1
    91, 85, 84, //B1, G2, F2
    84, 91, 82, //F2, B1, D2
    82, 84, 83, //D2, F2, E2

    //left middle bottom piece (poly12)
    97, 98, 94, //W1, V1, A2
    94, 97, 99, //A2, W1, Z1
    99, 94, 96, //Z1, A2, H2
    96, 94, 95, //H2, A2, B2
    96, 99, 92, //H2, Z1, I2
    92, 96, 93, //I2, H2, J2

    //left middle top piece (poly13)
    107, 103, 102, //I2, N2, M2
    102, 107, 108, //M2, I2, J2
    108, 106, 105, //J2, H2, G2
    105, 108, 104, //G2, J2, F2
    104, 108, 102, //F2, J2, M2
    102, 104, 100, //M2, F2, K2
    100, 102, 101, //K2, M2, L2

    //top left piece (poly14)
    113, 112, 114, //N2, M2, J
    114, 112, 111, //J, M2, Q2
    111, 109, 110, //Q2, O2, P2
    111, 114, 115, //Q2, J, G
    112, 111, 109, //Q2, M2, O2

    //topmost left piece (poly15)
    121, 116, 117, //Q2, R2, S2
    117, 121, 120, //S2, Q2, G
    120, 117, 119, //G, S2, L
    119, 120, 118, //L, G, K

    //sun piece (poly16)
    //outer rays
    123, 124, 126, //L, M, S2
    145, 144, 146, //O, N, P
    141, 142, 143, //T, R, Q
    138, 139, 140, //U, V, Z
    136, 137, 133, //A1, B1, D2
    134, 135, 131, //E2, F2, K2
    132, 130, 127, //L2, M2, O2
    128, 129, 125, //P2, Q2, R2

    //rays to center
    124, 126, 122, //M, S2, T2
    144, 146, 122, //N, P, T2
    141, 143, 122, //T, Q, T2
    138, 140, 122, //U, Z, T2
    136, 133, 122, //A1, D2, T2
    134, 131, 122, //E2, K2, T2
    132, 127, 122, //L2, O2, T2
    128, 125, 122, //P2, R2, T2

    //in-betweens to center
    125, 126, 122, //R2, S2, T2
    124, 144, 122, //M, N, T2
    146, 143, 122, //P, Q, T2
    141, 138, 122, //T, U, T2
    140, 136, 122, //Z, A1, T2
    133, 134, 122, //D2, E2, T2
    131, 132, 122, //K2, L2, T2
    127, 128, 122 //O2, P2, T2

};
    

// define OpenGL object IDs to represent the vertex array and the shader program in the GPU
GLuint vao;         // vertex array object (stores the render state for our vertex array)
GLuint vbo;         // vertex buffer object (reserves GPU memory for our vertex array)
GLuint ebo;
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
