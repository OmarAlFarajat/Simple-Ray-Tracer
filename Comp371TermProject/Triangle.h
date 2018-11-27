#ifndef _TRIANGLE_H
#define _TRIANGLE_H

#include <vector>
#include <string>
#include "Renderable.h"
#include "Plane.h"

class Triangle :
	public Renderable {
public:
	std::vector<float> v1, v2, v3;
	float shininess;
	Triangle(std::vector<float>, std::vector<float>, std::vector<float>, float, std::vector<float>, std::vector<float>, std::vector<float>);
	float findIntersection(Ray);
};
Triangle::Triangle(std::vector<float> v1, std::vector<float> v2, std::vector<float> v3, float shininess,
	std::vector<float> ambient, std::vector<float> diffuse, std::vector<float> specular) : Renderable(ambient, diffuse, specular) {
	this->name = "triangle";
	this->v1 = v1;
	this->v2 = v2;
	this->v3 = v3;
	this->shininess = shininess;
}
float Triangle::findIntersection(Ray ray) {

	https://courses.cs.washington.edu/courses/cse457/09sp/lectures/triangle_intersection.pdf



	//////////////////////////////////
	///////// Plane test
	///////////////////////////////////
	//std::vector<float> empty = { 0.0f, 0.0f, 0.0f };
	//std::vector<float> normal = normalize(crossProduct(subtr(this->v3, this->v2), subtr(this->v1, this->v2))); 
	//float t = (new Plane(normal, 
	//	this->v2,
	//	0.0f,
	//	empty,
	//	empty,
	//	empty))->findIntersection(ray);
	//return t; 

	//////////////////////////
	//// Geometric solution
	////https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-rendering-a-triangle/ray-triangle-intersection-geometric-solution
	////https://pages.cpsc.ucalgary.ca/~brosz/TA/453f08/labnotes/Lab%2022%20-%20Raytracing%20implicits.PDF
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	
	// Normal calculation 
	std::vector<float> normal = normalize(crossProduct(subtr(this->v2, this->v1), subtr(this->v3, this->v1)));

	float d = dotProduct(normal, this->v1);

	float denom = dotProduct(normal, ray.direction);

	if (denom == 0)
		return -1;

	float t = (dotProduct(normal, ray.origin) + d) / denom;

	std::vector<float> P = add(ray.origin, scalarMulti(t, ray.direction));

	// Check parallel
	if (fabs(dotProduct(normal, ray.direction)) < 1e-6)
		return -1;

	// Check if behind
	if (t < 0)
		return -1;

	float A = dotProduct(crossProduct(subtr(this->v2, this->v1), subtr(P, this->v1)), normal);
	float B = dotProduct(crossProduct(subtr(this->v3, this->v2), subtr(P, this->v2)), normal);
	float C = dotProduct(crossProduct(subtr(this->v1, this->v3), subtr(P, this->v3)), normal);

	if (A <= 0)
		return -1;
	if (B <= 0)
		return -1;
	if (C <= 0)
		return -1;

	return t;
}
#endif