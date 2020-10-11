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
#include "teapot.h"

#include "math.h"
#include "GLee.h"
#include "freeglut.h"
#include <fstream>
#include <stdlib.h>

#define CHECKERROR {int err = glGetError(); if (err) fprintf(stderr, "OpenGL error (at line %d): %x\n", __LINE__, err); }

//@@ Declare your FBOs here.  You'll need one per component of the
// reflection map.
FBO fbo0, fbo1;

//@@ Declare your shaders here.  You'll probably need two, one for
// the creation of the reflection map, and one for the drawing of the
// final scene.
ShaderProgram shader1;
ShaderProgram shader2;


float whiteColor[4] = {1.0, 1.0, 1.0, 1.0};
float envColor[4] = {0.5f, 0.5f, 0.9f, 1.0f};
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

    // Send vertex's tangent (i.e., Pu)
    //int loc = glGetAttribLocation(shader2.program, "vertexTangent");
    //glVertexAttrib3f(loc, -y, x, 0);

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
// Generates a bunch of random ellipsoids to provide a distant
// environment to be reflected in a central reflecting object.  Also
// generates a even larger sphere surrounding everything to provide a
// background.
void CreatEnv(Scene &scene)
{
    float t = 1.0;
    float s = 40.0;

    scene.envDL = glGenLists(1);
    glNewList(scene.envDL, GL_COMPILE);
    
    glPushMatrix();
	float color[4] = {0.8, 0.8, 0.9, 1.0};
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
    glScalef(s, s, s);
    glCallList(scene.sphereDL);
    glPopMatrix();

    float rMax = 5.0f;
    for (int i=0;  i<scene.nSpheres;  i++) {
        float color[4] = {R(0.2f,1.0f), R(0.2f,1.0f), R(0.2f,1.0f), 1.0f};
        glPushMatrix();
		glRotatef(R(0.0, 360.0), 0.0, 0.0, 1.0);
		glRotatef(R(-90.0, 90.0), 0.0, 1.0, 0.0);
        glTranslatef(R(20.0,30.0), 0.0, 0.0);
        glScalef(R(0.1f*rMax,rMax), R(0.1f*rMax,rMax), R(0.1f*rMax,rMax));
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);

        glCallList(scene.sphereDL);
        glPopMatrix(); }

    glEndList();

}

////////////////////////////////////////////////////////////////////////
// InitializeScene is called with the expectaton that OpenGL modes
// will be set up.  This should include lights, materials, shaders,
// texture maps, and display lists containing the objects to be drawn.
void InitializeScene(Scene &scene)
{
    scene.mode = 0;
    scene.nSpheres = 256;
    scene.showPass1 = 0;

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

    //@@ Create the shader program from some vert/frag files of source code.
    shader1.CreateProgram();
    shader1.CreateShader("shader1.vert", GL_VERTEX_SHADER);
    shader1.CreateShader("shader1.frag", GL_FRAGMENT_SHADER);
    shader1.LinkProgram();
    
    //@@ Create the shader program from some vert/frag files of source code.
    shader2.CreateProgram();
    shader2.CreateShader("shader2.vert", GL_VERTEX_SHADER);
    shader2.CreateShader("shader2.frag", GL_FRAGMENT_SHADER);
    shader2.LinkProgram();
    
}

void BuildScene(Scene &scene)
{
	//@@ Initialize reflection map component FBOs here.  The size of
	// each must be chosen based on the total number of pixels you
	// want to allocate for the reflection map.

	int size = 256;				//@@ Sample -- choose your own size
    fbo0.CreateFBO(size, size);
    fbo1.CreateFBO(size, size);

	// Allocate the various display lists used.
    scene.sphereDL = glGenLists(1);
    scene.objectDL = glGenLists(1);
    scene.envDL = glGenLists(1);

    // Create a display list for a sphere.
    glNewList(scene.sphereDL, GL_COMPILE);
    CreateSphere(scene, 32);
    glEndList();

    // Create the environment
    CreatEnv(scene);

	// Create the reflective object.
	//@@ Choose either the sphere (for boring but predictable
	// reflections) or the teapot (for more interesting reflections).
    glNewList(scene.objectDL, GL_COMPILE);
	DrawTeapot(0.75, 32);
	glCallList(scene.sphereDL);
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
	CHECKERROR;

    // Calculate the light's position from values controlled by the
    // user, and tell OpenGL where it is located.
    float lPos[4] = {
        scene.lightDist*cos(scene.lightSpin*rad)*sin(scene.lightTilt*rad),
        scene.lightDist*sin(scene.lightSpin*rad)*sin(scene.lightTilt*rad),
        scene.lightDist*cos(scene.lightTilt*rad),
        1.0 };
    glLightfv(GL_LIGHT0, GL_POSITION, lPos);

    ///////////////////////////////////////////////////////////////////
    // Pass 1: The geometry of the scene is drawn to the FBO render
    // targets.  The expection of the pass1 shaders is that the center
    // of reflection is at the orign.
    ///////////////////////////////////////////////////////////////////

	// No perspecive transformation needed.  The pass1 vertex
	// processor will project the world onto the reflactions map
	// components via (possibly complex) calculations.
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Set up the modelview matrix, leaving the reflective object's
    // center at the origin.
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glRotatef(scene.eyeTilt, 1.0f, 0.0f, 0.0f);
    glRotatef(scene.eyeSpin, 0.0f, 0.0f, 1.0f);

    // Bind pass1 shader program.
    shader1.Use();

    // Bind the FBO.  Any graphics pipeline operations will now write
    // to the FBO's texture instead of the screen.  Set its viewport,
    // and clear the screen (texture).
	// First compont:
	fbo0.Bind();
    glViewport(0, 0, fbo0.width, fbo0.height);
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
	// Inform the shader of which component is being drawn
	int loc = glGetUniformLocation(shader1.program, "component");
	glUniform1i(loc, 0);
	// Draw environment
    glCallList(scene.envDL);
    fbo0.Unbind();

	// Repeat above steps for each further component.
	fbo1.Bind();
    glViewport(0, 0, fbo1.width, fbo1.height);
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
	loc = glGetUniformLocation(shader1.program, "component");
	glUniform1i(loc, 1);
    glCallList(scene.envDL);
    fbo1.Unbind();

    // Remove shader program
    shader1.Unuse();

    ///////////////////////////////////////////////////////////////////
    // Pass 2: Draw the scene normally after making the output of
    // pass 1 available to the pass 2 shader program.
    ///////////////////////////////////////////////////////////////////

    // Set the viewport, and clear the screen
    glViewport(0,0,scene.width, scene.height);
    glClearColor(0.5,0.5, 0.5, 1.0);
    glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);

    // For debug purposes: This code draws the FBO's texture directly
    // to the screen.  It is triggered with the 's' key.
    if (scene.showPass1) {
        glMatrixMode(GL_TEXTURE);
        glLoadIdentity();;
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

		// Activates an FBO's texture and draws a rectange of the
		// appropriate height and width to display it
        glDisable(GL_LIGHTING);
		glPushMatrix();
		glTranslatef(-0.55, 0.0, 0.0);
		float max = fbo0.width>fbo0.height ? fbo0.width : fbo0.height;
		glScalef(0.4, 0.4, 0.4);
		glScalef(fbo0.width/max, fbo0.height/max, 1.0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, fbo0.texture);
        glEnable(GL_TEXTURE_2D);
        glBegin(GL_QUADS);
        glTexCoord2f( 0.0,  0.0); glVertex2f(-1.0, -1.0);
        glTexCoord2f( 1.0,  0.0); glVertex2f( 1.0, -1.0);
        glTexCoord2f( 1.0,  1.0); glVertex2f( 1.0,  1.0);
        glTexCoord2f( 0.0,  1.0); glVertex2f(-1.0,  1.0);
        glEnd();
		glPopMatrix();

		// Repeat the above for each FBO's texture
		glPushMatrix();
		glTranslatef(0.55, 0.0, 0.0);
		max = fbo1.width>fbo1.height ? fbo1.width : fbo1.height;
		glScalef(0.4, 0.4, 0.4);
		glScalef(fbo1.width/max, fbo1.height/max, 1.0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, fbo1.texture);
        glBegin(GL_QUADS);
        glTexCoord2f( 0.0,  0.0); glVertex2f(-1.0, -1.0);
        glTexCoord2f( 1.0,  0.0); glVertex2f( 1.0, -1.0);
        glTexCoord2f( 1.0,  1.0); glVertex2f( 1.0,  1.0);
        glTexCoord2f( 0.0,  1.0); glVertex2f(-1.0,  1.0);
        glEnd();
		glPopMatrix();

        glDisable(GL_TEXTURE_2D);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, 0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, 0);
        glEnable(GL_LIGHTING);
		return; }

	// Proceed with Pass 2.  Setup the perspective and modelview
    // matrices for normal viewing.
    double sx, sy;
    sy = 0.2f*scene.front;
	sx = sy * scene.width/scene.height;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-sx, sx, -sy, sy, scene.front, 1000.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(scene.translatex, scene.translatey, -scene.zoom);
    glRotatef(scene.eyeTilt, 1.0f, 0.0f, 0.0f);
    glRotatef(scene.eyeSpin, 0.0f, 0.0f, 1.0f);

	// Use pass 2 shader program
	shader2.Use();

    // Make each texture from pass 1 active in a texture unit, and
    // inform shader2.
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, fbo0.texture);
	loc = glGetUniformLocation(shader2.program, "texture0");
	glUniform1i(loc, 0);

	// Repeat the above for each FBO
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, fbo1.texture);
	loc = glGetUniformLocation(shader2.program, "texture1");
	glUniform1i(loc, 1);
	
	// Draw the scene objects, informing the shader whether they
	// should be shaded normally, or shaded as a reflecting object.
	loc = glGetUniformLocation(shader2.program, "reflective");
	glUniform1i(loc, 0);
	glCallList(scene.envDL);	// NON-reflective parts
	
	loc = glGetUniformLocation(shader2.program, "reflective");
	glUniform1i(loc, 1);
	glCallList(scene.objectDL);	// Reflective parts
    
	// Done with shader program
	shader2.Unuse();
    
	// Draw a small white sphere at the position of the light. glPushMatrix();
	glPushMatrix();
	glTranslatef(lPos[0],lPos[1],lPos[2]);
	glScalef(0.05f, 0.05f, 0.05f);
	glDisable(GL_LIGHTING);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, whiteColor);
	glCallList(scene.sphereDL);
	glPopMatrix();

}
