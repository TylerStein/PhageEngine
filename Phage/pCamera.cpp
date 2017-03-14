#include "pCamera.h"

#include "glm/gtx/vector_angle.hpp"

pCamera::pCamera()
{
	projectionMatrix = glm::mat4(1);
	minRenderDistance = 0.05f;
	maxRenderDistance = 256.0f;
	lastWindowWidth = 1.0f;
	lastWindowHeight = 1.0f;
	FOV = 60.0f;
	calculateProjectionMatrix(100.0f, 100.0f);
}

pCamera::pCamera(float _windowWidth, float _windowHeight, float _fieldOfView, float _farPlane, float _nearPlane)
{
	minRenderDistance = _nearPlane;
	maxRenderDistance = _farPlane;
	lastWindowHeight = _windowHeight;
	lastWindowWidth = _windowWidth;
	FOV = _fieldOfView;

	calculateProjectionMatrix(lastWindowWidth, lastWindowHeight);
}

pCamera::~pCamera()
{
}

float pCamera::getFOV()
{
	return FOV;
}

void pCamera::setFOV(float _fieldOfView)
{
	FOV = _fieldOfView;
	calculateProjectionMatrix(lastWindowWidth, lastWindowHeight);
}

glm::mat4 pCamera::getProjectionMatrix()
{
	return projectionMatrix;
}

void pCamera::calculateProjectionMatrix(float _windowWidth, float _windowHeight)
{
	float ratio = _windowWidth / _windowHeight;
	float top = tan(FOV * glm::pi<float>() / 360.0f) * minRenderDistance;
	float bottom = -top;
	float left = ratio * bottom;
	float right = ratio * top;
	projectionMatrix = glm::frustum(left, right, bottom, top, minRenderDistance, maxRenderDistance);
}
