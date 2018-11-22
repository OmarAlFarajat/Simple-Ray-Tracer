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
	int findIntersection(Ray, std::vector<float>);
};

Plane::Plane(std::vector<float> normal, std::vector<float> position, float shininess,
	std::vector<float> ambient, std::vector<float> diffuse, std::vector<float> specular) : Renderable(ambient, diffuse, specular) {
	this->name = "plane";
	this->normal = normal;
	this->position = position;
	this->shininess = shininess;
}
int Plane::findIntersection(Ray ray, std::vector<float> cameraPosition) {
	float denom = scalarProduct(this->normal, ray.direction);
	if (denom > 0) {
		std::vector<float> camPosToPlanePos = {this->position[0] - cameraPosition[0], this->position[1] - cameraPosition[1], this->position[2] - cameraPosition[2]};
		float t = scalarProduct(camPosToPlanePos, this->normal) / denom;
		if(t >= 0)
		return 1;
		else 
			return 0;
	}
	else
		return 0;
}
/// SPHERE
class Sphere :
	public Renderable {
public:
	std::vector<float> position;
	float radius, shininess;
	Sphere(std::vector<float>, float, float,
		std::vector<float>, std::vector<float>, std::vector<float>);
	float findIntersection(Ray, std::vector<float>);
};

Sphere::Sphere(std::vector<float> position, float radius, float shininess,
	std::vector<float> ambient, std::vector<float> diffuse, std::vector<float> specular) : Renderable(ambient, diffuse, specular) {
	this->name = "sphere";
	this->position = position;
	this->radius = radius;
	this->shininess = shininess;
}
float Sphere::findIntersection(Ray ray, std::vector<float> cameraPosition) {

	//http://web.cse.ohio-state.edu/~shen.94/681/Site/Slides_files/basic_algo.pdf
	// slide 24/55

	//(d.d) t + 2d.(e-c)t + (e-c).(e-c) - R = 0	// a = (d.d)
	// b = 2*d.(e-c)
	// c = (e-c).(e-c) - R^2
	//std::vector<float> centerToOrigin = std::vector<float>{ ray.origin[0] - this->position[0], ray.origin[1] - this->position[1], ray.origin[2] - this->position[2] };
	std::vector<float> centerToOrigin = std::vector<float>{ cameraPosition[0] - this->position[0], cameraPosition[1] - this->position[1], cameraPosition[2] - this->position[2] };

	float a = scalarProduct(ray.direction, ray.direction);
	float b = 2 * scalarProduct(ray.direction, centerToOrigin);
	float c = scalarProduct(centerToOrigin, centerToOrigin) - pow(this->radius,2);
	//b^2 – 4ac >= 0 ---> intersection
	float det = pow(b,2) - 4 * a*c; 
	if (det >= 0) 
		return 1;
	else
	return 0;
}
/// TRIANGLE
class Triangle :
	public Renderable {
public:
	std::vector<std::vector<float>> vertices;
	std::vector<float> normal;
	float shininess;
	Triangle(std::vector<std::vector<float>>, std::vector<float>, float, std::vector<float>, std::vector<float>, std::vector<float>);
};

Triangle::Triangle(std::vector<std::vector<float>> vertices, std::vector<float> normal, float shininess,
	std::vector<float> ambient, std::vector<float> diffuse, std::vector<float> specular) : Renderable(ambient, diffuse, specular) {
	this->vertices = vertices;
	this->normal = normal;
	this->shininess = shininess;
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
	{ a[1] * b[2] - a[2] * b[1],
		a[0] * b[2] - a[2] * b[0],
		a[0] * b[1] - a[1] * b[0]};
}

std::vector<float> add(std::vector<float> a, std::vector<float> b) {
	return std::vector<float>
	{	a[0] + b[0],
		a[1] + b[1],
		a[2] + b[2]};
}
#endif