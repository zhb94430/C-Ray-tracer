//Image.h
//
//Class representing an image
//  original by Wagner Correa, 1999
//  turned to C++ by Robert Osada, 2000

#ifndef IMAGE_INCLUDED
#define IMAGE_INCLUDED

#include <assert.h>
#include <stdio.h>
#include "pixel.h"
#include "sceneClasses.h"
#include "EasyBMP.h"
#include <vector>

/**
 * constants
 **/
enum {
    IMAGE_SAMPLING_POINT,
    IMAGE_SAMPLING_BILINEAR,
    IMAGE_SAMPLING_GAUSSIAN,
    IMAGE_N_SAMPLING_METHODS
};

enum {
    IMAGE_CHANNEL_RED,
    IMAGE_CHANNEL_GREEN,
    IMAGE_CHANNEL_BLUE,
    IMAGE_CHANNEL_ALPHA,
    IMAGE_N_CHANNELS
};


/**
 * Image
 **/
class Image
{
public:
    Pixel *pixels;
    int width, height, num_pixels;
    int sampling_method;
	BMP* bmpImg;
    double inf = std::numeric_limits<double>::infinity();
    
    int maxDepth;
    int counter;
    bool jitter;
    bool adaptive;
    background bgData;
    std::vector<sphere> sphereList;
    std::vector<camera> cameraList;
    std::vector<material> materialList;
    std::vector<directional_light> dirLightList;
    std::vector<point_light> pntLightList;
    std::vector<spot_light> spotLightList;
    std::vector<ambient_light> ambientLightList;
    std::vector<vertex> vertexList;
    std::vector<normal> normalList;
    std::vector<triangle> triangleList;
    std::vector<normalTriangle> normalTriangleList;
    //Assignment 3⬆️
    // Creates a blank image with the given dimensions
    Image (int width, int height);

    // Copy iamage
    Image (const Image& src);

	// Make image from file
	Image(char *fname);

    // Destructor
    ~Image ();

    // Pixel access
    int ValidCoord (int x, int y)  const { return x>=0 && x<width && y>=0 && y<height; }
    Pixel& GetPixel (int x, int y) const { assert(ValidCoord(x,y)); return pixels[y*width + x]; }

    // Dimension access
    int Width     () const { return width; }
    int Height    () const { return height; }
    int NumPixels () const { return num_pixels; }

	// Make file from image
	void Write( char *fname );

    // Sets the sampling method.
    void SetSamplingMethod(int method);

    // Sample image using current sampling method.
    Pixel Sample(double u, double v);
    
    //---------------------------
    //Raytracer Code
    //---------------------------
    
    //Store scene information
    
    void addSphere(sphere inputObject);
    void addCamera(camera inputObject);
    void addBackground(background inputObject);
    void addMaterial(material inputObject);
    void addDirLight(directional_light inputObject);
    void addPntLight(point_light inputObject);
    void addSpotLight(spot_light inputObject);
    void addAmbientLight(ambient_light inputObject);
    void addMaxDepth(int inputObject);
    void addJitter(bool inputObject);
    void addAdaptive(bool inputObject);
    
    //Assignment 3⬇️
    void addVertex(vertex inputObject);
    void addNormal(normal inputObject);
    void addTriangle(triangle inputObject);
    void addNormalTriangle(normalTriangle inputObject);
    //Assignment 3⬆️
    
    //"Main function" for image class to output the result
    void render();
    
//    //Set of helper functions
    double RTIntersect(RTRay ray, sphere object);
    
    //Assignment 3⬇️
    double RTIntersectTri(RTRay ray, triangle object);
    bool hit(RTRay ray);
    RTRay getReflectionRay(RTRay incoming, hitPoint hpt);
//    RTRay getReflectionRay(RTRay incoming, hitPoint hpt);
    colorFactor applyLightingModel(RTRay ray, hitPoint hpt, camera defaultCamera, int counter, int itemIndex, int itemIndexTri);
    colorFactor evaluateRay(int itemIndex, int itemIndexTri, camera defaultCamera, int materialIndex, RTRay tempRay, double t, int counter);
    //Assignment 3⬆️
    
    //Set of rendering functions
    
//    void setBackground();
    void basicShape();
    void jitterSampling();
    void adaptiveSampling();
};
#endif
