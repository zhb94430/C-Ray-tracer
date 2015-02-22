//Image Manipulation Skeleton Code
//
//
//  main.c
//  original by Wagner Correa, 1999
//  modified by Robert Osada, 2000
//  modified by Renato Werneck, 2003
//  mmodified by Stephen J. Guy, 2010

//This project uses the EasyBMP Cross-Platform Windows Bitmap Library
//Web page: http://easybmp.sourceforge.net 


#include "image.h"
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "EasyBMP.h"
#include "readFile.h"
using namespace std;

/**
 * prototypes
 **/
static void ShowUsage(void);
static void CheckOption(char *option, int argc, int minargc);

int main( int argc, char* argv[] ){
	Image *img = NULL;
	bool did_output = false;
    string output_image;

	// first argument is program name
	argv++, argc--;

	// look for help
	for (int i = 0; i < argc; i++) {
		if (!strcmp(argv[i], "-help")) {
			ShowUsage();
		}
	}

	// no argument case
	if (argc == 0) {
		ShowUsage();
	}

	// parse arguments
	while (argc > 0)
	{
		if (**argv == '-')
		{
			if (!strcmp(*argv, "-input"))
			{
				CheckOption(*argv, argc, 2);
                
                ifstream readIn(argv[1]);
                string line;
                
                //output_image & max_depth set to default
                output_image = "raytraced.bmp";
                int max_depth = 5;
                bool jitter = false;
                bool adaptive = false;
                double width = 640;
				double height = 480;
                camera defaultCameraObject(0,0,0,0,0,1,0,1,0,45,width/height);
                material defaultMaterial(0,0,0,1,1,1,0,0,0,5,0,0,0,1);
                ambient_light defaultAmbient(0,0,0);
                
                //Assignment 3⬇️
                int max_vertices = 0;
                int max_normals = 0;
                //Assignment 3⬆️
                
                if (img != NULL)
                    delete img;
                img = new Image(width, height);
                
                
                if (readIn.is_open()) {
                    
                    //First while loop to go through the file and grab basic
                    //info to construct the Image object
                    while (getline(readIn, line)) {
                        if (line.find("film_resolution") == 0) {
                            string cmd = "film_resolution";
                            string data = line.substr(cmd.length()+1);
                            stringstream ss(data);
                            
                            ss >> width >> height;
                            
                            film_resolution filmData(width, height);
                            
                            if (img != NULL)
                                delete img;
                            img = new Image(width, height);
                            
//                            cout << "filmres" << '\n';
//                            cout << filmData.width << '\n';
//                            cout << filmData.height << '\n';
                        }
                        
                        if (line.find("output_image") == 0) {
                            string cmd = "output_image";
                            output_image = line.substr(cmd.length()+1);
                            
                            //Get rid of the hidden \n at the end of file name
                            output_image.erase(output_image.length()-1);
                            
                                                        cout << "output_image" << '\n';
                                                        cout << output_image << '\n';
                        }
                        
                        //Assignment 3⬇️
                        
                        if (line.find("max_vertices") == 0) {
                            string cmd = "max_vertices";
                            string data = line.substr(cmd.length()+1);
                            stringstream ss(data);
                            
                            ss >> max_vertices;
                        }
                        
                        if (line.find("max_normals") == 0) {
                            string cmd = "max_normals";
                            string data = line.substr(cmd.length()+1);
                            stringstream ss(data);
                            
                            ss >> max_normals;
                        }
                        
                        //Assignment 3⬆️
                    }
                    
                    //Prepare the cursor to read throught the file again
                    readIn.clear();
                    readIn.seekg(0, readIn.beg);

                    //Second while loop to grab the scene info and store in the
                    //constructed Image object
                    while (getline(readIn, line)) {
                        if (line[0] == '#') {
                            continue;
                        }
                        
                        if (line.find("camera") == 0) {
                            string cmd = "camera";
                            string data = line.substr(cmd.length()+1);
                            stringstream ss(data);
                            
                            double px, py, pz;
                            double dx, dy, dz;
                            double ux, uy, uz;
                            double ha;
                            
                            ss >> px >> py >> pz >> dx >> dy >> dz >> ux >> uy >> uz >> ha;
                            
                            double ratio = width/height;
							camera cameraObject(px,py,pz,dx,dy,dz,ux,uy,uz,ha,ratio);
                            
                            img->addCamera(cameraObject);
//                            cout << "camera" << '\n';
//                            cout << cameraObject.px << '\n';
//                            cout << cameraObject.py << '\n';
//                            cout << cameraObject.pz << '\n';
//                            cout << cameraObject.dx << '\n';
//                            cout << cameraObject.dy << '\n';
//                            cout << cameraObject.dz << '\n';
//                            cout << cameraObject.ux << '\n';
//                            cout << cameraObject.uy << '\n';
//                            cout << cameraObject.uz << '\n';
//                            cout << cameraObject.ha << '\n';
                        }
                        
                        if (line.find("sphere") == 0) {
                            string cmd = "sphere";
                            string data = line.substr(cmd.length()+1);
                            stringstream ss(data);
                            
                            double x,y,z,r;
                            
                            ss >> x >> y >> z >> r;
                            
							sphere sphereData(x, y, z, r);
                            
                            img->addSphere(sphereData);
//                            cout << "sphere" << '\n';
//                            cout << sphereData.x << '\n';
//                            cout << sphereData.y << '\n';
//                            cout << sphereData.z << '\n';
//                            cout << sphereData.r << '\n';
                        }
                        
                        if (line.find("background") == 0) {
                            string cmd = "background";
                            string data = line.substr(cmd.length()+1);
                            stringstream ss(data);
                            
                            double r,g,b;
                            
                            ss >> r >> g >> b;
                            
							background bgData(r, g, b);
                            img->addBackground(bgData);
//                            cout << "bg" << '\n';
//                            cout << bgData.r << '\n';
//                            cout << bgData.g << '\n';
//                            cout << bgData.b << '\n';
                        }
                        
                        if (line.find("material") == 0) {
                            string cmd = "material";
                            string data = line.substr(cmd.length()+1);
                            stringstream ss(data);
                            
                            double ar, ag, ab;
                            double dr, dg, db;
                            double sr, sg, sb;
                            double ns;
                            double tr, tg, tb;
                            double ior;
                            
                            ss >> ar >> ag >> ab >> dr >> dg >> db >> sr >> sg >> sb >> ns >> tr >> tg >> tb >> ior;
                            
							material materialData(ar, ag, ab, dr, dg, db, sr, sg, sb, ns, tr, tg, tb, ior);
                            
                            img->addMaterial(materialData);
//                            cout << "material" << '\n';
//                            cout << materialData.ar << '\n';
//                            cout << materialData.ag << '\n';
//                            cout << materialData.ab << '\n';
//                            cout << materialData.dr << '\n';
//                            cout << materialData.dg << '\n';
//                            cout << materialData.db << '\n';
//                            cout << materialData.sr << '\n';
//                            cout << materialData.sg << '\n';
//                            cout << materialData.sb << '\n';
//                            cout << materialData.ns << '\n';
//                            cout << materialData.tr << '\n';
//                            cout << materialData.tg << '\n';
//                            cout << materialData.tb << '\n';
//                            cout << materialData.ior << '\n';
                        }
                        
                        if (line.find("directional_light") == 0) {
                            string cmd = "directional_light";
                            string data = line.substr(cmd.length()+1);
                            stringstream ss(data);
                            
                            double r, g, b;
                            double x, y, z;
                            
                            ss >> r >> g >> b >> x >> y >> z;
                            
							directional_light dirLightData(r, g, b, x, y, z);
                            
                            img->addDirLight(dirLightData);
//                            cout << "dir_light" << '\n';
//                            cout << dirLightData.r << '\n';
//                            cout << dirLightData.g << '\n';
//                            cout << dirLightData.b << '\n';
//                            cout << dirLightData.x << '\n';
//                            cout << dirLightData.y << '\n';
//                            cout << dirLightData.z << '\n';
                        }
                        
                        if (line.find("point_light") == 0) {
                            string cmd = "point_light";
                            string data = line.substr(cmd.length()+1);
                            stringstream ss(data);
                            
                            double r, g, b;
                            double x, y, z;
                            
                            ss >> r >> g >> b >> x >> y >> z;
                            
							point_light pntLightData(r, g, b, x, y, z);
                            
                            img->addPntLight(pntLightData);
//                            cout << "pnt_light" << '\n';
//                            cout << pntLightData.r << '\n';
//                            cout << pntLightData.g << '\n';
//                            cout << pntLightData.b << '\n';
//                            cout << pntLightData.x << '\n';
//                            cout << pntLightData.y << '\n';
//                            cout << pntLightData.z << '\n';
                        }
                        
                        if (line.find("spot_light") == 0) {
                            string cmd = "spot_light";
                            string data = line.substr(cmd.length()+1);
                            stringstream ss(data);
                            
                            double r, g, b;
                            double px, py, pz;
                            double dx, dy, dz;
                            double angle1, angle2;
                            
                            ss >> r >> g >> b >> px >> py >> pz >> dx >> dy >> dz >> angle1 >> angle2;
                            
							spot_light spotLightData(r, g, b, px, py, pz, dx, dy, dz, angle1, angle2);
                            
                            img->addSpotLight(spotLightData);
//                            cout << "spot_light" << '\n';
//                            cout << spotLightData.r << '\n';
//                            cout << spotLightData.g << '\n';
//                            cout << spotLightData.b << '\n';
//                            cout << spotLightData.px << '\n';
//                            cout << spotLightData.py << '\n';
//                            cout << spotLightData.pz << '\n';
//                            cout << spotLightData.dx << '\n';
//                            cout << spotLightData.dy << '\n';
//                            cout << spotLightData.dz << '\n';
//                            cout << spotLightData.angle1 << '\n';
//                            cout << spotLightData.angle2 << '\n';
                        }
                        
                        if (line.find("ambient_light") == 0) {
                            string cmd = "ambient_light";
                            string data = line.substr(cmd.length()+1);
                            stringstream ss(data);
                            
                            double r,g,b;
                            
                            ss >> r >> g >> b;
                            
							ambient_light ambientLightData(r, g, b);
                            
                            img->addAmbientLight(ambientLightData);
//                            cout << "am_light" << '\n';
//                            cout << ambientLightData.r << '\n';
//                            cout << ambientLightData.g << '\n';
//                            cout << ambientLightData.b << '\n';
                        }
                        
                        if (line.find("max_depth") == 0) {
                            string cmd = "max_depth";
                            string data = line.substr(cmd.length()+1);
                            stringstream ss(data);
                            
                            ss >> max_depth;
                            
                            img->addMaxDepth(max_depth);
//                            cout << "mdepth" << '\n';
//                            cout << max_depth << '\n';
                        }
                        
                        if (line.find("jitter") == 0) {
                            string cmd = "jitter";
                            string data = line.substr(cmd.length()+1);
                            stringstream ss(data);
                            
                            ss >> jitter;
                            
                            cout << "jitter" << jitter << endl;
                            
                            img->addJitter(jitter);
                            //                            cout << "mdepth" << '\n';
                            //                            cout << max_depth << '\n';
                        }
                        
                        if (line.find("adaptive") == 0) {
                            string cmd = "adaptive";
                            string data = line.substr(cmd.length()+1);
                            stringstream ss(data);
                            
                            ss >> adaptive;
                            
                            img->addAdaptive(adaptive);
                            //                            cout << "mdepth" << '\n';
                            //                            cout << max_depth << '\n';
                        }
                        
                        //Assignment 3⬇️
                        if (line.find("vertex") == 0) {
                            string cmd = "vertex";
                            string data = line.substr(cmd.length()+1);
                            stringstream ss(data);
                            
                            double x, y, z;
                            
                            ss >> x >> y >> z;
                            
                            vertex vertexData(x, y, z);
                            
                            img->addVertex(vertexData);
//                            cout << "vertex" << endl;
//                            cout << vertexData.x << endl;
//                            cout << vertexData.y << endl;
//                            cout << vertexData.z << endl;
                        }
                        
                        if (line.find("normal") == 0) {
                            string cmd = "normal";
                            string data = line.substr(cmd.length()+1);
                            stringstream ss(data);
                            
                            double x, y, z;
                            
                            ss >> x >> y >> z;
                            
                            normal normalData(x, y, z);
                            
                            img->addNormal(normalData);
//                            cout << "normal" << endl;
//                            cout << normalData.x << endl;
//                            cout << normalData.y << endl;
//                            cout << normalData.z << endl;
                        }
                        
                        if (line.find("triangle") == 0) {
                            string cmd = "triangle";
                            string data = line.substr(cmd.length()+1);
                            stringstream ss(data);
                            
                            int index1, index2, index3;
                            
                            ss >> index1 >> index2 >> index3;
                            
                            triangle triangleData(img->vertexList[index1],
                                                  img->vertexList[index2],
                                                  img->vertexList[index3]);
                            
                            img->addTriangle(triangleData);
//                            cout << "triangle" << endl;
//                            cout << triangleData.v1.x << endl;
//                            cout << triangleData.v1.y << endl;
//                            cout << triangleData.v1.z << endl;
//                            cout << triangleData.v2.x << endl;
//                            cout << triangleData.v2.y << endl;
//                            cout << triangleData.v2.z << endl;
//                            cout << triangleData.v3.x << endl;
//                            cout << triangleData.v3.y << endl;
//                            cout << triangleData.v3.z << endl;
                        }
                        
                        if (line.find("normalTriangle") == 0) {
                            string cmd = "normalTriangle";
                            string data = line.substr(cmd.length()+1);
                            stringstream ss(data);
                            
                            int index1, index2, index3, index4, index5, index6;
                            
                            ss >> index1 >> index2 >> index3 >> index4 >> index5 >> index6;
                            
                            normalTriangle normalTriangleData(img->vertexList[index1],
                                                  img->vertexList[index2],
                                                  img->vertexList[index3],
                                                  img->normalList[index4],
                                                  img->normalList[index5],
                                                  img->normalList[index6]);
                            
                            img->addNormalTriangle(normalTriangleData);
//                            cout << "normal triangle" << endl;
//                            cout << normalTriangleData.v1.x << endl;
//                            cout << normalTriangleData.v1.y << endl;
//                            cout << normalTriangleData.v1.z << endl;
//                            cout << normalTriangleData.n1.x << endl;
//                            cout << normalTriangleData.n1.y << endl;
//                            cout << normalTriangleData.n1.z << endl;
//                            cout << normalTriangleData.v2.x << endl;
//                            cout << normalTriangleData.v2.y << endl;
//                            cout << normalTriangleData.v2.z << endl;
//                            cout << normalTriangleData.n2.x << endl;
//                            cout << normalTriangleData.n2.y << endl;
//                            cout << normalTriangleData.n2.z << endl;
//                            cout << normalTriangleData.v3.x << endl;
//                            cout << normalTriangleData.v3.y << endl;
//                            cout << normalTriangleData.v3.z << endl;
//                            cout << normalTriangleData.n3.x << endl;
//                            cout << normalTriangleData.n3.y << endl;
//                            cout << normalTriangleData.n3.z << endl;
                        }
                        //Assignment 3⬆️
                    }
                    readIn.close();
                }
				
                //Debug code to verify the scene has been established
//                int i;
//                
//                for (i = 0; i < img->sphereList.size(); i++) {
//                    cout << "in sphere" << '\n';
//                    cout << img->sphereList[i].x << '\n';
//                }
//                
//                for (i = 0; i < img->cameraList.size(); i++) {
//                    cout << "in camera" << '\n';
//                    cout << img->cameraList[i].px << '\n';
//                }
//                
//                for (i = 0; i < img->materialList.size(); i++) {
//                    cout << "in material" << '\n';
//                    cout << img->materialList[i].ar << '\n';
//                }
//                
//                for (i = 0; i < img->dirLightList.size(); i++) {
//                    cout << "in dirLight" << '\n';
//                    cout << img->dirLightList[i].r << '\n';
//                }
//                
//                for (i = 0; i < img->pntLightList.size(); i++) {
//                    cout << "in pntLight" << '\n';
//                    cout << img->pntLightList[i].r << '\n';
//                }
//                
//                for (i = 0; i < img->spotLightList.size(); i++) {
//                    cout << "in spotLight" << '\n';
//                    cout << img->spotLightList[i].r << '\n';
//                }
//                
//                for (i = 0; i < img->ambientLightList.size(); i++) {
//                    cout << "in ambientLight" << '\n';
//                    cout << img->ambientLightList[i].r << '\n';
//                }
                
                //Default check
                if (img->cameraList.size() == 0) {
                    img->addCamera(defaultCameraObject);
                }
                
                if (img->materialList.size() == 0) {
                    img->addMaterial(defaultMaterial);
                }
                
                if (img->ambientLightList.size() == 0) {
                    img->addAmbientLight(defaultAmbient);
                }
                
                img->render();
                
				argv += 2, argc -= 2;
			}
            
            else
            {
                fprintf(stderr, "image: invalid option: %s\n", *argv);
                ShowUsage();
            }

//			else if (!strcmp(*argv, "-output"))
//			{
//				CheckOption(*argv, argc, 2);
//				if (img == NULL) ShowUsage();
//                
                if (output_image.length() != 0) {
                    char output_image_char_array[output_image.length()];
                    strcpy(output_image_char_array, output_image.c_str());
                    img->Write(output_image_char_array);
                    did_output = true;
                }
                else {
                    img->Write(argv[1]);
                    did_output = true;
                }
				argv += 2, argc -= 2;
//			}

			
		}
	}

	if (!did_output)
	{
		fprintf( stderr, "Warning, you didn't tell me to output anything.  I hope that's OK.\n" );
	}

	delete img;
	return EXIT_SUCCESS;
}


/**
 * ShowUsage
 **/
static char options[] =
"-help\n"
"-input <file>\n"
"-output <file>\n"
"-noise <factor>\n"
"-brightness <factor>\n"
"-contrast <factor>\n"
"-saturation <factor>\n"
"-crop <x> <y> <width> <height>\n"
"-extractChannel <channel no>\n"
"-quantize <nbits>\n"
"-randomDither <nbits>\n"
"-blur <maskSize>\n"
"-sharpen <maskSize>\n"
"-edgeDetect\n"
"-orderedDither <nbits>\n"
"-FloydSteinbergDither <nbits>\n"
"-scale <sx> <sy>\n"
"-rotate <angle>\n"
"-fun\n"
"-sampling <method no>\n"
;

static void ShowUsage(void)
{
	fprintf(stderr, "Usage: image -input <filename> [-option [arg ...] ...] -output <filename>\n");
	fprintf(stderr, options);
	exit(EXIT_FAILURE);
}



/**
 * CheckOption
 **/
static void CheckOption(char *option, int argc, int minargc)
{
	if (argc < minargc)
	{
		fprintf(stderr, "Too few arguments for %s\n", option);
		ShowUsage();
	}
}