#include "pPlane.h"

pPlane::pPlane(float _a, float _b, float _c, float _d, glm::vec3 &_point)
{
	point = _point;
	a = _a;
	b = _b;
	c = _c;
	d = _d;
}

pPlane::pPlane(glm::vec3 &p1, glm::vec3 &p2, glm::vec3 &p3)
{
	glm::vec3 v1 = p2 - p1;
	glm::vec3 v2 = p3 - p1;
	glm::vec3 normal = glm::normalize(glm::cross(v1, v2));
	a = normal[0];
	b = normal[1];
	c = normal[2];
	d = -(a * p1[0] + b * p1[1] + c * p1[2]);
	point = p1;
}

glm::vec3 pPlane::getNormal() const
{
	return glm::vec3(a, b, c);
}

float pPlane::getDistance() const
{
	return d;
}

pPlane& pPlane::operator=(const pPlane &other)
{
	a = other.a;
	b = other.b;
	c = other.c;
	d = other.d;
	return *this;
}

bool operator==(const pPlane &a, const pPlane &b)
{
	return (a.a == b.a) &&
		(a.b == b.b) &&
		(a.c == b.c) &&
		(a.d == b.d);
}

bool operator!=(const pPlane &a, const pPlane &b)
{
	return !((a.a == b.a) &&
		(a.b == b.b) &&
		(a.c == b.c) &&
		(a.d == b.d));
}

float pPlane::signedDistance(glm::vec3 &point)
{
	glm::vec3 N(a, b, c);
	return glm::dot(N, point) + d;
}
