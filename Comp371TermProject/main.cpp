#include <iostream>
#include <vector>
#include <cmath>
#include "Renderable.h"
#include "FileParser.h"
#include "Ray.h"
#include "CImg.h"
#include <fstream>
#include "objloader.h"

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
	bool repeat = true;

	while (repeat) {
		std::string file;
		cout << "Please enter the name of a scene file: " << endl;
		cin >> file;

		std::string meshAnswer;
		bool meshDraw = false;
		cout << "Do you want to render meshes in the scene? (yes, or anything else if no)" << endl;
		cin >> meshAnswer;
		cout << endl;
		if (meshAnswer.compare("yes") == 0) {
			meshDraw = true;
			cout << "Drawing with mesh" << endl;
		}
		else {
			meshDraw = false; 
			cout << "Drawing with NO mesh" << endl;
		}

		parseFile(file, objects, lights, camera);

		// Mesh loading is still work-in-progress, hence why I added the option to omit meshes from the scene
		if (meshDraw) {
			for (int x = 0; x < objects.size(); x++)
			{
				if (objects[x]->name.compare("mesh") == 0) {
					std::vector<glm::vec3> vertices;
					std::vector<glm::vec3> normals;
					std::vector<glm::vec2> uvs;		//unused in this program, but loadOBJ requires it as an input parameter 
					loadOBJ(static_cast<Mesh*>(objects[x])->file.c_str(), vertices, normals, uvs, static_cast<Mesh*>(objects[x]), objects);
					break;
				}
			}
		}
		printCamera();
		printObjects();
		int height = static_cast<int>(2 * camera.focalLength*tan(camera.fieldOfView / 2 * PI / 180.0f));
		int width = static_cast<int>(camera.aspectRatio * height);

		// MAIN RENDER LOOP
		cout << "Rendering..." << endl;
		renderingLoop(width, height);
		cout << "Finished." << endl;

		std::string answer;
		cout << "Would you like to render another scene? (yes, or anything else if no)" << endl;
		cin >> answer;
		cout << endl;
		if (answer.compare("yes") != 0) {
			repeat = false;
			cout << "Exiting the program..." << endl;
		}
		else {
			objects.clear();
			lights.clear();
		}
	}
	return 0;
}

void renderingLoop(int width, int height) {
	CImg<float> image(width, height, 1, 3, 0);

	// Nested loop over each pixel, top left corner to bottom right corner. 
	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {


			// RAY BEING CAST FROM CAMERA
			// http://web.cse.ohio-state.edu/~shen.94/681/Site/Slides_files/basic_algo.pdf
			vector<float> rayOrigin = { camera.position[0], camera.position[1], camera.position[2] + camera.focalLength };
			vector<float> pixelPosition = { -static_cast<float>(width) / 2 + x, static_cast<float>(height) / 2 - y, camera.position[2] };
			vector<float> rayDirection = normalize(subtr(pixelPosition, rayOrigin));
			Ray ray = Ray(camera.position, rayDirection);

			// Values used to track the smallest value of t and the object associated with that value (i.e. the closest hit by the ray)
			float tMin = INFINITY;
			int track = 0;


			for (int i = 0; i < objects.size(); i++) {
				if (objects[i]->name.compare("sphere") == 0) {
					float t = static_cast<Sphere*>(objects[i])->findIntersection(ray);
					if (t > 0) {
						if (t < tMin) {
							tMin = t;
							track = i;
						}
					}
				}
				if (objects[i]->name.compare("plane") == 0) {
					float t = static_cast<Plane*>(objects[i])->findIntersection(ray);
					if (t > 0) {
						if (t < tMin) {
							tMin = t;
							track = i;
						}
					}
				}
				if (objects[i]->name.compare("triangle") == 0) {
					float t = static_cast<Triangle*>(objects[i])->findIntersection(ray);
					if (t > 0) {
						if (t < tMin) {
							tMin = t;
							track = i;
						}
					}
				}

			}

			// The ray from the camera has hit something if the value for t is less than infinity
			if (tMin < INFINITY) {
				vector<float> totalDiffuse = { 0.0f, 0.0f, 0.0f };
				vector<float> totalSpecular = { 0.0f, 0.0f, 0.0f };
				vector<float> totalAmbient = { 0.0f, 0.0f, 0.0f };

				//// Ray intersection position P with offset
				//std::vector<float> offset = { 0.001f, 0.001f, 0.001f };

				// 3D parametric line equation: p(t) = e + t(s-e)
				std::vector<float> P = add(camera.position, scalarMulti(tMin, rayDirection));


				// Calculate normal N and retrieve alpha (shininess) depending on object type 
				std::vector<float> N;
				float alpha = 0;

				if (objects[track]->name.compare("sphere") == 0) {
					N = normalize(subtr(P, static_cast<Sphere*>(objects[track])->position));
					alpha = static_cast<Sphere*>(objects[track])->shininess;
				}
				if (objects[track]->name.compare("plane") == 0) {
					N = static_cast<Plane*>(objects[track])->normal;
					alpha = static_cast<Plane*>(objects[track])->shininess;
				}
				if (objects[track]->name.compare("triangle") == 0) {
					std::vector<float> v2v1 = subtr(static_cast<Triangle*>(objects[track])->v2, static_cast<Triangle*>(objects[track])->v1);
					std::vector<float> v3v1 = subtr(static_cast<Triangle*>(objects[track])->v3, static_cast<Triangle*>(objects[track])->v1);
					N = normalize(crossProduct(v2v1, v3v1));
					alpha = static_cast<Triangle*>(objects[track])->shininess;
				}





				//https://pages.cpsc.ucalgary.ca/~brosz/TA/453f08/labnotes/Lab%2018%20-%20Phong%20Lighting.pdf
				for (int i = 0; i < lights.size(); i++) {

					Ray shadowRay = Ray(P, normalize(subtr(lights[i]->position, P)));

					// http://web.cse.ohio-state.edu/~shen.94/681/Site/Slides_files/illum_large.pdf
					// Light Direction 
					std::vector<float> lightDir = normalize(subtr(lights[i]->position, P));
					// Specular
					std::vector<float> reflectDir = normalize(subtr(scalarMulti(2 * dotProduct(N, lightDir), N), lightDir));
					// View direction
					std::vector<float> viewDir = normalize(subtr(camera.position, P));

					vector<float> diffuse = scalarMulti(fmax(dotProduct(lightDir, N), 0.0f), lights[i]->diffuse);
					vector<float> specular = scalarMulti(pow(fmax(dotProduct(reflectDir, viewDir), 0.0f), alpha), lights[i]->specular);
					vector<float> ambient = vectMulti(objects[track]->ambient, lights[i]->ambient);

					// Testing for each shape, if there's no obstructing object, assign RGB contributions
					for (int j = 0; j < objects.size(); j++) {

						if (objects[j]->name.compare("triangle") == 0 && objects[j] != objects[track]) {
							float t = static_cast<Triangle*>(objects[j])->findIntersection(shadowRay);

							if (t < 0) {
								totalDiffuse = add(totalDiffuse, diffuse);
								totalSpecular = add(totalSpecular, specular);
								totalAmbient = add(totalAmbient, ambient);

							}

						}

						if (objects[j]->name.compare("sphere") == 0 && objects[j] != objects[track]) {
							float t = static_cast<Sphere*>(objects[j])->findIntersection(shadowRay);

							if (t < 0) {
								totalDiffuse = add(totalDiffuse, diffuse);
								totalSpecular = add(totalSpecular, specular);
								totalAmbient = add(totalAmbient, ambient);
							}

						}

						if (objects[j]->name.compare("plane") == 0 && objects[j] != objects[track]) {
							float t = static_cast<Plane*>(objects[j])->findIntersection(shadowRay);
							if (t < 0) {
								totalDiffuse = add(totalDiffuse, diffuse);
								totalSpecular = add(totalSpecular, specular);
								totalAmbient = add(totalAmbient, ambient);
							}
						}

					}
					totalDiffuse = vectMulti(objects[track]->diffuse, totalDiffuse);
					totalSpecular = vectMulti(objects[track]->specular, totalSpecular);
					totalAmbient = vectMulti(objects[track]->ambient, totalAmbient);

					vector<float> result = add(add(totalDiffuse, totalSpecular), totalAmbient);
					result = { result[0] / lights.size(), result[1] / lights.size(), result[2] / lights.size() };

					float color[] = { result[0], result[1], result[2] };

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
		else if ((objects[i]->name).compare("mesh") == 0) {
			cout << objects[i]->name << endl;
			cout << static_cast<Mesh*>(objects[i])->file << endl;
			vectorToString(static_cast<Mesh*>(objects[i])->ambient);
			vectorToString(static_cast<Mesh*>(objects[i])->diffuse);
			vectorToString(static_cast<Mesh*>(objects[i])->specular);
			cout << static_cast<Mesh*>(objects[i])->shininess << endl;
			cout << endl;
		}
		else if ((objects[i]->name).compare("triangle") == 0) {
			cout << objects[i]->name << endl;
			vectorToString(static_cast<Triangle*>(objects[i])->v1);
			vectorToString(static_cast<Triangle*>(objects[i])->v2);
			vectorToString(static_cast<Triangle*>(objects[i])->v3);
			vectorToString(static_cast<Triangle*>(objects[i])->ambient);
			vectorToString(static_cast<Triangle*>(objects[i])->diffuse);
			vectorToString(static_cast<Triangle*>(objects[i])->specular);
			cout << static_cast<Triangle*>(objects[i])->shininess << endl;
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