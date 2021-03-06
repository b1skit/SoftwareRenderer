// Renderer object
// By Adam Badke

#ifndef MYRENDERER_H
#define MYRENDERER_H

// Assignment includes:
#include "drawable.h"

// My includes:
#include "polygon.h"
#include "vertex.h"
#include "line.h"
#include "mesh.h"
#include "transformationmatrix.h"
#include "light.h"
#include "scene.h"

// Custom renderer class
class Renderer{
public:
    // Constructor
    Renderer(Drawable* newDrawable, int newXRes, int newYRes, int borderWidth);

    // Destructor
    ~Renderer();

    // Draw a rectangle. Used for setting panel background colors only. Ignores z-buffer.
    // Pre-condition: topLeft_ and botRight_ coords are valid, and in UI window space (ie. (0,0) is in the top left of the screen!)
    void drawRectangle(int topLeftX, int topLeftY, int botRightX, int botRightY, unsigned int color);

    // Render a scene
    void renderScene(Scene theScene);

    // Visually debug the renderer's collection of lights
    void debugLights();

private:
    Drawable* drawable; // A drawable object, used to interface with QT framework

    // Raster settings:
    int border;         // Screen border width
    int xRes;           // Calculated horizontal raster resolution
    int yRes;           // Calculated vertical raster resolution

    int** ZBuffer;               // Z Depth buffer
    int maxZVal = std::numeric_limits<int>::max();    // Max possible z-depth value

    // The current scene, mesh & polgyon objects being drawn (used to access various render variables)
    Scene* currentScene;
    Mesh* currentMesh;
    Polygon* currentPolygon;

    // A transformation matrix from world to camera space
    TransformationMatrix worldToCamera;

    // Perspective transformation: Takes an object in camera space, and adds perspective
    TransformationMatrix cameraToPerspective;   // Assembled in the constructor

    // A world to screen transformation matrix
    TransformationMatrix perspectiveToScreen;   // Assembled in the constructor

    // A transformation matrix from screen space back to perspective space
    TransformationMatrix screenToPerspective;


    // Draw a line
    // If theLine's p1.color != p2.color, the line color will be LERP'd. Updates the Z-Buffer.
    void drawLine(Line theLine, ShadingModel theShadingModel, bool doAmbient, double specularCoefficient, double specularExponent);

    // Draw a polygon. Calls the rasterize Polygon helper function
    // If thePolygon vertices are all not the same color, the color will be LERP'd
    // Pre-condition: All polygons are in camera space
    void drawPolygon(Polygon thePolygon, bool isWireframe);

    // Draw a polygon in wireframe only
    void drawPolygonWireframe(Polygon* thePolygon);

    // Draw a mesh object
    void drawMesh(Mesh* theMesh);

    // Draw a scanline, with consideration to the Z-Buffer.
    // Pre-condition: start and end vertices are in left to right order
    // Note: LERP's if start.color != end.color. Does NOT update the screen!
    void drawScanlineIfVisible(Vertex* start, Vertex* end);

    // Draw a scanline with per-pixel phong lighting, with consideration to the Z-Buffer
    void drawPerPxLitScanlineIfVisible(Vertex* start, Vertex* end, bool doAmbient, double specularCoefficient, double specularExponent);

    // Reset the depth buffer
    void resetDepthBuffer();

    // Change the frustum shape
    // Precondition: currentScene != nullptr
    void transformCamera(TransformationMatrix cameraMovement);

    // Calculate result of overlaying a pixel
    // Written color = opacity * color + (1 - opacity) * color at (x, y)
    // Return: An unsigned int, representing a blended translucent pixel value
    unsigned int blendPixelValues(int x, int y, unsigned int color, float opacity);

    // Override: Lerp be#Filter:3tween the color values of 2 points (Perspective correct: Takes Z-Depth into account)
    // Return: An unsigned int color value, calculated based on a LERP of the current position between the 2 provided points
    unsigned int getPerspCorrectLerpColor(Vertex* p1, Vertex* p2, double ratio) const;

    // Calculate a lighting value for a given pixel on a line between 2 points
    // Pre-condition: Ambient lighting has already been applied to the vertex color values
    unsigned int getFogPixelValue(Vertex* p1, Vertex* p2, double ratio, double correctZ);

    // Calculate interpolated pixel and depth fog value
    unsigned int getDistanceFoggedColor(unsigned int pixelColor, double correctZ);

    // Set a pixel on the raster
    // Pre-condition: Point is a valid coordinate on the raster canvas and has been previously checked against the z-buffer
    void setPixel(int x, int y, double z, unsigned int color);

    // Draw a polygon using opacity
    // If thePolygon vertices are all not the same color, the color will be LERP'd
    void rasterizePolygon(Polygon* thePolygon);

    // Light a Polygon using flat shading
    // Pre-condition: All vertices have a valid normal
    void flatShadePolygon(Polygon* thePolygon);

    // Light a Polygon using gouraud shading
    // Pre-condition: All vertices have a valid normal
    void gouraudShadePolygon(Polygon* thePolygon);

    // Light a given point in camera space
    unsigned int lightPointInCameraSpace(Vertex* currentPosition, NormalVector* viewVector, bool doAmbient, double specularExponent, double specularCoefficient);

    // Recursively ray trace a point's lighting
    unsigned int recursivelyLightPointInCS(Vertex* currentPosition, NormalVector* viewVector, bool doAmbient, double specularExponent, double specularCoefficient, int bounceRays, bool isEndPoint);

    // Recursive helper function for ray tracing
    unsigned int recursiveLightHelper(Vertex* currentPosition, NormalVector* viewVector, bool doAmbient, double specularExponent, double specularCoefficient, int bounceRays, bool isEndPoint);

    // Check if a pixel coordinate is in front of the current z-buffer depth
    bool isVisible(int x, int y, double z);

    // Get a scaled z-buffer value for a given Z
    int getScaledZVal(double correctZ);

    // Determine whether a current position is shadowed by some polygon in the scene that lies between it and a light
    bool isShadowed(Vertex currentPosition, NormalVector* lightDirection, double lightDistance);

    // Find the intersection point of a ray and the plane of a polygon
    // Return: True if the ray intersects, false otherwise. Modifies result Vertex to be the point of intersection, leaves it unchanged otherwise
    bool getPolyPlaneBackFaceIntersectionPoint(Vertex* currentPosition, NormalVector* currentDirection, Vertex* planePoint, NormalVector* planeNormal, Vertex* result);

    // Find the intersection point of a ray and the plane of a polygon
    // Return: True if the ray intersects, false otherwise. Modifies result Vertex to be the point of intersection, leaves it unchanged otherwise
    bool getPolyPlaneFrontFaceIntersectionPoint(Vertex* currentPosition, NormalVector* currentDirection, Vertex* planePoint, NormalVector* planeNormal, Vertex* result);

    // Find the intersection point of a ray and the plane of a polygon
    // Return: True if the ray intersects, false otherwise. Modifies result Vertex to be the point of intersection, leaves it unchanged otherwise
    bool getPolyPlaneIntersectionPoint(Vertex* currentPosition, NormalVector* currentDirection, Vertex* planePoint, NormalVector* planeNormal, Vertex* result);

    // Determine whether a point on a polygon's plane lies within the polygon
    bool pointIsInsidePoly(Polygon* thePolygon, Vertex* intersectionPoint);

    // Calculate the reflection of vector pointing away from a surface
    NormalVector reflectOutVector(NormalVector* faceNormal, NormalVector* outVector);

    // Update a raytracing intersection point with interpolated normals and color values
    void setInterpolatedIntersectionValues(Vertex* intersectionPoint, Polygon* hitPoly);

    // Check if two polygons share an edge
    bool haveSharedEdge(Polygon* poly1, Polygon* poly2);

    // Check if the angle between 2 polygon faces that share an edge is greater than 180 degrees
    bool isFaceReflexAngle(Polygon* currentPoly, Polygon* hitPoly);
};

#endif // MYRENDERER_H
