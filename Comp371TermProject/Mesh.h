#ifndef _MESH_H
#define _MESH_H

#include <vector>
#include <string>
#include "Renderable.h"

/// MODEL
class Mesh :
	public Renderable {
public:
	float shininess;
	std::string file;
	Mesh(float, std::string, std::vector<float>, std::vector<float>, std::vector<float>);
};

Mesh::Mesh(float shininess, std::string file,
	std::vector<float> ambient, std::vector<float> diffuse, std::vector<float> specular) : Renderable(ambient, diffuse, specular) {
	this->name = "mesh";
	this->file = /*"../" + */file;
	this->shininess = shininess;
}
#endif