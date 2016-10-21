#include "pMatrixStack.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>



pMatrixStack::pMatrixStack()
{
	glm::mat4 ident(1.0f), ident2(1.0);
	projectionStack.push(ident);
	modelStack.push(ident2);
}

pMatrixStack::~pMatrixStack()
{
}

void pMatrixStack::orthographic(float xMin, float xMax, float yMin, float yMax, float zMin, float zMax)
{
	projectionStack.top() = glm::ortho(xMin, xMax, yMin, yMax, zMin, zMax);
	projectionStackDirty = true;
}

void pMatrixStack::perspective(float fovy, float aspect, float nearZ, float farZ)
{
	projectionStack.top() = glm::perspective(fovy, aspect, nearZ, farZ);
	projectionStackDirty = true;
}

const glm::mat4 pMatrixStack::getProjectionMatrix() const
{
	return projectionStack.top();
}

const glm::mat4 pMatrixStack::getModelMatrix() const
{
	return modelStack.top();
}

const float* pMatrixStack::getProjectionMatrixValue() const
{
	return glm::value_ptr(getProjectionMatrix());
}

const float* pMatrixStack::getModelMatrixValue() const
{
	return glm::value_ptr(getModelMatrix());
}

void pMatrixStack::getProjectionMatrixValue(float *values) const
{
	const float *vals = getProjectionMatrixValue();
	for (int i = 0; i < 16; i++) values[i] = vals[i];
}

void pMatrixStack::getModelMatrixValue(float *values) const
{
	const float *vals = getModelMatrixValue();
	for (int i = 0; i < 16; i++) values[i] = vals[i];
}

void pMatrixStack::setProjectionToIdentity()
{
	projectionStack.top() = glm::mat4(1.0);
	projectionStackDirty = true;
}

void pMatrixStack::setModelViewToIdentity()
{
	modelStack.top() = glm::mat4(1.0);
	modelStackDirty = true;
}

void pMatrixStack::pushModelView()
{
	modelStack.push(modelStack.top());
}

void pMatrixStack::popModelView()
{
	modelStack.pop();
	modelStackDirty = true;
}

void pMatrixStack::pushProjection()
{
	projectionStack.push(projectionStack.top());
}

void pMatrixStack::popProjection()
{
	projectionStack.pop();
	projectionStackDirty = true;
}

void pMatrixStack::rotateBy(const glm::quat& q)
{
	float angle = glm::angle(q);
	glm::vec3 vect = glm::axis(q);

	modelStack.top() = modelStack.top() * glm::rotate(angle, vect);
	modelStackDirty = true;
}

void pMatrixStack::rotateBy(float angle, float vx, float vy, float vz)
{
	modelStack.top() = modelStack.top() * glm::rotate(angle, glm::vec3(vx, vy, vz));
	modelStackDirty = true;
}

void pMatrixStack::scaleBy(float xFactor, float yFactor, float zFactor)
{
	modelStack.top() = modelStack.top() * glm::scale(glm::vec3(xFactor, yFactor, zFactor));
	modelStackDirty = true;
}


void pMatrixStack::translateBy(float xAmt, float yAmt, float zAmt)
{
	modelStack.top() = modelStack.top() * glm::translate(glm::vec3(xAmt, yAmt, zAmt));
	modelStackDirty = true;
}

void pMatrixStack::lookAt(const glm::vec3& posn, const glm::vec3& at, const glm::vec3& up)
{
	modelStack.top() = glm::lookAt(posn, at, up);
	modelStackDirty = true;
}

void pMatrixStack::lookAt(float camX, float camY, float camZ, float atX, float atY, float atZ, float upX, float upY, float upZ)
{
	lookAt(glm::vec3(camX, camY, camZ), glm::vec3(atX, atY, atZ), glm::vec3(upX, upY, upZ));
	modelStackDirty = true;
}

bool pMatrixStack::isModelViewDirty() const
{
	return modelStackDirty;
}

bool pMatrixStack::isProjectionDirty() const
{
	return projectionStackDirty;
}

void pMatrixStack::clearModelViewDirty()
{
	modelStackDirty = false;
}

void pMatrixStack::clearProjectionDirty()
{
	projectionStackDirty = false;
}