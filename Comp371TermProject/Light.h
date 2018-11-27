#ifndef _LIGHT_H
#define _LIGHT_H
#include <vector>
#include <string>
#include "Renderable.h"

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