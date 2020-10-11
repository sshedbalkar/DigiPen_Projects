///////////////////////////////////////////////////////////////////////
// $Id$
//
// Provides the framework for graphics projects. Opens a window and
// hooks up all GUI callbacks.
//
// Gary Herron
//
// Copyright © 2007 DigiPen Institute of Technology
////////////////////////////////////////////////////////////////////////

#include "framework.h"
#include "geomlib.h"
#include "transformation.h"
#include "scenelib.h"

// Some globals used by the GUI framework.
int mouseX, mouseY;
bool leftDown = false;
bool middleDown = false;
bool rightDown = false;

#ifdef VIEWCELLS
float spin = 0.0;
float tilt = 0.0;
float zoom = -32.0;
float persp = 0.1f;
#else
float spin = -135.0;
float tilt = -60.0;
float zoom = 0.25;
float persp = 0.4f;
#endif
float translatex = 0.0;
float translatey = 0.0;

Scene scene;

////////////////////////////////////////////////////////////////////////
// Sets the scene's viewing and projection matrices AND their inverses
// according to the parameters controlled by mouse interactions.
void BuildProjection(Scene& scene, int width, int height)
{
	double sx, sy;
	if(width < height) {
		sx = persp;
		sy = sx * height / width; }
	else {
		sy = persp;
		sx = sy * width / height; }

	scene.projection.Identity();
	scene.projection.Perspective(float(sx), float(sy), 0.1f, 100.0f);
	scene.projection.ComputeInverse();

	scene.viewing.Identity();
	scene.viewing.Translate(0.0f, 0.0f, -3.0f);
	scene.viewing.Translate(0.01f*translatex, -0.01f*translatey, 0.25f*zoom);
	scene.viewing.RotateX(tilt);
	scene.viewing.RotateZ(spin);
	scene.viewing.ComputeInverse();

}

////////////////////////////////////////////////////////////////////////
// Displays the current scene as described by the parameters in the
// scene structure.
void ReDrawScene()
{
	int viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	int width = viewport[2];
	int height = viewport[3];

	glClearColor(1.0f,1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, width, 0, height, 0, -1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	BuildProjection(scene, width, height);
	glDisable(GL_DEPTH_TEST);
	if (scene.UseBlend) glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	DrawScene(scene, width, height);
	if (scene.UseBlend) glDisable(GL_BLEND);

	glutSwapBuffers();
}

////////////////////////////////////////////////////////////////////////
// Called by GLut when the window size is changed.
void ReshapeWindow(int w, int h)
{
	if (w && h) {
		glViewport(0, 0, w, h); }
}

////////////////////////////////////////////////////////////////////////
// Called by GLut when there are keyboard messages.
void KeyboardInput(unsigned char key, int, int)
{

	switch(key) {
	case 'g':
		scene.UseOpenGLRendering = !scene.UseOpenGLRendering;
		glutPostRedisplay();
		break;
	case 'w':
		scene.DrawEdges = !scene.DrawEdges;
		glutPostRedisplay();
		break;
	case 't':
		scene.UseBlend = !scene.UseBlend;
		glutPostRedisplay();
		break;
	case 27:					// Escape key
	case 'q':
		exit(EXIT_SUCCESS);
	}
}

////////////////////////////////////////////////////////////////////////
// Called by GLut when a mouse button changes state.
// button:	0 (GL_LEFT_BUTTON)	 for a left button click
//			1 (GL_MIDDLE_BUTTON) for a wheel click
//			2 (GL_RIGHT_BUTTON)	 for a right button click
//			3					 for a wheel roll up
//			4					 for a wheel roll down
//
// state: 0 button went down
//		  1 button went up
//
// x,y: The position of the mouse.
//
// Note: a scroll wheel action will generate two calls here, the first
// with state == 0 and the second with state == 1, and both with
// button equal to 3 or 4 depending on the direction of scroll.
void MouseButton(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON) {
		leftDown = (state == GLUT_DOWN);
#ifdef VIEWCELLS
		Interact(scene, x, y, leftDown);
#endif
	}

	if (button == GLUT_MIDDLE_BUTTON)
		middleDown = (state == GLUT_DOWN);

	if (button == GLUT_RIGHT_BUTTON)
		rightDown = (state == GLUT_DOWN);

	if (button == 3)
		zoom += 1;

	if (button == 4)
		zoom -= 1;

	mouseX = x;
	mouseY = y;

	scene.UseFastRendering = leftDown || middleDown || rightDown;

	glutPostRedisplay();
}

////////////////////////////////////////////////////////////////////////
// Called by GLut when a mouse moves (while a button is down)
void MouseMotion(int x, int y)
{
	// This uses changes in x mouse position to spin the model around
	// its z axis, and changes in the y axis to tilt the model forward
	// or backward.
#ifdef VIEWCELLS
	if(leftDown) {
		Interact(scene, x, y, leftDown); }

	if(middleDown) {
		spin += (x - mouseX);
		tilt += (y - mouseY); }
#else
	if(leftDown) {
		spin += (x - mouseX);
		tilt += (y - mouseY); }
#endif

	if(rightDown) {
		translatex += (x - mouseX);
		translatey += (y - mouseY); }

	// Record this position 
	mouseX = x;
	mouseY = y;

	// Draw the scene, transformed by the new spin and tilt values.
	glutPostRedisplay();

}

////////////////////////////////////////////////////////////////////////
// Called once by the main program to do the OpenGL/GLut setup and
// then enter the event loop.
int main(int argc, char** argv)
{
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(480, 480);
    glutInit(&argc, argv);
	glutCreateWindow("Class Framework");

	CreateScene(scene);
	PreprocessScene(scene);

	glutDisplayFunc(&ReDrawScene);
	glutReshapeFunc(&ReshapeWindow);
	glutKeyboardFunc(&KeyboardInput);
	glutMouseFunc(&MouseButton);
	glutMotionFunc(&MouseMotion);
	glutPostRedisplay();

	// Enter the event loop.
	glutMainLoop();

	DestroyScene(scene);
}

