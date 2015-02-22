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
                        }
                        
                        if (line.find("sphere") == 0) {
                            string cmd = "sphere";
                            string data = line.substr(cmd.length()+1);
                            stringstream ss(data);
                            
                            double x,y,z,r;
                            
                            ss >> x >> y >> z >> r;
                            
							sphere sphereData(x, y, z, r);
                            
                            img->addSphere(sphereData);
                        }
                        
                        if (line.find("background") == 0) {
                            string cmd = "background";
                            string data = line.substr(cmd.length()+1);
                            stringstream ss(data);
                            
                            double r,g,b;
                            
                            ss >> r >> g >> b;
                            
							background bgData(r, g, b);
                            img->addBackground(bgData);
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
                        }
                        
                        if (line.find("ambient_light") == 0) {
                            string cmd = "ambient_light";
                            string data = line.substr(cmd.length()+1);
                            stringstream ss(data);
                            
                            double r,g,b;
                            
                            ss >> r >> g >> b;
                            
							ambient_light ambientLightData(r, g, b);
                            
                            img->addAmbientLight(ambientLightData);
                        }
                        
                        if (line.find("max_depth") == 0) {
                            string cmd = "max_depth";
                            string data = line.substr(cmd.length()+1);
                            stringstream ss(data);
                            
                            ss >> max_depth;
                            
                            img->addMaxDepth(max_depth);
                        }
                        
                        if (line.find("jitter") == 0) {
                            string cmd = "jitter";
                            string data = line.substr(cmd.length()+1);
                            stringstream ss(data);
                            
                            ss >> jitter;
                            
                            cout << "jitter" << jitter << endl;
                            
                            img->addJitter(jitter);
                        }
                        
                        if (line.find("adaptive") == 0) {
                            string cmd = "adaptive";
                            string data = line.substr(cmd.length()+1);
                            stringstream ss(data);
                            
                            ss >> adaptive;
                            
                            img->addAdaptive(adaptive);
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
                        }
                        
                        if (line.find("normal") == 0) {
                            string cmd = "normal";
                            string data = line.substr(cmd.length()+1);
                            stringstream ss(data);
                            
                            double x, y, z;
                            
                            ss >> x >> y >> z;
                            
                            normal normalData(x, y, z);
                            
                            img->addNormal(normalData);
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
                        }
                        //Assignment 3⬆️
                    }
                    readIn.close();
                }
                
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