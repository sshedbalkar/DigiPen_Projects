////////////////////////////////////////////////////////////////////////
// The scene class contains all the parameters needed to define and
// draw the (really) simple scene, including:
//   * Geometry (in a display list)
//   * Light parameters
//   * Material properties
//   * Viewport size parameters
//   * Viewing transformation values
//   * others ...
//
// Some of these parameters are set when the scene is built, and
// others are set by the framework in response to user mouse/keyboard
// interactions.  All of them should be used to draw the scene.

class Scene
{
public:
    // An control integer communicated directly to the shaders.
    int mode;

	// Allow for switching models
    int model;

    // The displaylist containing the object to be drawn
    int objectDisplayList;

    // Viewing transformation parameters
    float front;
    float eyeSpin;
    float eyeTilt;
    float translatex;
    float translatey;
    float zoom;

    // Light position parameters
    float lightSpin;
    float lightTilt;
    float lightDist;
    
    // Lighting values
    float ambient[4];
    float lightI[4];
    
    // Material properties
    float Kd[2][4];
    float Ks[4];
    int shininess;
    
    // Viewport
    int width, height;

    // Boolean to toggle shader use on/off (hooked to the 's' key).
    bool UseShader;
};

void InitializeScene(Scene &scene);
void DrawScene(Scene &scene);
