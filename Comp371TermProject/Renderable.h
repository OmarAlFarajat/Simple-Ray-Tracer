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
	//https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-plane-and-ray-disk-intersection
	float denom = dotProduct(ray.direction, this->normal);

	if (denom > 1e-6 || denom < -1e-6) {
		float t = dotProduct(subtr(this->position, ray.origin), this->normal) / denom;
		return t;
	}
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

	//std::vector<float> L = subtr(this->position, ray.origin);
	//float tca = dotProduct(L, ray.direction); 
	////if (tca < 0) return -1; 
	//float d2 = dotProduct(L, L) - tca * tca; 
	//if (d2 > this->radius*this->radius)
	//	return -1; 
	//float thc = sqrt(this->radius*this->radius - d2);
	//float t0 = tca - thc; 
	//float t1 = tca + thc; 

	//float a = dotProduct(ray.direction, ray.direction);
	//float b = 2 * dotProduct(ray.direction, L);
	//float c = dotProduct(L, L) - this->radius*this->radius;
	//if (!solveQuadratic(a, b, c, t0, t1)) return -1; 

	//if (t0 > t1) std::swap(t0, t1);

	//if (t0 < 0) {
	//	t0 = t1;
	//	if (t0 < 0) return -1; 
	//}

	//return t0; 



	//float discriminant = pow(b, 2) - 4 * a*c;
	//if (discriminant < 0)
	//	return -1;
	//else if (discriminant == 0) {
	//	return -b / 2 / a;
	//}
	//else {

	//	float q = (b > 0) ? -0.5 * (b + sqrt(discriminant)) : -0.5 * (b - sqrt(discriminant));
	//	float t0 = q / a;
	//	float t1 = c / q;

	//	if (t0 < t1)
	//		return t0;
	//	else
	//		return t1;
	//}
		




	std::vector<float> centerToOrigin = subtr(ray.origin, this->position);


	float a = dotProduct(ray.direction, ray.direction);
	float b = 2 * dotProduct(ray.direction, centerToOrigin);
	float c = dotProduct(centerToOrigin, centerToOrigin) - pow(this->radius, 2);
	//b^2 – 4ac >= 0 ---> intersection
	float discriminant = pow(b, 2) - 4 * a*c;
	if (discriminant < 0)
		return -1; 
	else if (discriminant == 0) {
		return -b / 2 / a;
	}
	else{

		float q = (b > 0) ? -0.5 * (b + sqrt(discriminant)) : -0.5 * (b - sqrt(discriminant));
		float t0 = q / a; 
		float t1 = c / q; 

		if (t0 < t1)
			return t0;
		else
			return t1;
	}

}
/// TRIANGLE
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

	//std::vector<float> normal = normalize(crossProduct(subtr(this->v2, this->v1), subtr(this->v3, this->v1)));
	//std::vector<float> q = crossProduct(ray.direction, subtr(this->v3, this->v1)); 
	//float a = dotProduct(subtr(this->v2, this->v1), q);
	//if (dotProduct(normal, ray.direction) >= 0 || (fabs(a) <= 0))
	//	return -1; 

	//std::vector<float> s = scalarMulti(1/a, subtr(ray.origin, this->v1));
	//std::vector<float> r = crossProduct(s, subtr(this->v2, this->v1));

	//std::vector<float> b = { 0.0f, 0.0f, 0.0f };
	//b[0] = dotProduct(s, q); 
	//b[1] = dotProduct(r, ray.direction);
	//b[2] = 1.0f - b[0] - b[1];

	//if ((b[0] < 0.0f) || (b[1] < 0.0f) || (b[2] < 0.0f))
	//	return -1; 

	//float t = dotProduct(subtr(this->v3, this->v1), r); 
	//if (t >= 0.0f)
	//	return t; 

	////////////////////////
	// Geometric solution
	//https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-rendering-a-triangle/ray-triangle-intersection-geometric-solution
	//https://pages.cpsc.ucalgary.ca/~brosz/TA/453f08/labnotes/Lab%2022%20-%20Raytracing%20implicits.PDF

	// Normal calculation 
	std::vector<float> normal = normalize(crossProduct(subtr(this->v2, this->v1), subtr(this->v3, this->v2)));

	//float area = magnitude(normal);

	float d = dotProduct(normal, this->v1);

	float t = (dotProduct(normal, ray.origin) + d) / dotProduct(normal, ray.direction);

	std::vector<float> P = add(ray.origin, scalarMulti(t, ray.direction));

	// Check parallel
	if (fabs(dotProduct(normal, ray.direction)) < 1e-6)
		return -1; 

	// Check if behind
	if (t < 0)
		return -1;
	
	float A = dotProduct(crossProduct(subtr(this->v2,this->v1), subtr(P, this->v1)), normal);
	float B = dotProduct(crossProduct(subtr(this->v3, this->v2), subtr(P, this->v2)), normal);
	float C = dotProduct(crossProduct(subtr(this->v1, this->v3), subtr(P, this->v3)), normal);

	if (A < 0)
		return -1; 
	if (B < 0)
		return -1; 
	if (C < 0)
		return -1; 
	
		return t; 


	//std::vector<float> normal = normalize(crossProduct(subtr(this->v2, this->v1), subtr(this->v3, this->v1)));

	//float denom = dotProduct(ray.direction, normal);

	//if (denom != 0) {
	//	float t = dotProduct(subtr(this->v1, ray.origin), normal) / denom;
	//	return t;
	//}
	//else return -1;


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

#endif