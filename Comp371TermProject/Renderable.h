#ifndef _RENDERABLE_H
#define _RENDERABLE_H

#include <vector>
#include <string>
#include "Ray.h"

float scalarProduct(std::vector<float>, std::vector<float>);
std::vector<float> crossProduct(std::vector<float>, std::vector<float>);
std::vector<float> add(std::vector<float>, std::vector<float>);
std::vector<float> subtr(std::vector<float>, std::vector<float>);
std::vector<float> normalize(std::vector<float>);

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
	float findIntersection(Ray, std::vector<float>);
};

Plane::Plane(std::vector<float> normal, std::vector<float> position, float shininess,
	std::vector<float> ambient, std::vector<float> diffuse, std::vector<float> specular) : Renderable(ambient, diffuse, specular) {
	this->name = "plane";
	this->normal = normal;
	this->position = position;
	this->shininess = shininess;
}
float Plane::findIntersection(Ray ray, std::vector<float> cameraPosition) {
	//https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-plane-and-ray-disk-intersection
	float t = scalarProduct(subtr(this->position, cameraPosition), this->normal) / scalarProduct(ray.direction, this->normal);
	if (t > 0)
		return t; 
	else return -1; 
	
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
	//(d.d) t^2 + 2d.(e-c)t + (e-c).(e-c) - R^2 = 0
	// a = (d.d)
	// b = 2*d.(e-c)
	// c = (e-c).(e-c) - R^2
	std::vector<float> centerToOrigin = subtr(cameraPosition, this->position); 
	float a = scalarProduct(ray.direction, ray.direction);
	float b = 2 * scalarProduct(ray.direction, centerToOrigin);
	float c = scalarProduct(centerToOrigin, centerToOrigin) - pow(this->radius,2);
	//b^2 – 4ac >= 0 ---> intersection
	float discriminant = sqrt(pow(b,2) - 4 * a*c); 
	if (discriminant > 0) {
		float t0 = (-b + discriminant) / 2 / a;
		float t1 = (-b - discriminant) / 2 / a;
		if (t1 < t0)
			return t1;
		else
			return t0;
	}
	else
		return -1; 

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
std::vector<float> subtr(std::vector<float> a, std::vector<float> b) {
	return std::vector<float>
	{	a[0] - b[0],
		a[1] - b[1],
		a[2] - b[2]};
}
std::vector<float> scalarMulti(float a, std::vector<float> b) {
	return std::vector<float>
	{	a*b[0],
		a*b[1],
		a*b[2]};
}
std::vector<float> normalize(std::vector<float> vec) {
	float mag = sqrt(pow(vec[0],2) + pow(vec[1],2) + pow(vec[2],2));
	return {	vec[0] / mag,
				vec[1] / mag,
				vec[2] / mag };
}
#endif