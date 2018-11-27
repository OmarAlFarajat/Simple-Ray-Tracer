#ifndef _PLANE_H
#define _PLANE_H
#include <vector>
#include <string>
#include "Renderable.h"

/// PLANE
class Plane :
	public Renderable {
public:
	std::vector<float> normal, position;
	float shininess;
	Plane(std::vector<float>, std::vector<float>, float,
		std::vector<float>, std::vector<float>, std::vector<float>);
	float findIntersection(Ray);
};

Plane::Plane(std::vector<float> normal, std::vector<float> position, float shininess,
	std::vector<float> ambient, std::vector<float> diffuse, std::vector<float> specular) : Renderable(ambient, diffuse, specular) {
	this->name = "plane";
	this->normal = normal;
	this->position = position;
	this->shininess = shininess;
}
float Plane::findIntersection(Ray ray) {
	https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-plane-and-ray-disk-intersection

	float denom = dotProduct(ray.direction, this->normal);

	if (fabs(denom) > 1e-6) {

		float t = dotProduct(subtr(this->position, ray.origin), this->normal) / denom;
		if (t >= 0)
			return t;
	}
	return -1;

	

}

#endif