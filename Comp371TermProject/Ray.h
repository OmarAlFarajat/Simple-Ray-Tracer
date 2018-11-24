#ifndef _RAY_H
#define _RAY_H

#include <vector>

class Ray {
public: 
	std::vector<float> origin, direction; 
	Ray(std::vector<float>, std::vector < float>);
	Ray();
};
Ray::Ray() {

}
Ray::Ray(std::vector<float> origin, std::vector<float> direction) {
	this->origin = origin; 
	this->direction = direction; 
}
#endif