#include "pBoundingSphere.h"



pBoundingSphere::pBoundingSphere()
{
}


pBoundingSphere::~pBoundingSphere()
{
}

pBoundingSphere::pBoundingSphere(const pBoundingBox & boundingBox)
{
	glm::vec3 minimumCorner = boundingBox.getMinimumCorner();
	glm::vec3 maximumCorner = boundingBox.getMaximumCorner();
	center = minimumCorner + ((maximumCorner - minimumCorner) * 0.5f);

	glm::vec3 difference = center - minimumCorner;
	radius = sqrt(difference[0] * difference[0] + difference[1] * difference[1] + difference[2] * difference[2]);

	difference = maximumCorner - center;
	GLfloat len = sqrt(difference[0] * difference[0] + difference[1] * difference[1] + difference[2] * difference[2]);
	if (len > radius) radius = len;
}

pBoundingSphere& pBoundingSphere::operator=(const pBoundingSphere &other)
{
	center = other.center;
	radius = other.radius;
	return *this;
}

bool operator==(const pBoundingSphere &sphere1, const pBoundingSphere &sphere2)
{
	return sphere1.center == sphere2.center && sphere1.radius == sphere2.radius;
}

bool operator!=(const pBoundingSphere &pshere1, const pBoundingSphere &sphere2)
{
	return !(pshere1.center == sphere2.center && pshere1.radius == sphere2.radius);
}

pBoundingSphere operator+(const pBoundingSphere &sphere1, const pBoundingSphere &sphere2)
{
	glm::vec3 centerDifference = sphere2.center - sphere1.center;
	GLfloat centerDistance = sqrt(centerDifference[0] * centerDifference[0] + centerDifference[1] * centerDifference[1] + centerDifference[2] * centerDifference[2]);

	GLfloat maxRad = sphere1.radius;
	if (sphere2.radius > maxRad) maxRad = sphere2.radius;

	return pBoundingSphere(sphere1.center + (centerDifference * 0.5f), centerDistance * 0.5f + maxRad);
}

void pBoundingSphere::surround(pModel & model)
{
	pBoundingBox box;
	box.surround(model);
	pBoundingSphere sphere(box);
	(*this) = sphere;
}

bool pBoundingSphere::intersects(const pBoundingSphere & other) const
{
	glm::vec3 difference = other.center - center;
	GLfloat distance = sqrt(difference[0] * difference[0] + difference[1] * difference[1] + difference[2] * difference[2]);
	return (radius + other.radius) >= distance;
}

pBoundingSphere pBoundingSphere::movedBy(const glm::vec3 & amount) const
{
	return pBoundingSphere(center + amount, radius);
}
