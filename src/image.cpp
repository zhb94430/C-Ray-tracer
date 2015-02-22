#include "image.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <limits>
#include <algorithm>

/**
 * Image
 **/
using namespace std;

Image::Image (int width_, int height_)
{
    bmpImg = new BMP();
    
    assert(width_ > 0);
    assert(height_ > 0);
    
    width           = width_;
    height          = height_;
    num_pixels      = width * height;
    sampling_method = IMAGE_SAMPLING_POINT;
    
    if (!bmpImg->SetSize(width, height)){
        printf("Error allocating image.");
        exit(-1);
    }
    
    pixels = new Pixel[num_pixels];
    int c = 0;
    for (int j = 0; j < height; j++){
        for (int i = 0; i < width; i++){
            pixels[c].r = bmpImg->GetPixel(i,j).Red;
            pixels[c].g = bmpImg->GetPixel(i,j).Green;
            pixels[c].b = bmpImg->GetPixel(i,j).Blue;
            pixels[c].a = bmpImg->GetPixel(i,j).Alpha;
            c++;
        }
    }
    
    assert(pixels != NULL);
}


Image::Image (const Image& src)
{
    bmpImg = new BMP();
    
    width           = src.width;
    height          = src.height;
    num_pixels      = width * height;
    sampling_method = IMAGE_SAMPLING_POINT;
    
    RangedPixelToPixelCopy( *src.bmpImg, 0, width-1,
                           height-1 , 0,
                           *bmpImg, 0,0 );
    
    if (!bmpImg->SetSize(width, height)){
        printf("Error allocating image.");
        exit(-1);
    }
    
    pixels = new Pixel[num_pixels];
    int c = 0;
    for (int j = 0; j < height; j++){
        for (int i = 0; i < width; i++){
            pixels[c].r = bmpImg->GetPixel(i,j).Red;
            pixels[c].g = bmpImg->GetPixel(i,j).Green;
            pixels[c].b = bmpImg->GetPixel(i,j).Blue;
            pixels[c].a = bmpImg->GetPixel(i,j).Alpha;
            c++;
        }
    }
    
    assert(pixels != NULL);
}

Image::Image (char* fname){
    bmpImg = new BMP();
    
    if (!bmpImg->ReadFromFile(fname)){
        printf("Error loading image: %s", fname);
        exit(-1);
    }
    
    width = bmpImg->TellWidth();
    height = bmpImg->TellHeight();
    num_pixels = width * height;
    sampling_method = IMAGE_SAMPLING_POINT;
    
    pixels = new Pixel[num_pixels];
    int c = 0;
    for (int j = 0; j < height; j++){
        for (int i = 0; i < width; i++){
            pixels[c].r = bmpImg->GetPixel(i,j).Red;
            pixels[c].g = bmpImg->GetPixel(i,j).Green;
            pixels[c].b = bmpImg->GetPixel(i,j).Blue;
            pixels[c].a = bmpImg->GetPixel(i,j).Alpha;
            c++;
        }
    }
}

Image::~Image (){
    delete bmpImg;
    pixels = NULL;
}

void Image::Write(char* fname){
    int c = 0;
    for (int j = 0; j < height; j++){
        for (int i = 0; i < width; i++){
            RGBApixel p;
            p.Red = pixels[c].r;
            p.Green = pixels[c].g;
            p.Blue = pixels[c].b;
            p.Alpha = pixels[c].a;
            bmpImg->SetPixel(i,j,p);
            c++;
        }
    }
    bmpImg->SetBitDepth(24);
    bmpImg->WriteToFile(fname);
}

//---------------------------
//Raytracer Code
//---------------------------

//Store scene information

void Image::addSphere (sphere inputObject) {
    sphereList.push_back(inputObject);
}

void Image::addCamera (camera inputObject) {
    cameraList.push_back(inputObject);
}

void Image::addBackground (background inputObject) {
    bgData = inputObject;
}

void Image::addMaterial (material inputObject) {
    materialList.push_back(inputObject);
}

void Image::addDirLight (directional_light inputObject) {
    dirLightList.push_back(inputObject);
}

void Image::addPntLight (point_light inputObject) {
    pntLightList.push_back(inputObject);
}

void Image::addSpotLight (spot_light inputObject) {
    spotLightList.push_back(inputObject);
}

void Image::addAmbientLight (ambient_light inputObject) {
    ambientLightList.push_back(inputObject);
}

void Image::addMaxDepth (int inputObject) {
    maxDepth = inputObject;
}

void Image::addJitter (bool inputObject) {
    jitter = inputObject;
}

void Image::addAdaptive (bool inputObject) {
    adaptive = inputObject;
}

//Assignment 3⬇️
void Image::addVertex(vertex inputObject) {
    vertexList.push_back(inputObject);
}

void Image::addNormal(normal inputObject) {
    normalList.push_back(inputObject);
}

void Image::addTriangle(triangle inputObject) {
    triangleList.push_back(inputObject);
}

void Image::addNormalTriangle(normalTriangle inputObject) {
    normalTriangleList.push_back(inputObject);
}
//Assignment 3⬆️

//Rendering functions

//Check if the ray and sphere intersect
double Image::RTIntersect(RTRay ray, sphere object) {
    //Check the discriminant
    double discriminant = pow(RTDot(ray.direction, RTMinus(ray.origin, object.center)) ,2) - RTDot(ray.direction, ray.direction) * (RTDot(RTMinus(ray.origin, object.center), RTMinus(ray.origin, object.center)) - pow(object.r, 2));
    
    if (discriminant > 0 || discriminant == 0) {
        //Calculate the actual solution
        double s1 = (RTDot(RTMinus(RTVector(0, 0, 0), ray.direction), RTMinus(ray.origin, object.center)) + sqrt(discriminant))/RTDot(ray.direction, ray.direction);
        
        double s2 = (RTDot(RTMinus(RTVector(0, 0, 0), ray.direction), RTMinus(ray.origin, object.center)) - sqrt(discriminant))/RTDot(ray.direction, ray.direction);
        
        if (s1 > 0 || s1 == 0 || s2 > 0 || s2 == 0) {
            if (s1 < s2) {
                return s1;
            }
            else if (s2 < s1) {
                return s2;
            }
            else {
                return s1;
            }
        }
        
        return inf;
    }
    
    else {
        return inf;
    }
}

//Assignment 3⬇️
//Check if the ray and triangle intersect
double Image::RTIntersectTri(RTRay ray, triangle object) {
    //********************NASTY MATH
    
    //Making dummy variable for readability
    double a,b,c,d,e,f,g,h,i,j,k,l;
    
    a = object.v1.x - object.v2.x;
    b = object.v1.y - object.v2.y;
    c = object.v1.z - object.v2.z;
    d = object.v1.x - object.v3.x;
    e = object.v1.y - object.v3.y;
    f = object.v1.z - object.v3.z;
    g = ray.direction.x;
    h = ray.direction.y;
    i = ray.direction.z;
    j = object.v1.x - ray.x;
    k = object.v1.y - ray.y;
    l = object.v1.z - ray.z;
    
    double beta, gamma, t, M;
    
    M = a*(e*i-h*f) + b*(g*f-d*i) + c*(d*h-e*g);
    beta = (j*(e*i-h*f) + k*(g*f-d*i) + l*(d*h-e*g))/M;
    gamma = (i*(a*k-j*b) + h*(j*c - a*l) + g*(b*l-k*c))/M;
    t = -(f*(a*k-j*b) + e*(j*c-a*l) + d*(b*l-k*c))/M;
    
    //******************************
    if (t < 0 || t == inf) {
        return inf;
    }
    
    if (gamma < 0 || gamma > 1) {
        return inf;
    }
    
    if (beta < 0 || beta > (1 - gamma)) {
        return inf;
    }
    
    if (RTDot(ray.direction.RTNormalize(), object.normal) == 0) {
        return inf;
    }
    
    return t;
    
}

bool Image::hit(RTRay ray) {
    
    //hit check for sphere
    double current_tS = 0;
    for (int item = 0; item < sphereList.size(); item++) {
        sphere currentItemS = sphereList[item];
        current_tS = RTIntersect(ray, currentItemS);
        
        if (current_tS >= 0 && current_tS != inf) {
            return true;
        }
    }
    
    //hit check for triangle
    double current_tT = 0;
    for (int item = 0; item < triangleList.size(); item++) {
        
        triangle currentItemT = triangleList[item];
        current_tT = RTIntersectTri(ray, currentItemT);
        
        if (current_tT >= 0 && current_tT != inf) {
            return true;
        }
    }
    
    return false;
}

RTRay Image::getReflectionRay(RTRay incoming, hitPoint hpt) {
    
    //Extreme case where incoming is parallel to normal
    incoming.direction = incoming.direction.RTNormalize();
    RTVector normal = hpt.normal.RTNormalize();
    
    if (RTEqual(incoming.direction, normal)) {
        RTRay returnRay = incoming;
        returnRay.direction.x = -returnRay.direction.x;
        returnRay.direction.y = -returnRay.direction.y;
        returnRay.direction.z = -returnRay.direction.z;
        return returnRay;
    }
    
    if (RTDot(incoming.direction, normal) == 0) {
        return incoming;
    }
    
    RTVector returnDirection = RTMinus(incoming.direction, RTMultiC(RTMultiC(normal, RTDot(incoming.direction, normal)), 2));
    
    RTRay returnRay(hpt.xIntersect + 0.001, hpt.yIntersect + 0.001, hpt.zIntersect + 0.001, returnDirection);
    return returnRay;
}

colorFactor Image::applyLightingModel(RTRay ray, hitPoint hpt, camera defaultCamera, int counter, int itemIndex, int itemIndexTri) {
    //Returning colorFactor
    colorFactor returnValue;
    material currentMaterial = materialList[hpt.materialIndex];
    
    //Loop through each light source
    if (dirLightList.size() != 0) {
        for (int i = 0; i < dirLightList.size(); i++) {
            directional_light directionalLightPoint = dirLightList[i];

            //Make the shadow ray
            RTRay shadowRay = RTRay(hpt.xIntersect + 0.001, hpt.yIntersect + 0.001, hpt.zIntersect + 0.001, RTVector(-directionalLightPoint.x, -directionalLightPoint.y, -directionalLightPoint.z));
            
            if (hit(shadowRay)) {
                continue;
            }
            
            //Calculating diffuse lighting
            RTVector reflect(-directionalLightPoint.x, -directionalLightPoint.y, -directionalLightPoint.z);
            RTVector reflectN = RTMultiC(reflect, 1 / (sqrt(pow(reflect.x, 2) + pow(reflect.y, 2) +  pow(reflect.z, 2))));
            double factor = RTDot(hpt.normal, reflectN);
            colorFactor diffuseFactor(currentMaterial.dr * directionalLightPoint.r * max(0.0, factor),
                                      currentMaterial.dg * directionalLightPoint.g * max(0.0, factor),
                                      currentMaterial.db * directionalLightPoint.b * max(0.0, factor));
            
            returnValue.addFactor(diffuseFactor);
            
            //Calculating phong lighting
            RTVector vPlusl = RTAdd(hpt.incoming, reflect);
            RTVector h = RTMultiC(vPlusl, 1 / (sqrt(pow(vPlusl.x, 2) + pow(vPlusl.y, 2) + pow(vPlusl.z, 2))));
            double phongFactor = RTDot(hpt.normal, h);
            double p = 64;
            colorFactor phongFactorR(currentMaterial.sr * directionalLightPoint.r * (pow(max(0.0, phongFactor), p)),
                                    currentMaterial.sg * directionalLightPoint.g * (pow(max(0.0, phongFactor), p)),
                                    currentMaterial.sb * directionalLightPoint.b * (pow(max(0.0, phongFactor), p)));
            returnValue.addFactor(phongFactorR);
        }
    }

    if (pntLightList.size() != 0) {
        for (int i = 0; i < pntLightList.size(); i++) {
            point_light pointLightPoint = pntLightList[i];
            
            //Make the shadow ray
            RTRay shadowRay = RTRay(hpt.xIntersect + 0.001, hpt.yIntersect + 0.001, hpt.zIntersect + 0.001, RTVector(pointLightPoint.x - hpt.xIntersect, pointLightPoint.y - hpt.yIntersect, pointLightPoint.z - hpt.zIntersect));

            //For spheres
            double closest = inf;
            if (sphereList.size() != 0) {
                for (int item = 0; item < sphereList.size(); item++) {
                    if (item == itemIndex) {
                        continue;
                    }
                    sphere currentItem = sphereList[item];
                    double current_t = RTIntersect(shadowRay, currentItem);
                    if (current_t < closest && current_t > 0) {
                        closest = current_t;
                    }
                }
            }
            //For triangles
            if (triangleList.size() != 0) {
                for (int item = 0; item < triangleList.size(); item++) {
                    if (item == itemIndexTri) {
                        continue;
                    }
                    triangle currentItem = triangleList[item];
                    double current_t = RTIntersectTri(shadowRay, currentItem);
                    if (current_t < closest && current_t > 0) {
                        closest = current_t;
                    }
                }
            }

            if (hit(shadowRay) && (RTLength(RTMultiC(shadowRay.direction, closest)) <= RTDistance(hpt.incoming, RTVector(pointLightPoint.x, pointLightPoint.y, pointLightPoint.z)))) {
                continue;
            }
            
            //Calculating diffuse lighting
            RTVector reflect(pointLightPoint.x - hpt.xIntersect, pointLightPoint.y - hpt.yIntersect, pointLightPoint.z - hpt.zIntersect);
            double lightRadius = sqrt(pow(reflect.x, 2) + pow(reflect.y, 2) + pow(reflect.z, 2));
            RTVector reflectN = RTMultiC(reflect, 1 / (sqrt(pow(reflect.x, 2) + pow(reflect.y, 2) + pow(reflect.z, 2))));
            double factor = RTDot(hpt.normal, reflectN);
            colorFactor diffuseFactor(currentMaterial.dr * pointLightPoint.r * (1/(pow(lightRadius, 2))) * max(0.0, factor),
                                      currentMaterial.dg * pointLightPoint.g * (1/(pow(lightRadius, 2))) * max(0.0, factor),
                                      currentMaterial.db * pointLightPoint.b * (1/(pow(lightRadius, 2))) * max(0.0, factor));
            
            returnValue.addFactor(diffuseFactor);
            
            //Calculating phong lighting
            RTVector vPlusl = RTAdd(hpt.incoming, reflect);
            
            RTVector h = RTMultiC(vPlusl, 1 / (sqrt(pow(vPlusl.x, 2) + pow(vPlusl.y, 2) + pow(vPlusl.z, 2))));
            double phongFactor = RTDot(hpt.normal, h);
            double p = 64;
            colorFactor phongFactorR(currentMaterial.sr * pointLightPoint.r * (1/(pow(lightRadius, 2))) * (pow(max(0.0, phongFactor), p)),
                                    currentMaterial.sg * pointLightPoint.g * (1/(pow(lightRadius, 2))) * (pow(max(0.0, phongFactor), p)),
                                    currentMaterial.sb * pointLightPoint.b * (1/(pow(lightRadius, 2))) * (pow(max(0.0, phongFactor), p)));
            returnValue.addFactor(phongFactorR);
            
            closest = inf;
        }
    }
    
    //recursive call for reflection
    RTRay mirrorRay = getReflectionRay(ray, hpt);
    
    
    //Preperation for recursive call
    //Keep track of the index of the visible sphere
    int itemIndexMIR = -1;
    //Keep track of the index of the visible triangle
    int itemIndexTriMIR= -1;
    
    int materialIndexMIR = 0;
    
    double closestMIR = inf;
    
    //INTERSECTING
    //Go through all the objects and find the closest intersecting one
    //For sphere
    
    if (sphereList.size() != 0) {
        materialIndexMIR = sphereList.size();
        
        for (int item = 0; item < sphereList.size(); item++) {
            sphere currentItem = sphereList[item];
            if (item == itemIndex) {
                continue;
            }
            double current_t = RTIntersect(mirrorRay, currentItem);
            
            if (current_t < closestMIR) {
                closestMIR = current_t;
                itemIndexMIR = item;
            }
        }
    }

    //For triangles
    if (triangleList.size() != 0) {
        for (int item = 0; item < triangleList.size(); item++) {
            triangle currentItem = triangleList[item];
            if (item == itemIndexTri) {
                continue;
            }
            double current_t = RTIntersectTri(mirrorRay, currentItem);
            
            if (current_t < closestMIR) {
                closestMIR = current_t;
                itemIndexTriMIR = item;
            }
        }
        
    }
    
    if (!(itemIndexMIR == -1 && itemIndexTriMIR == -1)) {
        colorFactor reflectFactor = (evaluateRay(itemIndexMIR, itemIndexTriMIR, defaultCamera, materialIndexMIR, mirrorRay, closestMIR, counter));
        returnValue.fR += reflectFactor.fR*currentMaterial.sr;
        returnValue.fG += reflectFactor.fG*currentMaterial.sg;
        returnValue.fB += reflectFactor.fB*currentMaterial.sb;
    }
    
    closestMIR = inf;
    
    //Ambient light
    if (ambientLightList.size() != 0) {
        for (int i = 0; i < ambientLightList.size(); i++) {
            ambient_light currentAmbientLight = ambientLightList[i];
            returnValue.addFactor(colorFactor(currentMaterial.ar * currentAmbientLight.r,
                                              currentMaterial.ag * currentAmbientLight.g,
                                              currentMaterial.ab * currentAmbientLight.b));
        }
    }
    return returnValue;
}

colorFactor Image::evaluateRay(int itemIndex, int itemIndexTri, camera defaultCamera, int materialIndex, RTRay tempRay, double t, int counter) {
    
    if (counter > maxDepth) {
        colorFactor bgFactor(bgData.r, bgData.g, bgData.b);
        return bgFactor;
    }
    
    counter++;
    bool hitConfirm = hit(tempRay);
    
    if (hitConfirm) {
        //If it is a triangle
        if (itemIndexTri != -1) {
            hitPoint hpt(tempRay, t, triangleList[itemIndexTri].normal, materialIndex);
            return applyLightingModel(tempRay, hpt, defaultCamera, counter, itemIndex, itemIndexTri);
        }
        
        //Else it's a sphere
        else if (itemIndex != -1){
            hitPoint hpt(tempRay, t, sphereList[itemIndex], itemIndex);
            return applyLightingModel(tempRay, hpt, defaultCamera, counter, itemIndex, itemIndexTri);
        }
        else {
            cout << "Error, not suppose to be happening" << endl;
            cout << itemIndexTri << endl;
            cout << itemIndex << endl;
            return colorFactor(0.5, 0.5, 0);
        }
    }
    else {
        colorFactor bgFactor(bgData.r, bgData.g, bgData.b);
        return bgFactor;
    }
}
//Assignment 3⬆️

void Image::basicShape() {
    //For now, automatically extract the first camera in the list
    camera defaultCamera = cameraList[0];
    double closest = inf;
    
    //Setting the material index for triangles
    int materialIndex = 0;
    
    //Make the film
    film_resolution film(Width(), Height());
    
    //Two for loop to go through the film
    
    for (int i = 0; i < Width(); i++) {
        for (int j = 0; j < Height(); j++) {
            RTRay tempRay = defaultCamera.getRay(i, j, film);
            bool pixelSet = false;
            
            //Keep track of the index of the visible sphere
            int itemIndex = -1;
            //Keep track of the index of the visible triangle
            int itemIndexTri = -1;
            
            //INTERSECTING
            //Go through all the objects and find the closest intersecting one
            //For sphere
            
            if (sphereList.size() != 0) {
                materialIndex = sphereList.size();
                
                for (int item = 0; item < sphereList.size(); item++) {
                    sphere currentItem = sphereList[item];
                    double current_t = RTIntersect(tempRay, currentItem);
                    
                    if (current_t < closest) {
                        closest = current_t;
                        itemIndex = item;
                    }
                }
            }
            
            
            //Assignment 3⬇️
            //For triangles
            if (triangleList.size() != 0) {
                for (int item = 0; item < triangleList.size(); item++) {
                    triangle currentItem = triangleList[item];
                    double current_t = RTIntersectTri(tempRay, currentItem);
                    
                    if (current_t < closest) {
                        closest = current_t;
                        itemIndexTri = item;
                    }
                }

            }
            //Assignment 3⬆️
            
            colorFactor factorValue = evaluateRay(itemIndex, itemIndexTri, defaultCamera, materialIndex, tempRay, closest, 0);
            
            
            GetPixel(i, j).SetClamp(255 * factorValue.fR, 255 * factorValue.fG, 255 * factorValue.fB);
            pixelSet = true;
        
            closest = inf;
            materialIndex = 0;
            
            if (!pixelSet) {
                int r = bgData.r * 255;
                int g = bgData.g * 255;
                int b = bgData.b * 255;
                
                GetPixel(i, j).SetClamp(r, g, b);
            }
        }
    }
}

void Image::jitterSampling() {
    cout << "Getting jittered" << endl;
    Image *sampleImg = new Image(width*2, height*2);
    sampleImg->maxDepth = maxDepth;
    sampleImg->counter = counter;
    sampleImg->jitter = 0;
    sampleImg->adaptive = 0;
    sampleImg->bgData = bgData;
    sampleImg->sphereList = sphereList;
    sampleImg->cameraList = cameraList;
    sampleImg->materialList = materialList;
    sampleImg->dirLightList = dirLightList;
    sampleImg->pntLightList = pntLightList;
    sampleImg->spotLightList = spotLightList;
    sampleImg->ambientLightList = ambientLightList;
    sampleImg->vertexList = vertexList;
    sampleImg->normalList = normalList;
    sampleImg->triangleList = triangleList;
    sampleImg->normalTriangleList = normalTriangleList;
    
    sampleImg->basicShape();
    
    for (int i = 0; i < Width(); i++) {
        for (int j = 0; j < Height(); j++) {
            cout << i << endl;
            //Get the samples
            int r = 0;
            int g = 0;
            int b = 0;
            
            if ((i*2+1) > sampleImg->Width() && (j*2+1) > sampleImg->Height()) {
                r = sampleImg->GetPixel(i*2, j*2).r;
                g = sampleImg->GetPixel(i*2, j*2).g;
                b = sampleImg->GetPixel(i*2, j*2).b;
            }
            
            else if ((i*2+1) > sampleImg->Width()) {
                r = (sampleImg->GetPixel(i*2, j*2).r + sampleImg->GetPixel(i*2, j*2+1).r)/2;
                g = (sampleImg->GetPixel(i*2, j*2).g + sampleImg->GetPixel(i*2, j*2+1).g)/2;
                b = (sampleImg->GetPixel(i*2, j*2).b + sampleImg->GetPixel(i*2, j*2+1).b)/2;
            }
            
            else if ((j*2+1) > sampleImg->Height()) {
                r = (sampleImg->GetPixel(i*2, j*2).r + sampleImg->GetPixel(i*2+1, j*2).r)/2;
                g = (sampleImg->GetPixel(i*2, j*2).g + sampleImg->GetPixel(i*2+1, j*2).g)/2;
                b = (sampleImg->GetPixel(i*2, j*2).b + sampleImg->GetPixel(i*2+1, j*2).b)/2;
            }
            
            else {
                //Generate random jitter index
                int randomIndex = rand() % 3;
                
                if (randomIndex == 0) {
                    r = sampleImg->GetPixel(i*2, j*2).r;
                    g = sampleImg->GetPixel(i*2, j*2).g;
                    b = sampleImg->GetPixel(i*2, j*2).b;
                }
                
                else if (randomIndex == 1) {
                    r = (sampleImg->GetPixel(i*2, j*2).r + sampleImg->GetPixel(i*2+1, j*2).r)/2;
                    g = (sampleImg->GetPixel(i*2, j*2).g + sampleImg->GetPixel(i*2, j*2+1).g)/2;
                    b = (sampleImg->GetPixel(i*2+1, j*2+1).b + sampleImg->GetPixel(i*2, j*2+1).b)/2;
                }
                
                else if (randomIndex == 2) {
                    r = (sampleImg->GetPixel(i*2, j*2).r + sampleImg->GetPixel(i*2+1, j*2+1).r + sampleImg->GetPixel(i*2+1, j*2).r)/3;
                    g = (sampleImg->GetPixel(i*2, j*2).g + sampleImg->GetPixel(i*2, j*2+1).g + sampleImg->GetPixel(i*2+1, j*2).g)/3;
                    b = (sampleImg->GetPixel(i*2, j*2).b + sampleImg->GetPixel(i*2+1, j*2+1).b + sampleImg->GetPixel(i*2, j*2+1).b)/3;
                }
                
                else if (randomIndex == 3) {
                    r = (sampleImg->GetPixel(i*2, j*2).r + sampleImg->GetPixel(i*2+1, j*2+1).r + sampleImg->GetPixel(i*2, j*2+1).r + sampleImg->GetPixel(i*2+1, j*2).r)/4;
                    g = (sampleImg->GetPixel(i*2, j*2).g + sampleImg->GetPixel(i*2+1, j*2+1).g + sampleImg->GetPixel(i*2, j*2+1).g + sampleImg->GetPixel(i*2+1, j*2).g)/4;
                    b = (sampleImg->GetPixel(i*2, j*2).b + sampleImg->GetPixel(i*2+1, j*2+1).b + sampleImg->GetPixel(i*2, j*2+1).b + sampleImg->GetPixel(i*2+1, j*2).b)/4;
                }
            }
            
            GetPixel(i, j).SetClamp(r, g, b);
        }
    }
}

void Image::adaptiveSampling() {
    cout << "Getting adaptiveSampled" << endl;
    Image *sampleImg = new Image(width*2, height*2);
    sampleImg->maxDepth = maxDepth;
    sampleImg->counter = counter;
    sampleImg->jitter = 0;
    sampleImg->adaptive = 0;
    sampleImg->bgData = bgData;
    sampleImg->sphereList = sphereList;
    sampleImg->cameraList = cameraList;
    sampleImg->materialList = materialList;
    sampleImg->dirLightList = dirLightList;
    sampleImg->pntLightList = pntLightList;
    sampleImg->spotLightList = spotLightList;
    sampleImg->ambientLightList = ambientLightList;
    sampleImg->vertexList = vertexList;
    sampleImg->normalList = normalList;
    sampleImg->triangleList = triangleList;
    sampleImg->normalTriangleList = normalTriangleList;
    
    sampleImg->basicShape();
    
    for (int i = 0; i < Width(); i++) {
        for (int j = 0; j < Height(); j++) {
            cout << i << endl;
            //Get the samples
            int r = 0;
            int g = 0;
            int b = 0;
            
            if ((i*2+1) > sampleImg->Width() && (j*2+1) > sampleImg->Height()) {
                r = sampleImg->GetPixel(i*2, j*2).r;
                g = sampleImg->GetPixel(i*2, j*2).g;
                b = sampleImg->GetPixel(i*2, j*2).b;
            }
            
            else if ((i*2+1) > sampleImg->Width()) {
                //Check if sampling is needed
                if ((sampleImg->GetPixel(i*2, j*2).r == sampleImg->GetPixel(i*2, j*2+1).r) && (sampleImg->GetPixel(i*2, j*2).g == sampleImg->GetPixel(i*2, j*2+1).g) && (sampleImg->GetPixel(i*2, j*2).b == sampleImg->GetPixel(i*2, j*2+1).b)){
                            r = sampleImg->GetPixel(i*2, j*2).r;
                            g = sampleImg->GetPixel(i*2, j*2).g;
                            b = sampleImg->GetPixel(i*2, j*2).b;
                }
                else {
                    r = (sampleImg->GetPixel(i*2, j*2).r + sampleImg->GetPixel(i*2, j*2+1).r)/2;
                    g = (sampleImg->GetPixel(i*2, j*2).g + sampleImg->GetPixel(i*2, j*2+1).g)/2;
                    b = (sampleImg->GetPixel(i*2, j*2).b + sampleImg->GetPixel(i*2, j*2+1).b)/2;
                }
            }
            
            else if ((j*2+1) > sampleImg->Height()) {
                //Check if sampling is needed
                if ((sampleImg->GetPixel(i*2, j*2).r == sampleImg->GetPixel(i*2+1, j*2).r) && (sampleImg->GetPixel(i*2, j*2).g == sampleImg->GetPixel(i*2+1, j*2).g) && (sampleImg->GetPixel(i*2, j*2).b == sampleImg->GetPixel(i*2+1, j*2).b)) {
                            r = sampleImg->GetPixel(i*2, j*2).r;
                            g = sampleImg->GetPixel(i*2, j*2).g;
                            b = sampleImg->GetPixel(i*2, j*2).b;
                }
                else {
                    r = (sampleImg->GetPixel(i*2, j*2).r + sampleImg->GetPixel(i*2+1, j*2).r)/2;
                    g = (sampleImg->GetPixel(i*2, j*2).g + sampleImg->GetPixel(i*2+1, j*2).g)/2;
                    b = (sampleImg->GetPixel(i*2, j*2).b + sampleImg->GetPixel(i*2+1, j*2).b)/2;
                }
            }
            
            else {
                //Check if sampling is needed
                if (((sampleImg->GetPixel(i*2, j*2).r == sampleImg->GetPixel(i*2+1, j*2+1).r) && (sampleImg->GetPixel(i*2+1, j*2+1).r == sampleImg->GetPixel(i*2, j*2+1).r) && (sampleImg->GetPixel(i*2, j*2+1).r == sampleImg->GetPixel(i*2+1, j*2).r)) && ((sampleImg->GetPixel(i*2, j*2).g == sampleImg->GetPixel(i*2+1, j*2+1).g) && (sampleImg->GetPixel(i*2+1, j*2+1).g == sampleImg->GetPixel(i*2, j*2+1).g) && (sampleImg->GetPixel(i*2, j*2+1).g == sampleImg->GetPixel(i*2+1, j*2).g)) && ((sampleImg->GetPixel(i*2, j*2).b == sampleImg->GetPixel(i*2+1, j*2+1).b) && (sampleImg->GetPixel(i*2+1, j*2+1).b == sampleImg->GetPixel(i*2, j*2+1).b) && (sampleImg->GetPixel(i*2, j*2+1).b == sampleImg->GetPixel(i*2+1, j*2).b))) {
                            r = sampleImg->GetPixel(i*2, j*2).r;
                            g = sampleImg->GetPixel(i*2, j*2).g;
                            b = sampleImg->GetPixel(i*2, j*2).b;
                }
                else {
                    r = (sampleImg->GetPixel(i*2, j*2).r + sampleImg->GetPixel(i*2+1, j*2+1).r + sampleImg->GetPixel(i*2, j*2+1).r + sampleImg->GetPixel(i*2+1, j*2).r)/4;
                    g = (sampleImg->GetPixel(i*2, j*2).g + sampleImg->GetPixel(i*2+1, j*2+1).g + sampleImg->GetPixel(i*2, j*2+1).g + sampleImg->GetPixel(i*2+1, j*2).g)/4;
                    b = (sampleImg->GetPixel(i*2, j*2).b + sampleImg->GetPixel(i*2+1, j*2+1).b + sampleImg->GetPixel(i*2, j*2+1).b + sampleImg->GetPixel(i*2+1, j*2).b)/4;
                }
            }
            
            GetPixel(i, j).SetClamp(r, g, b);
        }
    }
}

//"Main function" for image class to output the result
void Image::render() {
    if (jitter || adaptive) {
        if (jitter) {
            jitterSampling();
        }
        else if (adaptive) {
            adaptiveSampling();
        }
    }
    else {
    cout << "before basicShape" << endl;
    basicShape();
    cout << "After basicShape" << endl;
    }
}

