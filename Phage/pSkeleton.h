#pragma once
#include "glm/mat4x4.hpp"
#include "glm/gtx/quaternion.hpp"
#include "glm/gtx/matrix_decompose.hpp"

//Defines a single joint/bone
struct Joint {
	Joint(char* name, unsigned int parentIndex, glm::mat4x4 inverseTransform) {
		_name = name;
		_parentIndex = parentIndex;
		_inverseTransform = inverseTransform;
	}

	char* _name; //Human-readable name
	unsigned int _parentIndex; //Parent bone index ID
	glm::mat4x4 _inverseTransform; //Inverse transform matrix
};

//Defines a single pose of a joint
struct JointPose {
	JointPose(glm::vec3 translation, glm::vec3 rotation, glm::vec3 scale) {
		_translation = translation;
		_rotation = rotation;
		_scale = scale;
	}

	glm::quat _rotation;
	glm::vec3 _translation;
	glm::vec3 _scale;
};

//Defines a single skeleton, composed of joints
struct Skeleton {
	Skeleton(char* name, Joint* joints, unsigned int jointCount) {
		_joints = joints;
		_jointCount = jointCount;
		_name = name;
	}
	 
	char* getName() const {
		return _name;
	}

	char* _name; //Name identifier
	Joint* _joints; //Array of joints
	unsigned int _jointCount; //Number of joints present on this skeleton
};


//Defines a single pose of a skeleton
struct SkeletonPose {
	Skeleton* _skeleton; //Skeleton this pose is meant for
	unsigned int _skeletonID; //ID of the Skeleton this pose is meant for
	

	JointPose* _pose; //Array of joint poses

	
	
};

