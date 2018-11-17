#ifndef _FILEPARSER_H
#define _FILEPARSER_H

#include <vector> 
#include <string>
#include <fstream>
#include "Renderable.h"
#include "Camera.h"

void parseFile(std::string & fileName, std::vector<Renderable*> & objects, Camera & camera) {

	std::ifstream inFile(fileName, std::ios::in);
	std::string lineRead;
	std::getline(inFile, lineRead);

	//objCount = std::stoi(lineRead);
	//cout << lineRead << endl; 

	while (inFile) {
		getline(inFile, lineRead);
		//cout << lineRead << endl;
		if (lineRead.compare("camera") == 0)
		{

			/// Position
			getline(inFile, lineRead, ' ');
			std::vector<float> position;
			float fieldOfView;
			float focalLength;
			float aspectRatio; 

			for (int i = 0; i < 3; i++) {
				getline(inFile, lineRead, ' ');
				position.push_back(std::stof(lineRead));
			}

			//camera->position = temp;
			//std::vector<float>().swap(temp);

			/// FoV
			getline(inFile, lineRead);
			fieldOfView = std::stof(lineRead);
			/// Focal length
			getline(inFile, lineRead, ' ');
			getline(inFile, lineRead, ' ');
			focalLength = std::stof(lineRead);
			/// Aspect ratio
			getline(inFile, lineRead);
			aspectRatio = std::stof(lineRead);
			camera = Camera(position, fieldOfView, focalLength, aspectRatio);
		}
		else if (lineRead.compare("plane") == 0)
			return;
		else if (lineRead.compare("sphere") == 0)
		{
			std::vector<float> position;
			float radius;
			float shininess;
			std::vector<float> ambient;
			std::vector<float> diffuse;
			std::vector<float> specular;

			//objects.push_back(new Sphere);
			/// Position
			getline(inFile, lineRead, ' ');

			for (int i = 0; i < 3; i++) {
				getline(inFile, lineRead, ' ');
				position.push_back(std::stof(lineRead));
			}
			
			/// Radius
			getline(inFile, lineRead);
			radius = std::stof(lineRead);
			/// Ambient
			/// Diffuse
			/// Specular
			/// Shininess
			//vector<float>().swap(temp);
		}
		else if (lineRead.compare("model") == 0)
			return;
		else if (lineRead.compare("light") == 0)
			return;

	}

	inFile.close();

}
#endif