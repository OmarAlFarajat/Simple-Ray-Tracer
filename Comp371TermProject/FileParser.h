#ifndef _FILEPARSER_H
#define _FILEPARSER_H

#include <vector> 
#include <string>
#include <fstream>
#include "Renderable.h"
#include "Camera.h"
#include <iostream>

void parseFile(std::string & fileName, std::vector<Renderable*> & objects, std::vector<Light*> & lights, Camera & camera) {

	std::ifstream inFile(fileName, std::ios::in);
	std::string lineRead;
	std::getline(inFile, lineRead);

	while (inFile) {
		getline(inFile, lineRead);
		/// CAMERA
		if (lineRead.compare("camera") == 0)
		{
			std::vector<float> position;
			float fieldOfView, focalLength, aspectRatio;

			getline(inFile, lineRead, ' ');

			// Position
			for (int i = 0; i < 3; i++) {
				getline(inFile, lineRead, ' ');
				position.push_back(std::stof(lineRead));
			}
			// Field of view
			getline(inFile, lineRead);
			fieldOfView = std::stof(lineRead);
			// Focal length
			getline(inFile, lineRead, ' ');
			getline(inFile, lineRead, ' ');
			focalLength = std::stof(lineRead);
			// Aspect ratio
			getline(inFile, lineRead);
			aspectRatio = std::stof(lineRead);
			camera = Camera(position, fieldOfView, focalLength, aspectRatio);
		}
		/// PLANE
		else if (lineRead.compare("plane") == 0) { 
			std::vector<float> normal, position, ambient, diffuse, specular;
			float shininess;
			
			getline(inFile, lineRead, ' ');

			//normal
			for (int i = 0; i < 3; i++) {
				getline(inFile, lineRead, ' ');
				normal.push_back(std::stof(lineRead));
			}
			//position
			for (int i = 0; i < 3; i++) {
				getline(inFile, lineRead, ' ');
				position.push_back(std::stof(lineRead));
			}
			//ambient
			for (int i = 0; i < 3; i++) {
				getline(inFile, lineRead, ' ');
				ambient.push_back(std::stof(lineRead));
			}
			//diffuse
			for (int i = 0; i < 3; i++) {
				getline(inFile, lineRead, ' ');
				diffuse.push_back(std::stof(lineRead));
			}
			//specular
			for (int i = 0; i < 3; i++) {
				getline(inFile, lineRead, ' ');
				specular.push_back(std::stof(lineRead));
			}
			//shininess
			getline(inFile, lineRead);
			shininess = std::stof(lineRead);

			objects.push_back(new Plane(normal, position, shininess, ambient, diffuse, specular));
		}
		/// SPHERE
		else if (lineRead.compare("sphere") == 0)
		{
			std::vector<float> position, ambient, diffuse, specular;
			float radius, shininess; 

			getline(inFile, lineRead, ' ');

			// Position
			for (int i = 0; i < 3; i++) {
				getline(inFile, lineRead, ' ');
				position.push_back(std::stof(lineRead));
			}
			// Radius
			getline(inFile, lineRead,' ');
			radius = std::stof(lineRead);
			// Ambient
			for (int i = 0; i < 3; i++) {
				getline(inFile, lineRead, ' ');
				ambient.push_back(std::stof(lineRead));
			}
			// Diffuse
			for (int i = 0; i < 3; i++) {
				getline(inFile, lineRead, ' ');
				diffuse.push_back(std::stof(lineRead));
			}
			// Specular
			for (int i = 0; i < 3; i++) {
				getline(inFile, lineRead, ' ');
				specular.push_back(std::stof(lineRead));
			}
			// Shininess
			getline(inFile, lineRead);
			shininess = std::stof(lineRead);

			objects.push_back(new Sphere(position, radius, shininess,
				ambient, diffuse, specular));
		}
		/// TRIANGLE
		else if (lineRead.compare("triangle") == 0)
		{
		std::vector<float> v1, v2, v3, ambient, diffuse, specular;
		float shininess;

		getline(inFile, lineRead, ' ');

		//v1
		for (int i = 0; i < 3; i++) {
			getline(inFile, lineRead, ' ');
			v1.push_back(std::stof(lineRead));
		}
		//v2
		for (int i = 0; i < 3; i++) {
			getline(inFile, lineRead, ' ');
			v2.push_back(std::stof(lineRead));
		}
		//v3
		for (int i = 0; i < 3; i++) {
			getline(inFile, lineRead, ' ');
			v3.push_back(std::stof(lineRead));
		}
		// Ambient
		for (int i = 0; i < 3; i++) {
			getline(inFile, lineRead, ' ');
			ambient.push_back(std::stof(lineRead));
		}
		// Diffuse
		for (int i = 0; i < 3; i++) {
			getline(inFile, lineRead, ' ');
			diffuse.push_back(std::stof(lineRead));
		}
		// Specular
		for (int i = 0; i < 3; i++) {
			getline(inFile, lineRead, ' ');
			specular.push_back(std::stof(lineRead));
		}
		// Shininess
		getline(inFile, lineRead);
		shininess = std::stof(lineRead);
		objects.push_back(new Triangle(v1, v2, v3, shininess, ambient, diffuse, specular));
		}
		/// MODEL
		else if (lineRead.compare("model") == 0)
		{
			std::string file;
			std::vector<float> ambient, diffuse, specular;
			float shininess;

			getline(inFile, lineRead, ' ');

			// File
			getline(inFile, lineRead);
			file = lineRead; 

			getline(inFile, lineRead, ' ');

			// Ambient
			for (int i = 0; i < 3; i++) {
				getline(inFile, lineRead, ' ');
				ambient.push_back(std::stof(lineRead));
			}
			// Diffuse
			for (int i = 0; i < 3; i++) {
				getline(inFile, lineRead, ' ');
				diffuse.push_back(std::stof(lineRead));
			}
			// Specular
			for (int i = 0; i < 3; i++) {
				getline(inFile, lineRead, ' ');
				specular.push_back(std::stof(lineRead));
			}
			// Shininess
			getline(inFile, lineRead);
			shininess = std::stof(lineRead);

			objects.push_back(new Model(shininess, file, ambient, diffuse, specular));
		}
		/// LIGHT
		else if (lineRead.compare("light") == 0) {
			std::vector<float> position, ambient, diffuse, specular; 
			getline(inFile, lineRead, ' ');

			// Position
			for (int i = 0; i < 3; i++) {
				getline(inFile, lineRead, ' ');
				position.push_back(std::stof(lineRead));
			}
			// Ambient
			for (int i = 0; i < 3; i++) {
				getline(inFile, lineRead, ' ');
				ambient.push_back(std::stof(lineRead));
			}
			// Diffuse
			for (int i = 0; i < 3; i++) {
				getline(inFile, lineRead, ' ');
				diffuse.push_back(std::stof(lineRead));
			}
			// Specular
			for (int i = 0; i < 2; i++) {
				getline(inFile, lineRead, ' '); 
				specular.push_back(std::stof(lineRead));
			}
			getline(inFile, lineRead);
			specular.push_back(std::stof(lineRead));
			lights.push_back(new Light(position, ambient, diffuse, specular));
			
		}
		
	}

	inFile.close();

}
#endif