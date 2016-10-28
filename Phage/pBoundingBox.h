#pragma once

#include "glm\vec3.hpp"
#include <ostream>

class pBoundingBox
{

	friend class pModel;

public:
	pBoundingBox();
	~pBoundingBox();

	//constructor to create a bounding box with given size
	pBoundingBox(const glm::vec3 &minCorner, const glm::vec3 &maxCorner);

	//Assignment operator
	pBoundingBox& operator=(const pBoundingBox &box);

	//Equality operator for comparing different bounding boxes
	friend bool operator==(const pBoundingBox &box1, const pBoundingBox &box2);

	//adds two bounding boxes together to create a bounding box that encloses
	//both boxes included in the operation
	friend pBoundingBox& operator+(const pBoundingBox &box1, const pBoundingBox &box2);

	//Calculate coords for the bounding box so it encapsulates the model passed in
	void surround(pModel &model);

	bool intersects(const pBoundingBox &box) const;

	//returns the minimum corner
	glm::vec3 getMinimumCorner() const;

	//returns the maximum corner
	glm::vec3 getMaximumCorner() const;

	//returns the vertex farthest along the bounding boxes normal to the plane
	glm::vec3 getFurthestOnNormal(glm::vec3 &normal);

	//returns the vertex closest along the bounding boxes normal to the plane
	glm::vec3 getClosestOnNormal(glm::vec3 &normal);

	//returns a boundng  box that is the same, but shifted by the amount specified
	pBoundingBox movedBy(const glm::vec3 &amount);

private:
	//the bottom-left corner
	glm::vec3 minimumCorner;
	//the bottom right corner
	glm::vec3 maximumCorner;
};

