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


// Globals used by the GUI framework.
void (*dragFn)(const int xMouse, const int yMouse) = NULL;

Scene scene;


////////////////////////////////////////////////////////////////////////
// Displays the current scene as described by the VertexTable vertices
// and the PolygonTable polygons and transformed by values spin and
// tilt.
void ReDrawScene()
{
	int viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	int width = viewport[2];
	int height = viewport[3];

	glClearColor(1.0,1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, width, 0, height, 0, -1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	BuildProjection(scene, width, height);
	glDisable(GL_DEPTH_TEST);
	DrawScene(scene, width, height);

	glutSwapBuffers();
}

void Update()
{
        ReDrawScene();
		glutIdleFunc(NULL);
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
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
		sceneID = key - '0';
		scene.Clear();
	    CreateScene(scene);
	    PreprocessScene(scene);
		glutIdleFunc(&Update);
	    break;
	case 'g':
		scene.UseOpenGLRendering = !scene.UseOpenGLRendering;
		glutIdleFunc(&Update);
		break;
	case 'a':
		scene.UseAntiAliasing = !scene.UseAntiAliasing;
		glutIdleFunc(&Update);
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
	// Record the up/down status of each mouse button
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		dragFn = LeftButtonDrag;
		LeftButtonDown(x,y); }
	else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		dragFn = NULL;
		LeftButtonUp(x,y); }

	else if (button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN) {
		dragFn = MiddleButtonDrag;
		MiddleButtonDown(x,y); }
	else if (button == GLUT_MIDDLE_BUTTON && state == GLUT_UP) {
		dragFn = NULL;
		MiddleButtonUp(x,y); }

	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		dragFn = RightButtonDrag;
		RightButtonDown(x,y); }
	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_UP) {
		dragFn = NULL;
		RightButtonUp(x,y); }

	else if (button == 3 && state == GLUT_DOWN) {
		dragFn = NULL;
		MouseWheelUp(); }
	else if (button == 4 && state == GLUT_DOWN) {
		dragFn = NULL;
		MouseWheelDown(); }

	else
		dragFn = NULL;

	glutIdleFunc(&Update);
}

////////////////////////////////////////////////////////////////////////
// Called by GLut when a mouse moves (while a button is down)
void MouseMotion(int x, int y)
{
	if (dragFn)
		dragFn(x,y);

	// Draw the scene, transformed by the new viewing parameters.
	glutIdleFunc(&Update);

}

////////////////////////////////////////////////////////////////////////
// Called once by the main program to do the OpenGL/GLut setup and
// then enter the interactive loop.
int main(int argc, char** argv)
{
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(480, 480);
    glutInit(&argc, argv);
	glutCreateWindow("Class Framework");

	glutDisplayFunc(&ReDrawScene);
	glutReshapeFunc(&ReshapeWindow);
	glutKeyboardFunc(&KeyboardInput);
	glutMouseFunc(&MouseButton);
	glutMotionFunc(&MouseMotion);
	glutIdleFunc(&Update);

	CreateScene(scene);
	PreprocessScene(scene);

	// This function enters an event loop, and never returns.
	glutMainLoop();
}

