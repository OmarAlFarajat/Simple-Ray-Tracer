#ifndef _RENDERABLE_H
#define _RENDERABLE_H

#include <vector>
#include <string>

/// RENDERABLE
class Renderable {
public:
	std::string name;
	std::vector<float> ambient, diffuse, specular;
	Renderable(std::vector<float>, std::vector<float>, std::vector<float>);
};

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
};

Plane::Plane(std::vector<float> normal, std::vector<float> position, float shininess,
	std::vector<float> ambient, std::vector<float> diffuse, std::vector<float> specular) : Renderable(ambient, diffuse, specular) {
	this->name = "plane";
	this->normal = normal;
	this->position = position;
	this->shininess = shininess;
}

/// SPHERE
class Sphere :
	public Renderable {
public: 
	std::vector<float> position;
	float radius, shininess;
	Sphere(std::vector<float>, float, float,
		std::vector<float>, std::vector<float>, std::vector<float>);
};

Sphere::Sphere(std::vector<float> position, float radius, float shininess,
	std::vector<float> ambient, std::vector<float> diffuse, std::vector<float> specular) : Renderable(ambient, diffuse, specular) {
	this->name = "sphere";
	this->position = position; 
	this->radius = radius; 
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
#endif