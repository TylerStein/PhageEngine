#include "pBoundingBox.h"
#include "pModel.h"



pBoundingBox::pBoundingBox()
{
}


pBoundingBox::~pBoundingBox()
{
}

pBoundingBox::pBoundingBox(const glm::vec3 & minCorner, const glm::vec3 & maxCorner)
{
	minimumCorner = minCorner;
	maximumCorner = maxCorner;
}

void pBoundingBox::surround(pModel & model)
{
	glm::vec3 min = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 max = glm::vec3(0.0f, 0.0f, 0.0f);

	GLuint increaseAmount = 3;

	for (int i = 1; i < (model.vertCount * increaseAmount); i += increaseAmount)
	{
		if (model.vPositions[i] < min.x) { min.x = model.vPositions[i]; }
		if (model.vPositions[i + 1] < min.y) { min.y = model.vPositions[i + 1]; }
		if (model.vPositions[i + 2] < min.z) { min.z = model.vPositions[i + 3]; }

		if (model.vPositions[i] > max.x) { max.x = model.vPositions[i]; }
		if (model.vPositions[i + 1] > max.y) { max.y = model.vPositions[i = 1]; }
		if (model.vPositions[i + 2] > max.z) { max.z = model.vPositions[i = 2]; }
	}
	pBoundingBox(min, max);
}

bool pBoundingBox::intersects(const pBoundingBox & box) const
{
	if (((glm::vec3)minimumCorner)[0] > ((pBoundingBox&)box).maximumCorner[0]) return false;
	if (((glm::vec3)minimumCorner)[1] > ((pBoundingBox&)box).maximumCorner[1]) return false;
	if (((glm::vec3)minimumCorner)[2] > ((pBoundingBox&)box).maximumCorner[2]) return false;

	if (((glm::vec3)maximumCorner)[0] < ((pBoundingBox&)box).minimumCorner[0]) return false;
	if (((glm::vec3)maximumCorner)[1] < ((pBoundingBox&)box).minimumCorner[1]) return false;
	if (((glm::vec3)maximumCorner)[2] < ((pBoundingBox&)box).minimumCorner[2]) return false;

	return true;
}

glm::vec3 pBoundingBox::getMinimumCorner() const
{
	return minimumCorner;
}

glm::vec3 pBoundingBox::getMaximumCorner() const
{
	return maximumCorner;
}

glm::vec3 pBoundingBox::getFurthestOnNormal(glm::vec3 & normal)
{
	glm::vec3 result(minimumCorner);

	if (normal[0] > 0) result[0] = maximumCorner[0];
	if (normal[1] > 0) result[1] = maximumCorner[1];
	if (normal[2] > 0) result[2] = maximumCorner[2];

	return result;
}

glm::vec3 pBoundingBox::getClosestOnNormal(glm::vec3 & normal)
{
	glm::vec3 result(minimumCorner);

	if (normal[0] < 0) result[0] = maximumCorner[0];
	if (normal[1] < 0) result[1] = maximumCorner[1];
	if (normal[2] < 0) result[2] = maximumCorner[2];

	return result;
}

pBoundingBox pBoundingBox::movedBy(const glm::vec3 & amount)
{
	pBoundingBox result(*this);
	result.minimumCorner = result.minimumCorner + amount;
	result.maximumCorner = result.maximumCorner + amount;
	return result;
}

pBoundingBox& pBoundingBox::operator=(const pBoundingBox &box)
{
	minimumCorner = box.minimumCorner;
	maximumCorner = box.maximumCorner;
	return *this;
}

bool operator==(const pBoundingBox &box1, const pBoundingBox &box2)
{
	return box1.minimumCorner == box2.minimumCorner &&
		box1.maximumCorner == box2.maximumCorner;
}

pBoundingBox& operator+(const pBoundingBox &box1, const pBoundingBox &box2)
{
	pBoundingBox newBox;
	glm::vec3 box1Min = box1.getMinimumCorner();
	glm::vec3 box1Max = box1.getMaximumCorner();
	glm::vec3 box2Min = box2.getMinimumCorner();
	glm::vec3 box2Max = box2.getMaximumCorner();

	newBox.minimumCorner[0] = (box1Min[0] < box2Min[0]) ? box1Min[0] : box2Min[0];
	newBox.minimumCorner[1] = (box1Min[1] < box2Min[1]) ? box1Min[1] : box2Min[1];
	newBox.minimumCorner[2] = (box1Min[2] < box2Min[2]) ? box1Min[2] : box2Min[2];

	newBox.maximumCorner[0] = (box1Max[0] > box2Max[0]) ? box1Max[0] : box2Max[0];
	newBox.maximumCorner[1] = (box1Max[1] > box2Max[1]) ? box1Max[1] : box2Max[1];
	newBox.maximumCorner[2] = (box1Max[2] > box2Max[2]) ? box1Max[2] : box2Max[2];

	return newBox;
}