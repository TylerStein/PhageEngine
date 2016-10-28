#ifndef PFRUSTUM
#define PFRUSTUM

#include "pPlane.h"
#include <glm/glm.hpp>
#include "pBoundingBox.h"

class pFrustum
{
private:
	enum Planes {nearPlane, farPlane, topPlane, bottomPlane, leftPlane, rightPlane};
	pPlane planes[6];
	glm::vec3 nearTopLeft, nearBottomLeft, nearBottomRight, nearTopRight, farTopLeft, farBottomLeft, farBottomRight, farTopRight;
	glm::vec3 cameraPos;
	glm::vec3 viewDirection;
	glm::vec3 nearCentre;
	glm::vec3 farCentre;
	glm::vec3 up;
	glm::vec3 right;

	float nearPlaneDistance;
	float farPlaneDistance;
	float nearPlaneWidth;
	float nearPlaneHeight;
	float farPlaneWidth;
	float farPlaneHeight;
	float ratio;
	float fovAngle;
	float tangent;
	
public:
	void onWindowResize(float _angle, float _ratio, float distanceToNearPlane, float distanceToFarPlane);

	void onCameraChange(glm::vec3 &position, glm::vec3 &lookAt, glm::vec3 &up);

	bool inside(glm::vec3 &point);

	bool inside(glm::vec3 &center, float radius);

	bool inside(pBoundingBox &bBox);
};


#endif

