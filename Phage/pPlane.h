#ifndef PPLANE_H
#define PPLANE_H

#include <glm/glm.hpp>

class pPlane
{
private:
	glm::vec3 point;
	float a, b, c, d;
public:
	pPlane() :
		a(0.0), b(1.0), c(0.0), d(0.0), point(glm::vec3(0.0, 0.0, 0.0)) {}

	pPlane(float _a, float _b, float _c, float _d, glm::vec3 &pt);

	pPlane(glm::vec3 &p1, glm::vec3 &p2, glm::vec3 &p3);

	glm::vec3 getNormal() const;

	float getDistance() const;

	pPlane& operator=(const pPlane &other);

	friend bool operator==(const pPlane &a, const pPlane &b);

	friend bool operator!=(const pPlane &a, const pPlane &b);

	float signedDistance(glm::vec3 &point);

};

#endif