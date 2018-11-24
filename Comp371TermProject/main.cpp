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

	std::string file = "sceneCube.txt";
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

			// RAY BEING CAST FROM CAMERA

			//e
			vector<float> rayOrigin = { camera.position[0], camera.position[1], camera.position[2] + camera.focalLength };
			//s
			vector<float> pixelPosition = { -static_cast<float>(width) / 2 + x, static_cast<float>(height) / 2 - y, camera.position[2] };
			// s-e
			vector<float> rayDirection = normalize(subtr(pixelPosition, rayOrigin));
			Ray ray = Ray(camera.position, rayDirection);

			float tMin = 9999999999.0f;
			int track = 0;
			for (int i = 0; i < objects.size(); i++) {

				if (objects[i]->name.compare("plane") == 0) {
					float t = static_cast<Plane*>(objects[i])->findIntersection(ray);
					if (t >= 0) {
						if (t < tMin) {
							tMin = t;
							track = i;
						}
					}
				}
				if (objects[i]->name.compare("sphere") == 0) {
					float t = static_cast<Sphere*>(objects[i])->findIntersection(ray);
					if (t >= 0) {
						if (t < tMin) {
							tMin = t;
							track = i;
						}
					}
				}
			}
			// IF RAY HAS HIT AN OBJECT, AND tMin IS THE INTERSECTION VALUE RETURNED FOR CLOSEST OBJECT
			if (tMin < 9999999999.0f) {

				// Ray intersection position P with offset
				std::vector<float> offset = { 0.0001f, 0.0001f, 0.0001f };
				std::vector<float> P = add(add(camera.position, scalarMulti(tMin, rayDirection)), offset);

				// Calculate normal N and retrieve alpha (shininess) depending on object type 
				std::vector<float> N;
				float alpha = 0;

				if (objects[track]->name.compare("sphere") == 0) {
					N = normalize(subtr(P, static_cast<Sphere*>(objects[track])->position));
					alpha = static_cast<Sphere*>(objects[track])->shininess;
				}
				if (objects[track]->name.compare("plane") == 0) {
					N = normalize(subtr(static_cast<Plane*>(objects[track])->normal, static_cast<Plane*>(objects[track])->position));
					alpha = static_cast<Plane*>(objects[track])->shininess;
				}

				//Adapted multiple light modelling code from Assignment 3.
				for (int i = 0; i < 1; i++) {
					Ray shadowRay = Ray(P, normalize(subtr(lights[i]->position, P)));

					// Light Direction 
					std::vector<float> lightDir = normalize(subtr(lights[i]->position, P));
					// Specular
					std::vector<float> reflectDir = normalize(subtr(lightDir, scalarMulti(2 * scalarProduct(N, lightDir), N)));
					// View direction
					std::vector<float> viewDir = normalize(subtr(P, camera.position));
					
				


					vector<float> result = add(add(scalarMulti(fmax(scalarProduct(lightDir, N), 0.0f), vecMulti(objects[track]->diffuse, lights[i]->diffuse)),
						scalarMulti(pow(fmax(scalarProduct(reflectDir, viewDir), 0.0f), alpha), (vecMulti(objects[track]->specular, lights[i]->specular)))), 
						vecMulti(objects[track]->ambient,lights[i]->ambient)
						);
						
					/// TEST
//375, 360
					if (x == 375 && y == 360) {
						cout << "1: " << endl;
						vectorToString(P);
						cout << "Norm: " << endl;
						vectorToString(N);
						cout << "Color: " << endl;
						vectorToString(result);
						cout << tMin << endl;
					}

					//375, 470
					if (x == 375 && y == 470) {
						cout << "2: " << endl;
						vectorToString(P);
						cout << "Norm: " << endl;
						vectorToString(N);
						cout << "Color: " << endl;
						vectorToString(result);
						cout << tMin << endl;
					}

					if (x == 775 && y == 280) {
						cout << "3: " << endl;
						vectorToString(P);
						cout << "Norm: " << endl;
						vectorToString(N);
						cout << "Color: " << endl;
						vectorToString(result);
						cout << tMin << endl;
					}

					if (x == 990 && y == 460) {
						cout << "4: " << endl;
						vectorToString(P);
						cout << "Norm: " << endl;
						vectorToString(N);
						cout << "Color: " << endl;
						vectorToString(result);
						cout << tMin << endl;
					}
					// Shadow ray intersection loop
					for (int j = 0; j < objects.size(); j++) {

						if (objects[j]->name.compare("sphere") == 0) {
							float t = static_cast<Sphere*>(objects[j])->findIntersection(shadowRay);
							// If shadowRay is obstructed by object, black for shadow
							if (t >= 0) {
								
								float color[] = { 0.75f*result[0], 0.75f*result[1], 0.75f*result[2] };
								image.draw_point(x, y, color);
								break;
							}
							// Else green for unobstructed sphere or plane
							else {
								//float color[] = { 0.0f, 0.0f, 0.0f };
								float color[] = { result[0], result[1], result[2] };
								image.draw_point(x, y, color);
							}
						}
					}
				}




			}
			/////////
			//if (tMin < 9999999999.0f) {
			//	float color[] = { objects[track]->ambient[0], objects[track]->ambient[1], objects[track]->ambient[2] };
			//	image.draw_point(x, y, color);
			//}
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