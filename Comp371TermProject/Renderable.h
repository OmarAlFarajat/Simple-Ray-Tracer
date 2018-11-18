#ifndef _RENDERABLE_H
#define _RENDERABLE_H

#include <vector>
#include <string>
#include "Ray.h"

float scalarProduct(std::vector<float>, std::vector<float>);
std::vector<float> crossProduct(std::vector<float>, std::vector<float>);
std::vector<float> add(std::vector<float>, std::vector<float>);

/// RENDERABLE
class Renderable {
public:
	std::string name;
	std::vector<float> ambient, diffuse, specular;
	Renderable(std::vector<float>, std::vector<float>, std::vector<float>);
	
	float findIntersection(Ray);

};
 float Renderable::findIntersection(Ray ray) {
	return 0;
}

Renderable::Renderable(std::vector<float> ambient, std::vector<float> diffuse, std::vector<float> specular) {
	this->ambient = ambient;
	this->diffuse = diffuse;
	this->specular = specular;
}

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
	float a = scalarProduct(ray.direction, this->normal);
	if (a == 0) 		// ray is parallel to the plane
		return -1;
	else {
		float b = scalarProduct(this->normal, add(ray.origin, { this->normal[0] * (-100.0f), this->normal[1] * (-100.0f), this->normal[2] * (-100.0f) }));
		return -1 * b / a; 
	}
	}
/// SPHERE
class Sphere :
	public Renderable {
public: 
	std::vector<float> position;
	float radius, shininess;
	Sphere(std::vector<float>, float, float,
		std::vector<float>, std::vector<float>, std::vector<float>);
	float findIntersection(Ray);
};

Sphere::Sphere(std::vector<float> position, float radius, float shininess,
	std::vector<float> ambient, std::vector<float> diffuse, std::vector<float> specular) : Renderable(ambient, diffuse, specular) {
	this->name = "sphere";
	this->position = position; 
	this->radius = radius; 
	this->shininess = shininess; 
}
float Sphere::findIntersection(Ray ray) { 
	return 0; 
}

/// MODEL
class Model :
	public Renderable {
public: 
	float shininess;
	std::string file;
	Model(float, std::string, std::vector<float>, std::vector<float>, std::vector<float>);
};

Model::Model(float shininess, std::string file,
	std::vector<float> ambient, std::vector<float> diffuse, std::vector<float> specular) : Renderable(ambient, diffuse, specular) {
	this->name = "model";
	this->file = file; 
	this->shininess = shininess;
}

class Light :
	public Renderable {
public:
	std::vector<float> position;
	Light(std::vector<float>, std::vector<float>, std::vector<float>, std::vector<float>);
};

Light::Light(std::vector<float> position,
	std::vector<float> ambient, std::vector<float> diffuse, std::vector<float> specular) : Renderable(ambient, diffuse, specular) {
	this->name = "light";
	this->position = position; 
}

float scalarProduct(std::vector<float> a, std::vector<float> b) {
	float product = 0;
	for (int i = 0; i < a.size(); i++)
		product += a[i] * b[i];
	return product;
}

std::vector<float> crossProduct(std::vector<float> a, std::vector<float> b) {
	return std::vector<float> 
	{ a[1]*b[2] - a[2]*b[1], 
		a[0]*b[2] - a[2]*b[0], 
		a[0]*b[1] - a[1]*b[0]};
}

std::vector<float> add(std::vector<float> a, std::vector<float> b) {
	return std::vector<float>
	{	a[0] + b[0],
		a[1] + b[1],
		a[2] + b[2]};
}
#endif