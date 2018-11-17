#include <iostream>
#include <vector>
#include "Renderable.h"
#include "FileParser.h"

using namespace std;

vector<Renderable*> objects;
Camera camera;

void vectorToString(vector<float> vector) {
	cout << vector[0] << " " << vector[1] << " " << vector[2] << endl;
}

int main() {

	objects.push_back(new Plane({ 0.0f,1.0f,0.0f }, { 0.0f,-5.0f,0.0f }, 6.0f,
		{ 0.8f,0.8f,0.8f }, { 0.1f, 0.1f, 0.1f }, { 0.7f,0.7f,0.7f }));
	std::string fileName = "sceneCube.txt";
	parseFile(fileName, objects, camera);


	cout << objects[0]->name << endl;
	vectorToString(static_cast<Plane*>(objects[0])->normal); 
	vectorToString(static_cast<Plane*>(objects[0])->position);
	vectorToString(static_cast<Plane*>(objects[0])->ambient);
	vectorToString(static_cast<Plane*>(objects[0])->diffuse);
	vectorToString(static_cast<Plane*>(objects[0])->specular);
	cout << static_cast<Plane*>(objects[0])->shininess << endl;

	cout << "camera" << endl;
	vectorToString(camera.position);
	cout << camera.fieldOfView << endl; 
	cout << camera.focalLength << endl;
	cout << camera.aspectRatio << endl;
	return 0; 
}