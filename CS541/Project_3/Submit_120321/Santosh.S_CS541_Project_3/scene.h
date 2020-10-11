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
    // Some user controllable parameters
    int mode; // Communicated to the shaders as "mode".  Keys '0'-'9'
	bool showPass1; // Boolean to display result of pass 1;  Key 's'
	int nSpheres; // Controls the number of shperoids in scene; Keys '-', '+'

    // Viewing transformation parameters;  Mouse buttons 1-3
    float front;
    float eyeSpin;
    float eyeTilt;
    float translatex;
    float translatey;
    float zoom;

    // Light position parameters;  Mouse buttons SHIFT 1-3
    float lightSpin;
    float lightTilt;
    float lightDist;
    
    // The displaylists containing the objects to be drawn
    int objectDL, envDL, sphereDL;

    // Viewport
    int width, height;

};

void InitializeScene(Scene &scene);
void BuildScene(Scene &scene);
void DrawScene(Scene &scene);
