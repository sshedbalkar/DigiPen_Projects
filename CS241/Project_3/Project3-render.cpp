q///////////////////////////////////////////////////////////////////////
// $Id$
//
// All student written rendering code can go in this file.
//
// Gary Herron
//
// Copyright Å© 2007 DigiPen Institute of Technology
////////////////////////////////////////////////////////////////////////

#include "framework.h"
#include "geomlib.h"
#include "transformation.h"
#include "scenelib.h"

////////////////////////////////////////////////////////////////////////
// This is called once after the scene is created but before it is
// ever drawn to allow for any pre-processing steps.
void PreprocessScene(Scene& scene)
{
}


////////////////////////////////////////////////////////////////////////
// Procedure DrawScene is called whenever the scene needs to be drawn.
void DrawScene(Scene& scene, int width, int height)
{
    // Choose OpenGL or student rendering here.  The decision can be
    // based on useOpenGLRendering, useFastRendering, or both:

    // if (useOpenGLRendering || useFastRendering)  {
    if (scene.UseOpenGLRendering)  {
        glEnable(GL_DEPTH_TEST);
        DrawSceneWithOpenGL(scene, width, height);
        glDisable(GL_DEPTH_TEST);
        DrawMessage("Rendered by OpenGL (toggle with 'g')", width, height);
    }

    // Student rendering code goes here
    else {
        //!! Turn off Depth testing (since you'll do your own).
        glDisable(GL_DEPTH_TEST);

        glClear(GL_COLOR_BUFFER_BIT); // Clear the screen

        //@@ Allocate, or if necessary, resize, an existing Z-Buffer
        //@@ Clear Z-buffer to all 1.0's

        // Turn off OpenGL's transformation machinery since we'll be
        // using the student's implementation of transformations.
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
    
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();

        //!! Setup screen coordinates for pixel addressing.
        gluOrtho2D(0,width,0,height);
        glPointSize(5.0);  //@@ Change this to 1 point size of 1.0!!

        // Set some lighting parameters for OpenGL
        glEnable(GL_LIGHTING);
        glEnable(GL_DEPTH_TEST);
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, scene.ambient.rgba);
        glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, 1);
        for (unsigned int li=0;  li<scene.lights.size();  li++) {
            Point3D& P3 = scene.lights[li].position;
            Vector4D P4(P3[0], P3[1], P3[2], 0); // OpenGL needs a 4 vector
            glLightfv(GL_LIGHT0+li, GL_POSITION, P4.begin());
            glLightfv(GL_LIGHT0+li, GL_DIFFUSE,  scene.lights[li].color.rgba);
            glLightfv(GL_LIGHT0+li, GL_SPECULAR, scene.lights[li].color.rgba);
            glEnable(GL_LIGHT0+li); }

        // Loop through the scene's objects
        for (unsigned int k=0;  k<scene.objects.size();  k++) {
            Object& obj = scene.objects[k];

            // Send this object's material properties to OpenGL (for lighting)
            glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, obj.Kd.rgba);
            glMateriali(GL_FRONT, GL_SHININESS, obj.n);
            glMaterialfv(GL_FRONT, GL_SPECULAR, obj.Ks.rgba);
        
            // Loop through the object's polygons
            for (int j=0;  j< (int)obj.polygons.size();  j++) {
                APolygon& poly = obj.polygons[j];
            
                // Set OpenGL's drawing mode
                glBegin(GL_POINTS); //!! Pixel drawing mode; was GL_POLYGON
            
                // Loop through the polygon's vertices.
                //@@ Create an empty Polygon P
                for (int i=0;  i< (int)poly.size(); i++)  {
                    // Send the normal to OpenGL (for lighting calculations).
                    glNormal3fv(poly[i].N.begin());

                    // Transform the vertex and send to OpenGL
                    Vector4D P = poly[i].V;
                    Vector4D T = scene.viewing.Transform(P);
                    T = scene.projection.Transform(T);
                    //@@ Store vertex T in polygon in polygon P.
                    glVertex2f(T[0]/T[3]*width/2+width/2, T[1]/T[3]*height/2+height/2); //@@ REMOVE ME!!
                }

                //@@ Clip the polygon P

                //@@ Convert clipped vertices to pixel coords (include the Hdiv)
                //@@ Split the polygon into triangles
                //@@ Scan convert each triangle, resulting in individual pixels
                //@@ Depth-test and depth-update each pixel
                //@@ Output individual pixel with glVertex2f(px,py)
            
                // End openGL's drawing mode
                glEnd(); 

            }
        }

        //@@ Create a useful message
        DrawMessage("Rendered by <student>", width, height);
    }
}
