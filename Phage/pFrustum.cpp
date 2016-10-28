#include "pFrustum.h"
//#include "pBoundingBox"

#define ANGLETORAD 3.14159265358979323846/180.0

void pFrustum::onWindowResize(float _angle, float _ratio, float distanceToNearPlane, float distanceToFarPlane)
{
	fovAngle = _angle;
	ratio = _ratio;
	nearPlaneDistance = distanceToNearPlane;
	farPlaneDistance = distanceToFarPlane;

	tangent = (float) tan(ANGLETORAD * fovAngle * 0.5);
	nearPlaneHeight = nearPlaneDistance * tangent;
	nearPlaneWidth = nearPlaneHeight * ratio;
	farPlaneHeight = farPlaneDistance * tangent;
	farPlaneWidth = farPlaneHeight * ratio;
}

void pFrustum::onCameraChange(glm::vec3 & position, glm::vec3 & lookAt, glm::vec3 & _up)
{
	viewDirection = glm::normalize(position - lookAt);
	right = glm::normalize(glm::cross(_up, viewDirection));
	up = glm::cross(viewDirection, right);
	nearCentre = position - (viewDirection * nearPlaneDistance);
	farCentre = position - (viewDirection * farPlaneDistance);

	nearTopLeft = nearCentre + up * nearPlaneHeight - right * nearPlaneWidth;
	nearTopRight = nearCentre + up * nearPlaneHeight + right * nearPlaneWidth;
	nearBottomLeft = nearCentre - up * nearPlaneHeight - right * nearPlaneWidth;
	nearBottomRight = nearCentre - up * nearPlaneHeight + right * nearPlaneWidth;

	farTopLeft = farCentre + up * farPlaneHeight - right * farPlaneWidth;
	farTopRight = farCentre + up * farPlaneHeight + right * farPlaneWidth;
	farBottomLeft = farCentre - up * farPlaneHeight - right * farPlaneWidth;
	farBottomRight = farCentre - up * farPlaneHeight + right * farPlaneWidth;

	planes[topPlane] = pPlane(nearTopRight, nearTopLeft, farTopLeft);
	planes[bottomPlane] = pPlane(nearBottomLeft, nearBottomRight, farBottomRight);
	planes[leftPlane] = pPlane(nearTopLeft, nearBottomLeft, farBottomLeft);
	planes[rightPlane] = pPlane(nearBottomRight, nearTopRight, farBottomRight);
	planes[nearPlane] = pPlane(nearTopLeft, nearTopRight, nearBottomRight);
	planes[farPlane] = pPlane(farTopRight, nearTopLeft, nearBottomLeft);	
}

bool pFrustum::inside(glm::vec3 & point)
{
	for (int i = 0; i < 6; i++)
	{
		if (planes[i].signedDistance(point) < 0)
		{
			return false;
		}
	}
	return true;
}

bool pFrustum::inside(glm::vec3 & center, float radius)
{
	float distance;

	for (int i = 0; i < 6; i++)
	{
		distance = planes[i].signedDistance(center);
		if (distance < -radius) return false;
		else if (distance < radius) return true;
	}

	return true;
}

/*
bool pFrustum::inside(pBoundingBox & bBox)
{
	for (int i = 0; i < 6; i++)
	{
		if (planes[i].signedDistance(bBox.getClosestOnNormal(planes[i].getNormal())) < 0)
			return false;
		else if (planes[i].signedDistance(bBox.getFurthestOnNormal(planes[i].getNormal())) < 0)
			return true;
	}
	return true;
}
*/

