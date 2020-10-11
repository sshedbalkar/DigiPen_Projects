///////////////////////////////////////////////////////////////////////
// Defines and draws a scene with shadows.
//
// Gary Herron
//
// Copyright © 2011 DigiPen Institute of Technology
////////////////////////////////////////////////////////////////////////

#include "shader.h"
#include "fbo.h"
#include "scene.h"

#include "math.h"
#include "GLee.h"
#include "freeglut.h"
#include <fstream>
#include <stdlib.h>

#define CHECKERROR {int err = glGetError(); if (err) fprintf(stderr, "OpenGL error (at line %d): %x\n", __LINE__, err); }

//@@ Declare your FBO and shader program here.
FBO fbo;
ShaderProgram shader1;
ShaderProgram shader2;


float whiteColor[4] = {1.0, 1.0, 1.0, 1.0};
float roomColor[4] = {0.5f, 0.5f, 0.9f, 1.0f};
float ambientColor[4] = {0.5f, 0.5f, 0.5f, 1.0f};
float lightColor[4] = {0.8f, 0.8f, 0.8f, 1.0f};
float specularColor[4] = {1.0f, 1.0f, 1.0f, 1.0f};
int shininess = 120;

const float PI = 3.14159f;
const float rad = PI/180.0f;

float R(float min, float max)
{
    return min + (max-min)*((float)rand())/((float)RAND_MAX);
}

////////////////////////////////////////////////////////////////////////
// Issues a vertex on a sphere with associated texture coordiantes and
// normal and tangent.
void SphereVert(float s, float t)
{
    float x = cos(s)*sin(t);
    float y = sin(s)*sin(t);
    float z = cos(t);

    float u = s/(2.0f*PI);
    float v = 1.0f-t/PI;

    // Send veertex's texture coordinate
    glTexCoord3f(u,v,0.0f);

    // Send vertex's normal
    float d = 1.0f/sqrt(x*x+y*y+z*z);
    glNormal3f(d*x,d*y,d*z);

    // Finally, send vertex.
    glVertex3f(x,y,z);
}

////////////////////////////////////////////////////////////////////////
// Generates a sphere with alternating strips of color
void CreateSphere(Scene &scene, int n)
{
    float d = 2.0f*PI/float(n*2);
    for (int s=0;  s<n*2;  s++) {
        glBegin(GL_QUAD_STRIP);
        float s0 = d*s;
        float s1 = d*(s+1);
        for (int t=0;  t<=n;  t++) {
            float t0 = PI*t/float(n);
            SphereVert(s1,t0);
            SphereVert(s0,t0); }
        glEnd(); }
}

////////////////////////////////////////////////////////////////////////
// Issues a vertex on a torus with associated texture coordiantes
// normal and tangent.
void TorusVert(float r1, float r2, float s, float t)
{
    float x = r1*sin(s) + r2*sin(t)*sin(s);
    float y = r1*cos(s) + r2*sin(t)*cos(s);
    float z =             r2*cos(t);

    float u = s/(2.0f*PI);
    float v = t/(2.0f*PI);

    // Send veertex's texture coordinate
    glTexCoord3f(u,v,0.0f);

    // Send vertex's normal
    float d = 1.0f/sqrt(x*x+y*y+z*z);
    glNormal3f(sin(t)*sin(s), sin(t)*cos(s), cos(t));

    // Finally, send vertex.
    glVertex3f(x,y,z);
}

////////////////////////////////////////////////////////////////////////
// Generates a sphere with alternating strips of color
void CreateTorus(Scene &scene, int n, int m, float r2)
{
    float d = 2.0f*PI/float(n);
    float r1 = 1.0f-r2;
    for (int s=0;  s<n;  s++) {
        glBegin(GL_QUAD_STRIP);
        float s0 = d*s;
        float s1 = d*(s+1);
        for (int t=0;  t<=m;  t++) {
            float t0 = 2.0f*PI*t/float(m);
            TorusVert(r1, r2, s1,t0);
            TorusVert(r1, r2, s0,t0); }
        glEnd(); }
}

////////////////////////////////////////////////////////////////////////
// Generates a sphere with alternating strips of color
void CreatRoom(Scene &scene)
{
    float t = 1.0;
    float s = 2.0;

    int sqr = glGenLists(1);
    glNewList(sqr, GL_COMPILE);

    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, roomColor);

    glBegin(GL_QUADS);  
    glNormal3f(0.0, 0.0, 1.0);
    glTexCoord2f( 0.0,  0.0); glVertex2f(-t, -t);
    glTexCoord2f( 0.0,  1.0); glVertex2f(-t,  t);
    glTexCoord2f( 1.0,  1.0); glVertex2f( t,  t);
    glTexCoord2f( 1.0,  0.0); glVertex2f( t, -t);
    glEnd();
    glEndList();
    
    scene.roomDL = glGenLists(1);
    glNewList(scene.roomDL, GL_COMPILE);
    
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, -s);
    glScalef(s, s, s);
    glCallList(sqr);
    glPopMatrix();

    glPushMatrix();
    glRotatef(90, 0,1,0);
    glTranslatef(0.0f, 0.0f, -s);
    glScalef(s, s, s);
    glCallList(sqr);
    glPopMatrix();

    glPushMatrix();
    glRotatef(-90, 1,0,0);
    glTranslatef(0.0f, 0.0f, -s);
    glScalef(s, s, s);
    glCallList(sqr);
    glPopMatrix();
    glEndList();

}

////////////////////////////////////////////////////////////////////////
// InitializeScene is called with the expectaton that OpenGL modes
// will be setup.  This should include lights, materials, shaders,
// texture maps, and display lists containing the objects to be drawn.
void InitializeScene(Scene &scene)
{
    scene.mode = 1;
    scene.nSpheres = 8;
    scene.showPass1 = 0;
    scene.shadowSize = 1024;
    scene.cull = true;

    // Set the initial transformation parameters
    scene.front = 1.0;
    scene.eyeSpin = -135.0;
    scene.eyeTilt = -60.0;
    scene.translatex = 0.0;
    scene.translatey = 0.0;
    scene.zoom = 12.0;
    
    // Set the initial light position parammeters
    scene.lightSpin = 45.0-135.0;
    scene.lightTilt = -60.0;
    scene.lightDist = 30.0;
    
    // Initialize OpenGL parameters that will not change

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glShadeModel(GL_SMOOTH);

    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, 1);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  lightColor);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightColor);
    glEnable(GL_LIGHT0);

    glMateriali(GL_FRONT, GL_SHININESS, shininess);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specularColor);

    // Create the shader program from some vert/frag files of source code.
    shader1.CreateProgram();
    shader1.CreateShader("shader1.vert", GL_VERTEX_SHADER);
    shader1.CreateShader("shader1.frag", GL_FRAGMENT_SHADER);
    shader1.LinkProgram();
    
    // Create the shader program from some vert/frag files of source code.
    shader2.CreateProgram();
    shader2.CreateShader("shader2.vert", GL_VERTEX_SHADER);
    shader2.CreateShader("shader2.frag", GL_FRAGMENT_SHADER);
    shader2.LinkProgram();
    
}

void BuildScene(Scene &scene)
{
    float rMax = 0.5f*4.0f/sqrt((float)scene.nSpheres);

    int size = 1024;                //@@ Sample -- choose your own size
    fbo.CreateFBO(size, size);

    // Allocate the various display lists used.
    scene.sphereDL = glGenLists(1);
    scene.sceneDL = glGenLists(1);
    scene.roomDL = glGenLists(1);

    // Create a display list for a sphere.
    glNewList(scene.sphereDL, GL_COMPILE);
    CreateSphere(scene, 32);
    glEndList();

    // Create the room
    CreatRoom(scene);

    glNewList(scene.sceneDL, GL_COMPILE);

    for (int i=0;  i<scene.nSpheres;  i++) {
        float color[4] = {R(0.2f,1.0f), R(0.2f,1.0f), R(0.2f,1.0f), 1.0f};
        glPushMatrix();
        glTranslatef(R(-1.0,1.0), R(-1.0,1.0),
                     R(-1.0,1.0));
        glScalef(R(0.1f*rMax,rMax), R(0.1f*rMax,rMax),
                 R(0.1f*rMax,rMax));
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);

        glCallList(scene.sphereDL);
        glPopMatrix();
    }
    glEndList();

}

////////////////////////////////////////////////////////////////////////
// Procedure DrawScene is called whenever the scene needs to be drawn.
void DrawScene(Scene &scene)
{   
    //@@ Most OpenGL calls can record errors.  A careful programmer
    // will check the error status *often*, perhaps as often as after
    // each OpenGL call.  At the very least, once per refresh will
    // tell you if something is going wrong.

    // Calculate the light's position from values controlled by the
    // user. 
    float lPos[4] = {
        scene.lightDist*cos(scene.lightSpin*rad)*sin(scene.lightTilt*rad),
        scene.lightDist*sin(scene.lightSpin*rad)*sin(scene.lightTilt*rad),
        scene.lightDist*cos(scene.lightTilt*rad),
        1.0 };
    glLightfv(GL_LIGHT0, GL_POSITION, lPos);

    ///////////////////////////////////////////////////////////////////
    // Pass 1: The scene is drawn from the light's point of view into
    // the shadowTexture of the FBO.
    ///////////////////////////////////////////////////////////////////

    // Calculate the light's view frustum parameters
    float front = 0.5;
    float h = 2.5f*front/scene.lightDist;
    float w = h; // * float(scene.width) / float(scene.height);
   
    // Setup transformations for light's viewpoint
    // Projection transformation (PL):
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity(); 
    glFrustum(-w, w, -h, h, front, scene.lightDist+5.0);
    // View transformation (VL):
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(lPos[0],lPos[1],lPos[2], 0.0, 0.0, 0.0, 0.0, 0.0, 1.0);
     
    // Draw scene using pass1 shader program, culling if desired.
    shader1.Use();

    // Bind the FBO set it's viewport, clear, draw the scene, and
    // finally unbind the FBO.  Any graphics pipeline operations will
    // write to the FBO's texture instead of the screen.
    fbo.Bind();
    glViewport(0, 0, scene.shadowSize, scene.shadowSize);
    glClearColor(1.0, 1.0, 1.0, 9999.0);
    glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);

    glCallList(scene.sceneDL);  // Draw the scene
	glCallList(scene.roomDL);

    fbo.Unbind();

    // Remove shader program
    shader1.Unuse();
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

    ///////////////////////////////////////////////////////////////////
    // Make the ShadowTransformation here
    // This is the product B PL VL Vinv where:
    //    B    is the bias matrix that maps  [-1,1] to [0,1]
    //    PL   is the light's perspective matrix (see above)
    //    VL   is the light's view matrix (see above)
    //    Vinv is the inverse of the world viewing matrix (see below)
    // The product is called the shadow transformation
    ///////////////////////////////////////////////////////////////////

    //@@ ShadowTransformation: ...
	// Calculated and stored in one of the texture matrices
	glMatrixMode(GL_TEXTURE);
    glLoadIdentity();
	// Bias Transformation, B
	float B[] = {0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 0.5f, 0.0f, 1.0f};
    glMultMatrixf(B);
	// Light Perspective Matrix, PL
	glFrustum(-w, w, -h, h, front, scene.lightDist+5.0);
	// Light's View Matrix, VL
	gluLookAt(lPos[0],lPos[1],lPos[2], 0.0, 0.0, 0.0, 0.0, 0.0, 1.0);
	// Inverse of World View Matrix, Vinv
	glRotatef(-scene.eyeSpin, 0.0f, 0.0f, 1.0f);
    glRotatef(-scene.eyeTilt, 1.0f, 0.0f, 0.0f);
	glTranslatef(-scene.translatex, -scene.translatey, scene.zoom);
	
    ///////////////////////////////////////////////////////////////////
    // Pass 2: Draw the scene normally after making the output of
    // pass 1 available to the pass 2 shader program.
    ///////////////////////////////////////////////////////////////////

    // Set the viewport, and clear the screen
    glViewport(0,0,scene.width, scene.height);
    glClearColor(0.5,0.5, 0.5, 1.0);
    glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);

    double sx, sy;
    if (scene.width < scene.height) {
        sx = 0.2f*scene.front;
        sy = sx * scene.height/scene.width; }
    else {
        sy = 0.2f*scene.front;
        sx = sy * scene.width/scene.height; }

    // Set up the projection matrix (P)
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-sx, sx, -sy, sy, scene.front, 100.0);

    // Set up the modelview matrix (V)
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(scene.translatex, scene.translatey, -scene.zoom);
    glRotatef(scene.eyeTilt, 1.0f, 0.0f, 0.0f);
    glRotatef(scene.eyeSpin, 0.0f, 0.0f, 1.0f);

    // Make shadow texture (from pass 1) active in texture unit 0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, fbo.texture);

    // For debug purposes: allow viewing of the shadow texture
    if (scene.showPass1) {
        glDisable(GL_LIGHTING);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glMatrixMode(GL_TEXTURE);
        glLoadIdentity();
        glEnable(GL_TEXTURE_2D);
        glBegin(GL_QUADS);
        glTexCoord2f( 0.0,  0.0); glVertex2f(-1.0, -1.0);
        glTexCoord2f( 1.0,  0.0); glVertex2f( 1.0, -1.0);
        glTexCoord2f( 1.0,  1.0); glVertex2f( 1.0,  1.0);
        glTexCoord2f( 0.0,  1.0); glVertex2f(-1.0,  1.0);
        glEnd();
        glDisable(GL_TEXTURE_2D);
        glEnable(GL_LIGHTING); }

    // Non-debug draw scene with shadows.
    else {
        // Use pass 2 shader program
        shader2.Use();

        // Let shader know shadowTexture is in texture unit 0
        int loc = glGetUniformLocation(shader2.program, "shadowTexture");
        glUniform1i(loc, 0);

        loc = glGetUniformLocation(shader2.program, "mode");
        glUniform1i(loc, scene.mode);

        // Draw the scene
        glCallList(scene.sceneDL);
        glCallList(scene.roomDL);
                                                
        // Done with shader program
        shader2.Unuse();
        
        // Draw a small white sphere at the position of the light.
        glPushMatrix();
        glTranslatef(lPos[0],lPos[1],lPos[2]);
        glScalef(0.05f, 0.05f, 0.05f);
        glDisable(GL_LIGHTING);
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, whiteColor);
        glCallList(scene.sphereDL);
        glPopMatrix(); }

    glBindTexture(GL_TEXTURE_2D, 0); // Un-Bind texture
}
