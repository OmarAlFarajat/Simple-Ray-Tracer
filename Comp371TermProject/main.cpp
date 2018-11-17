#include <iostream>
#include <vector>
#include "Renderable.h"
#include "FileParser.h"

using namespace std;

vector<Renderable*> objects;
Camera camera;

void vectorToString(vector<float>);
void printObjects();

int main() {
	std::string file = "sceneCube.txt";
	parseFile(file, objects, camera);

	cout << "camera" << endl;
	vectorToString(camera.position);
	cout << camera.fieldOfView << endl;
	cout << camera.focalLength << endl;
	cout << camera.aspectRatio << endl;

	cout << endl;
	
	printObjects(); 

	cout << endl;

	return 0; 
}

void vectorToString(vector<float> vector) {
	cout << vector[0] << " " << vector[1] << " " << vector[2] << endl;
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