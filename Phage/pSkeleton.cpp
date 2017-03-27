#include "pSkeleton.h"

glm::mat4x3 Skeleton::getLocalTransform(Joint * target) const
{
	return target->_inverseTransform;
}

glm::mat4x3 Skeleton::getGlobalTransform(Joint * target)
{
	//Recursively transform by parent to get global
	if (target->_parentIndex > 0) {
		glm::mat4 parentTransform = getGlobalTransform(getJoint(target->_parentIndex));
		glm::mat4 globalTransform = target->_inverseTransform;

		globalTransform = globalTransform * parentTransform;

		return globalTransform;
	}

	//Target has no parent, this is global transform
	return target->_inverseTransform;
}
