#include <iostream>
#include <vector>

#include "Renderable.h"
#include "FileParser.h"
#include "Ray.h"
#include "CImg.h"
using namespace std;

using namespace cimg_library;


vector<Renderable*> objects;
Camera camera;

void vectorToString(vector<float>);
void printObjects();
void printCamera(); 
void renderingLoop(int, int);

int main() {
	std::string file = "sceneCube.txt";
	parseFile(file, objects, camera);
	printCamera();
	printObjects(); 

	cout << "Rendering..." << endl; 
	renderingLoop(800, 600);
	cout << "Finished." << endl;

	return 0; 
}

void renderingLoop(int width, int height) {
	CImg<float> image(width, height, 1, 3, 0);

	std::vector<float> X{ 1.0f, 0.0f, 0.0f };
	std::vector<float> Y{ 0.0f, 1.0f, 0.0f };
	std::vector<float> Z{ 0.0f, 0.0f, 1.0f };

	std::vector<float> lookAt{ 0.0f, 0.0f, 0.0f };

	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {

//////// Ray tracing and collision / RGB decision-making goes here. 

			//Test, assigns random RGB to each pixel. 
			float color[] = { static_cast <float> (rand()) / static_cast <float> (RAND_MAX),
				static_cast <float> (rand()) / static_cast <float> (RAND_MAX),
				static_cast <float> (rand()) / static_cast <float> (RAND_MAX) };
			image.draw_point(x, y, color);
				
				
		}
	}
	image.normalize(0, 255);
	image.save("render.bmp");
	cimg_library::CImgDisplay main_disp(image, "Render");
	while (!main_disp.is_closed()) { main_disp.wait(); 
	
	}
}





void vectorToString(vector<float> vector) {
	cout << vector[0] << " " << vector[1] << " " << vector[2] << endl;
}

void printCamera() {
	cout << "camera" << endl;
	vectorToString(camera.position);
	cout << camera.fieldOfView << endl;
	cout << camera.focalLength << endl;
	cout << camera.aspectRatio << endl;
	cout << endl;
}

void printObjects() {
	for (int i = 0; i < objects.size(); i++) {
		if ((objects[i]->name).compare("plane") == 0) {
			cout << objects[i]->name << endl;
			vectorToString(static_cast<Plane*>(objects[i])->normal);
			vectorToString(static_cast<Plane*>(objects[i])->position);
			vectorToString(static_cast<Plane*>(objects[i])->ambient);
			vectorToString(static_cast<Plane*>(objects[i])->diffuse);
			vectorToString(static_cast<Plane*>(objects[i])->specular);
			cout << static_cast<Plane*>(objects[i])->shininess << endl;
			cout << endl;
		}
		else if ((objects[i]->name).compare("sphere") == 0) {
			cout << objects[i]->name << endl;
			vectorToString(static_cast<Sphere*>(objects[i])->position);
			cout << static_cast<Sphere*>(objects[i])->radius << endl;
			vectorToString(static_cast<Sphere*>(objects[i])->ambient);
			vectorToString(static_cast<Sphere*>(objects[i])->diffuse);
			vectorToString(static_cast<Sphere*>(objects[i])->specular);
			cout << static_cast<Sphere*>(objects[i])->shininess << endl;
			cout << endl;
		}
		else if ((objects[i]->name).compare("model") == 0) {
			cout << objects[i]->name << endl;
			cout << static_cast<Model*>(objects[i])->file << endl;
			vectorToString(static_cast<Model*>(objects[i])->ambient);
			vectorToString(static_cast<Model*>(objects[i])->diffuse);
			vectorToString(static_cast<Model*>(objects[i])->specular);
			cout << static_cast<Model*>(objects[i])->shininess << endl;
			cout << endl;
		}
		else if ((objects[i]->name).compare("light") == 0) {
			cout << objects[i]->name << endl;
			vectorToString(static_cast<Light*>(objects[i])->position);
			vectorToString(static_cast<Light*>(objects[i])->ambient);
			vectorToString(static_cast<Light*>(objects[i])->diffuse);
			vectorToString(static_cast<Light*>(objects[i])->specular);
			cout << endl;
		}
	}



}