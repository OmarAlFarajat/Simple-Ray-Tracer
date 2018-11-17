#ifndef _CAMERA_H
#define _CAMERA_H

#include <string>
#include <vector>

/// RENDERABLE
class Camera {
public:
	std::vector<float> position;
	float fieldOfView, focalLength, aspectRatio; 
	Camera(std::vector<float>, float, float, float);
	Camera();
};

Camera::Camera() {};
Camera::Camera(std::vector<float> position, float fieldOfView, float focalLength, float aspectRatio) {
	this->position = position;
	this->fieldOfView = fieldOfView; 
	this->focalLength = focalLength;
	this->aspectRatio = aspectRatio;
}
#endif
