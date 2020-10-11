///////////////////////////////////////////////////////////////////////
// $Id$
//
// Defines and draws a scene.
//
// Gary Herron
//
// Copyright © 2009 DigiPen Institute of Technology
////////////////////////////////////////////////////////////////////////

#include "shader.h"
#include "scene.h"
#include <iostream>

#include "math.h"
#include "GLee.h"
#include "freeglut.h"

const float PI = 3.14159;
const float rad = PI/180.0;

GLuint texId1,texId2;

void set(float v[4], float a, float b, float c, float d)
{
    v[0] = a;
    v[1] = b;
    v[2] = c;
    v[3] = d;
}

ShaderProgram shader;

////////////////////////////////////////////////////////////////////////
// Issues a vertex on a sphere with associated texture coordiantes and
// normal and tangent.
void SphereVert(float s, float t)
{
    float x = cos(s)*sin(t);
    float y = sin(s)*sin(t);
    float z = cos(t);

    float u = s/(2.0f*PI);
    float v = 1.0-t/PI;

    // Send veertex's texture coordinate
    glTexCoord3f(u,v,0.0f);

    // Send vertex's normal
    float d = 1.0f/sqrt(x*x+y*y+z*z);
    glNormal3f(d*x,d*y,d*z);

    // Send vertex's tangent (i.e., Pu)
    int loc = glGetAttribLocation(shader.program, "vertexTangent");
    glVertexAttrib3f(loc, -y, x, 0);

    // Finally, send vertex.
    glVertex3f(x,y,z);
}

////////////////////////////////////////////////////////////////////////
// Generates a sphere with alternating strips of color
void CreateSphere(Scene &scene, int n)
{
    float d = 2.0f*PI/float(n*2);
    for (int s=0;  s<n*2;  s++) {
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, scene.Kd[s%2]);
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

    // Send vertex's tangent (i.e., Pu)
    int loc = glGetAttribLocation(shader.program, "vertexTangent");
    glVertexAttrib3f(loc, -y, x, 0);

    // Finally, send vertex.
    glVertex3f(x,y,z);
}

////////////////////////////////////////////////////////////////////////
// Generates a sphere with alternating strips of color
void CreateTorus(Scene &scene, int n, int m, float r2)
{
    float d = 2.0f*PI/float(n);
    float r1 = 1.0-r2;
    for (int s=0;  s<n;  s++) {
        glBegin(GL_QUAD_STRIP);
        float s0 = d*s;
        float s1 = d*(s+1);
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, scene.Kd[s%2]);
        for (int t=0;  t<=m;  t++) {
            float t0 = 2.0f*PI*t/float(m);
            TorusVert(r1, r2, s1,t0);
            TorusVert(r1, r2, s0,t0); }
        glEnd(); }
}

////////////////////////////////////////////////////////////////////////
// InitializeScene is called with the expectaton that OpenGL modes
// will be setup.  This should include lights, materials, shaders,
// texture maps, and display lists containing the objects to be drawn.
void InitializeScene(Scene &scene)
{
    scene.mode = 0;

    // Set the initial transformation parameters
    scene.front = 1.0;
    scene.eyeSpin = -135.0;
    scene.eyeTilt = -60.0;
    scene.translatex = 0.0;
    scene.translatey = 0.0;
    scene.zoom = 6.0;
    
    // Set the initial light position parammeters
    scene.lightSpin = 0.0;
    scene.lightTilt = 90.0;
    scene.lightDist = 30.0;
    
    // Set the light properties
    set(scene.ambient, 0.2, 0.2, 0.2, 1.0);
    set(scene.lightI, 0.8f, 0.8f, 0.8f, 1.0f);
    
    // Set the material properties
    set(scene.Kd[0], 0.5, 0.5, 0.9, 1.0);
    set(scene.Kd[1], 0.5, 0.5, 0.5, 1.0);
    set(scene.Ks, 1.0f, 1.0f, 1.0f, 1.0f);
    scene.shininess = 120;
    
    // Start off using the shader (can be toggled with the 's' key.)
    scene.UseShader = true;
    
    // Initialize OpenGL parameters that will not change
    glClearColor(1.0,1.0, 1.0, 1.0);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glShadeModel(GL_SMOOTH);

    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, scene.ambient);
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, 1);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  scene.lightI);
    glLightfv(GL_LIGHT0, GL_SPECULAR, scene.lightI);
    glEnable(GL_LIGHT0);

    glMateriali(GL_FRONT, GL_SHININESS, scene.shininess);
    glMaterialfv(GL_FRONT, GL_SPECULAR, scene.Ks);

    // Create the shader program from some vert/frag files of source code.
    shader.CreateProgram();
    shader.CreateShader("shader.vert", GL_VERTEX_SHADER);
    shader.CreateShader("shader.frag", GL_FRAGMENT_SHADER);
    shader.LinkProgram();
    
    // Create the display lists for several objects
    scene.objectDisplayList = glGenLists(2);

    // A sphere in displaylist scene.objectDisplayList+0
    glNewList(scene.objectDisplayList, GL_COMPILE);
    CreateSphere(scene, 24);
    glEndList();
    
    // A torus in displaylist scene.objectDisplayList+1
    glNewList(scene.objectDisplayList+1, GL_COMPILE);
    CreateTorus(scene, 24, 12, 0.3);
    glEndList();

    FILE *f;
	unsigned char* texture_map=new unsigned char[256*256*3];
	unsigned char* height_map=new unsigned char[256*256*1];

	if(f=fopen("tile_color.raw","rb"))
	{
		fread(texture_map,256*256,3,f);
		fclose(f);
	}

	if(f=fopen("tile_height.raw","rb"))
	{
		fread(height_map,256*256,1,f);
		fclose(f);
	}
	
    
	// Send small texture map to OpenGL
    glGenTextures(1,&texId1);
    glBindTexture(GL_TEXTURE_2D, texId1);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, 256,256,GL_RGB, GL_UNSIGNED_BYTE, texture_map);
    glBindTexture(GL_TEXTURE_2D,0);

	glGenTextures(1,&texId2);
    glBindTexture(GL_TEXTURE_2D, texId2);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_LUMINANCE, 256,256,GL_LUMINANCE, GL_UNSIGNED_BYTE, height_map);
    glBindTexture(GL_TEXTURE_2D,0);
}

////////////////////////////////////////////////////////////////////////
// Procedure DrawScene is called whenever the scene needs to be drawn.
void DrawScene(Scene &scene)
{
    glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);

    double sx, sy;
    if (scene.width < scene.height) {
        sx = 0.2f*scene.front;
        sy = sx * scene.height/scene.width; }
    else {
        sy = 0.2f*scene.front;
        sx = sy * scene.width/scene.height; }

    // Setup the projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-sx, sx, -sy, sy, scene.front, 100.0);

    // Setup the modelview matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, -3.0f);
    glTranslatef(0.01*scene.translatex, -0.01*scene.translatey, -scene.zoom);
    glRotatef(scene.eyeTilt, 1.0f, 0.0f, 0.0f);
    glRotatef(scene.eyeSpin, 0.0f, 0.0f, 1.0f);

    // Setup the light's position
    float lPos[4] = {
        scene.lightDist*cos(scene.lightSpin*rad)*sin(scene.lightTilt*rad),
        scene.lightDist*sin(scene.lightSpin*rad)*sin(scene.lightTilt*rad),
        scene.lightDist*cos(scene.lightTilt*rad),
        1.0 };
    glLightfv(GL_LIGHT0, GL_POSITION, lPos);

    // Draw the display list with or without the shader program
    if (scene.UseShader)  shader.Use();

    int loc = glGetUniformLocation(shader.program, "mode");
    glUniform1i(loc, scene.mode);

    // Make texture map active, and inform shaders how to access it:
    glActiveTexture(GL_TEXTURE0);        // Choose texture unit 0
    glBindTexture(GL_TEXTURE_2D, texId1); // Bind texture to unit 0
    
	glActiveTexture(GL_TEXTURE1);        // Choose texture unit 1
    glBindTexture(GL_TEXTURE_2D, texId2); // Bind texture to unit 1

    // Let shader access the texture unit 0 via variable named "tile_color".
    loc = glGetUniformLocation(shader.program, "tile_color");
    glUniform1i(loc, 0);

	// Let shader access the texture unit 1 via variable named "tile_height".
    loc = glGetUniformLocation(shader.program, "tile_height");
    glUniform1i(loc, 1);

	glCallList(scene.objectDisplayList+scene.model);
        
    if (scene.UseShader)  shader.Unuse();
	
	glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, 0); // Un-Bind texture 1
	
	glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0); // Un-Bind texture 0
}
