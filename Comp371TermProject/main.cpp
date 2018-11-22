#include <iostream>
#include <vector>
#include <cmath>
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

#define PI 3.14159265

int main() {

	std::string file = "scene5.txt";
	parseFile(file, objects, camera);
	printCamera();
	printObjects();
	int height = static_cast<int>(2 * camera.focalLength*tan(camera.fieldOfView / 2 * PI / 180.0f));
	int width = static_cast<int>(camera.aspectRatio * height);
	cout << "Rendering..." << endl;
	renderingLoop(width, height);
	cout << "Finished." << endl;

	return 0;
}

void renderingLoop(int width, int height) {
	CImg<float> image(width, height, 1, 3, 0);

	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			vector<float> rayOrigin = { camera.position[0], camera.position[1], camera.position[2] + camera.focalLength };
			vector<float> pixelPosition = { -static_cast<float>(width) / 2 + x, static_cast<float>(height) / 2 - y, camera.position[2] };
			vector<float> rayDirection = { pixelPosition[0] - rayOrigin[0],
				pixelPosition[1] - rayOrigin[1],
				pixelPosition[2] - rayOrigin[2] };
			Ray ray = Ray(rayOrigin, rayDirection);

			//////// Ray tracing and collision / RGB decision-making goes here. 

						////Test, assigns random RGB to each pixel. 

							//float color[] = { static_cast <float> (rand()) / static_cast <float> (RAND_MAX),
							//static_cast <float> (rand()) / static_cast <float> (RAND_MAX),
							//static_cast <float> (rand()) / static_cast <float> (RAND_MAX) };




			for (int i = 0; i < objects.size(); i++) {
				//if (objects[i]->name.compare("plane") == 0)
				//	if (static_cast<Plane*>(objects[i])->findIntersection(ray, camera.position) == 1)
				//		image.draw_point(x, y, color2);

				if (objects[i]->name.compare("sphere") == 0)
					if (static_cast<Sphere*>(objects[i])->findIntersection(ray, camera.position) == 1) {
						float color[] = { static_cast<Sphere*>(objects[i])->ambient[0], static_cast<Sphere*>(objects[i])->ambient[1], static_cast<Sphere*>(objects[i])->ambient[2] };
						image.draw_point(x, y, color);
					}
			}





		}
	}
	image.normalize(0, 255);
	image.save("render.bmp");
	cimg_library::CImgDisplay main_disp(image, "Render");
	while (!main_disp.is_closed()) {
		main_disp.wait();

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