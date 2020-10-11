 ///////////////////////////////////////////////////////////////////////
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
        glEnable(GL_DEPTH_TEST);

        // Turn off OpenGL's transformation machinery since we'll be
        // using the student's implementation of transformations.
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
    
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();

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

        glClear(GL_COLOR_BUFFER_BIT); // Clear the screen

        // Loop through the scene's objects
        for (unsigned int k=0;  k<scene.objects.size();  k++) {
            Object& obj = scene.objects[k];

            // Set this object's material properties
            glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, obj.Kd.rgba);
            glMateriali(GL_FRONT, GL_SHININESS, obj.n);
            glMaterialfv(GL_FRONT, GL_SPECULAR, obj.Ks.rgba);
        
            // Loop through the object's polygons
            for (int j=0;  j< (int)obj.polygons.size();  j++) {
                APolygon& poly = obj.polygons[j];
            
                // Set OpenGL's polygon mode
                glBegin(GL_POLYGON);
            
                // Loop through the polygon's  vertices.
                for (int i=0;  i< (int)poly.size(); i++)  {
                    // Send the normal to OpenGL (for lighting calculations).
                    glNormal3fv(poly[i].N.begin());

                    // Transform the vertex and send to OpenGL
                    Vector4D P = poly[i].V;
                    Vector4D T = scene.viewing.Transform(P);
                    T = scene.projection.Transform(T);
                    Vector3D S = T.Hdiv();
                    glVertex3fv(S.begin());
                }
            
                // End openGL's polygon mode
                glEnd(); 

            }
        }

        // @TODO: Create a useful message
        DrawMessage("Rendered by <student>", width, height);
    }
}
