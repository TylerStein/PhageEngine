#pragma once

#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/quaternion.hpp"

class pCamera {
public:
	pCamera();
	pCamera(float _windowWidth, float _windowHeight, float _fieldOfView = 60.0f, float _farPlane = 256.0f, float _nearPlane = 0.05f);
	~pCamera();

	float getFOV();
	void setFOV(float _fieldOfView);

	glm::mat4 getProjectionMatrix();
private:
	glm::mat4 projectionMatrix;
	glm::mat4 viewMatrix;

	float FOV;

	float maxRenderDistance;
	float minRenderDistance;

	float lastWindowWidth;
	float lastWindowHeight;

	void calculateProjectionMatrix(float _windowWidth, float _windowHeight);
};