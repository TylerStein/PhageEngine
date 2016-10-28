#pragma once

#include "pBoundingBox.h"
#include "glm\vec3.hpp"
#include "GL/glew.h"
#include <iostream>
#include <sstream>

class pBoundingSphere
{
public:
	pBoundingSphere();
	~pBoundingSphere();

	//constructor to make a bounding sphere from a center and radius
	pBoundingSphere(glm::vec3 &cent, GLfloat rad) : center(cent), radius(rad)
	{}

	//make a bounding sphere from a bounding box
	pBoundingSphere(const pBoundingBox &boundingBox);

	//assignment operator
	pBoundingSphere& operator=(const pBoundingSphere &other);

	//inequality operator
	friend bool operator!=(const pBoundingSphere &pshere1, const pBoundingSphere &sphere2);

	//Equality operator
	friend bool operator==(const pBoundingSphere &sphere1, const pBoundingSphere &sphere2);

	//surround a model with a bounding sphere
	void surround(pModel &model);

	//checks if spheres are intersecting
	bool intersects(const pBoundingSphere &other) const;

	//return the center of the sphere
	glm::vec3 getCenter()
	{
		return center;
	}

	//returns the radius
	GLfloat getRadius()
	{
		return radius;
	}

	//adds two bounding spheres together to create one bounding sphere that
	//encompases both objects
	friend pBoundingSphere operator+(const pBoundingSphere &sphere1, const pBoundingSphere &sphere2);

	//returns a boundng sphere that is the same, but shifted by the amount specified
	pBoundingSphere movedBy(const glm::vec3 &amount) const;


private:
	glm::vec3 center;
	GLfloat radius;
};

