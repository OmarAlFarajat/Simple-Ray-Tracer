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
vector<Light*> lights;


void vectorToString(vector<float>);
void printObjects();
void printCamera();
void renderingLoop(int, int);

#define PI 3.14159265

int main() {

	std::string file = "scene5.txt";
	parseFile(file, objects, lights, camera);
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
			vector<float> rayDirection = subtr(pixelPosition, rayOrigin);
			//{ pixelPosition[0] - rayOrigin[0],
			//	pixelPosition[1] - rayOrigin[1],
			//	pixelPosition[2] - rayOrigin[2] };
			Ray ray = Ray(rayOrigin, rayDirection);

			float tMin = 9999999999.0f;
			int track = 0;
			for (int i = 0; i < objects.size(); i++) {

				if (objects[i]->name.compare("plane") == 0) {
					float t = static_cast<Plane*>(objects[i])->findIntersection(ray, camera.position);
					if (t > 0) {
						if (t < tMin) {
							tMin = t;
							track = i;
						}
					}
				}
				if (objects[i]->name.compare("sphere") == 0) {
					float t = static_cast<Sphere*>(objects[i])->findIntersection(ray, camera.position);
					if (t > 0) {

						if (t < tMin) {
							tMin = t;
							track = i;
						}
					}
				}
			}
			//std::vector<float> pixel_color = { 0,0,0 };

			//for (int i = 0; i < lights.size(); i++) {

			//	if (objects[track]->name.compare("sphere") == 0) {
			//		std::vector<float> P = add(rayOrigin, scalarMulti(tMin, rayDirection));
			//		std::vector<float> N = normalize(subtr(P, static_cast<Sphere*>(objects[track])->position));
			//		Ray shadowRay = Ray(N, lights[i]->position);
			//		for (int j = 0; j < objects.size(); j++) {
			//			if (objects[j]->name.compare("plane") == 0) {
			//				float t = static_cast<Plane*>(objects[i])->findIntersection(shadowRay, N);
			//				if (t < 0) {
			//					pixel_color = scalarMulti(fmax(0.0f, scalarProduct(N, subtr(lights[i]->position, P))), add(pixel_color, lights[i]->diffuse));
			//					float color[] = { pixel_color[0], pixel_color[1], pixel_color[2] };
			//					image.draw_point(x, y, color);
			//				}
			//			}
			//			if (objects[j]->name.compare("sphere") == 0) {
			//				float t = static_cast<Sphere*>(objects[i])->findIntersection(shadowRay, N);
			//				if (t < 0) {
			//					pixel_color = scalarMulti(fmax(0.0f, scalarProduct(N, subtr(lights[i]->position, P))), add(pixel_color, lights[i]->diffuse));
			//					float color[] = { pixel_color[0], pixel_color[1], pixel_color[2] };
			//					image.draw_point(x, y, color);
			//				}
			//			}
			//		}
			//	}
			//}
			/////////
			if (tMin < 9999999999.0f) {
				float color[] = { objects[track]->ambient[0], objects[track]->ambient[1], objects[track]->ambient[2] };
				image.draw_point(x, y, color);
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
	}
	for (int i = 0; i < lights.size(); i++) {

		cout << lights[i]->name << endl;
		vectorToString(lights[i]->position);
		vectorToString(lights[i]->ambient);
		vectorToString(lights[i]->diffuse);
		vectorToString(lights[i]->specular);
		cout << endl;
	}
}