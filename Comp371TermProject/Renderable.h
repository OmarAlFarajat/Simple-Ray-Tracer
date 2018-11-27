#ifndef _RENDERABLE_H
#define _RENDERABLE_H

#include <vector>
#include <string>
#include "Ray.h"

float dotProduct(std::vector<float>, std::vector<float>);
std::vector<float> crossProduct(std::vector<float>, std::vector<float>);
std::vector<float> add(std::vector<float>, std::vector<float>);
std::vector<float> subtr(std::vector<float>, std::vector<float>);
std::vector<float> normalize(std::vector<float>);
float angle(std::vector<float>, std::vector<float>);
float magnitude(std::vector<float>);
std::vector<float> vectMulti(std::vector<float>, std::vector<float>); 
std::vector<float> scalarMulti(float, std::vector<float>);
float innerProduct(std::vector<float>, std::vector<float>);

/// RENDERABLE
class Renderable {
public:
	std::string name;
	std::vector<float> ambient, diffuse, specular;
	Renderable(std::vector<float>, std::vector<float>, std::vector<float>);

	float findIntersection(Ray);

};
float Renderable::findIntersection(Ray ray) {
	return -1;
}

Renderable::Renderable(std::vector<float> ambient, std::vector<float> diffuse, std::vector<float> specular) {
	this->ambient = ambient;
	this->diffuse = diffuse;
	this->specular = specular;
}

float dotProduct(std::vector<float> a, std::vector<float> b) {
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
float innerProduct(std::vector<float> a, std::vector<float> b) {
	return (a[0] * b[0] + 
		a[1] * b[1] +
		a[2] * b[2]) ;
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
std::vector<float> vectMulti(std::vector<float> a, std::vector<float> b) {
	return std::vector<float>
	{ a[0] * b[0] ,
		a[1] * b[1],
		a[2] * b[2]};
}
float magnitude(std::vector<float> vec) {
	return sqrt(pow(vec[0], 2) + pow(vec[1], 2) + pow(vec[2], 2)); 
}
std::vector<float> normalize(std::vector<float> vec) {
	float mag = sqrt(pow(vec[0], 2) + pow(vec[1], 2) + pow(vec[2], 2));
	if (mag != 0)
		return { vec[0] / mag,
						vec[1] / mag,
						vec[2] / mag };
	else
		return vec;

}
float angle(std::vector<float> a, std::vector<float> b) {

	float lenSq1 = pow(a[0], 2) + pow(a[1], 2) + pow(a[2], 2);
	float lenSq2 = pow(b[0], 2) + pow(b[1], 2) + pow(b[2], 2);
	float angle = acos(dotProduct(a, b) / sqrt(lenSq1 * lenSq2));
	return angle;
}
std::vector<float> invert(std::vector<float> a) {

	return std::vector<float>
	{ a[0] * -1,
		a[1] * -1,
		a[2] * -1};
}
#endif