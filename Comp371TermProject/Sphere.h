#ifndef _SPHERE_H
#define _SPHERE_H
#include <vector>
#include <string>
#include "Renderable.h"

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

	//http://web.cse.ohio-state.edu/~shen.94/681/Site/Slides_files/basic_algo.pdf
	// slide 24/55
	std::vector<float> centerToOrigin = subtr(ray.origin, this->position);

	float a = dotProduct(ray.direction, ray.direction);
	float b = 2 * dotProduct(ray.direction, centerToOrigin);
	float c = dotProduct(centerToOrigin, centerToOrigin) - pow(this->radius, 2);
	//b^2 – 4ac >= 0 ---> intersection
	float discriminant = pow(b, 2) - 4 * a*c;



	//if (-1e-6 < discriminant < 1e-6)
	//	return -b / 2 / a; 

	//float t0 = -b + sqrt(discriminant) / 2 / a;
	//float t1 = -b - sqrt(discriminant) / 2 / a;

	//if (t0 < t1 && t0 > 1e-6)
	//	return t0;
	//else if (t1 < t0 && t1 > 1e-6)
	//	return t1;
	//else
	//	return -1; 

	if (discriminant < 0)
		return -1;
	else if (-1e-6 < discriminant < 1e-6 ) {
		return -b / 2 / a;
	}
	else {
		float q = (b > 0.0f) ? -0.5f * (b + sqrt(discriminant)) : -0.5 * (b - sqrt(discriminant));
		float t0 = q / a;
		float t1 = c / q;

		if (t0 < t1)
			return t0;
		else
			return t1;
	}

}
#endif