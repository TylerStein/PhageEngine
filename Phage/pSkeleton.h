#pragma once
#include "glm/mat4x4.hpp"
#include "glm/gtx/quaternion.hpp"
#include "glm/gtx/matrix_decompose.hpp"
#include <map>
#include <vector>

//Index integer
typedef uint8_t uIndex; //0-255 joints
#define BONES_PER_VERTEX 256

struct VertexBoneData {
	std::vector<uint16_t> IDs = std::vector<uint16_t>(BONES_PER_VERTEX);
	std::vector<float> weights = std::vector<float>(BONES_PER_VERTEX);
};

//Defines a single joint/bone
struct Joint {
	Joint(char* name) {
		_name = name;

		_parentIndex = 0;
		_inverseTransform = glm::mat4x3();
	}

	void addBoneData(unsigned int ID, float weight) {
		_vertexBoneData.IDs.push_back(ID);
		_vertexBoneData.weights.push_back(weight);
	}

	VertexBoneData _vertexBoneData;

	char* _name; //Human-readable name
	uIndex _parentIndex; //Parent bone index ID
	glm::mat4x3 _inverseTransform; //Inverse transform matrix (4x3 no shearing)
};

//Defines a single pose of a joint
struct JointPose {
	JointPose(std::string jointName, glm::vec3 translation, glm::quat rotation, float scale) {
		_translation = translation;
		_rotation = rotation;
		_scale = scale;
		_jointName = jointName;
	}

	glm::quat _rotation;
	glm::vec3 _translation;
	float _scale; //Uniform scale only
	std::string _jointName;
};

//Defines a single skeleton, composed of joints
class Skeleton {
	friend class pModelLoader;
public:
	Skeleton(char* name = "", std::map<uIndex, Joint*> joints = std::map<uIndex, Joint*>()) {
		_jointCount = (uIndex)(joints.size());
		_name = name;

		_joints = std::map<uIndex, Joint*>(joints);
	}

	Joint* getJoint(uIndex idx) {
		return _joints[idx];
	}
	 
	char* getName() const {
		return _name;
	}

	glm::mat4x3 getLocalTransform(Joint* target) const; //Get transform relative to parent joint
	glm::mat4x3 getGlobalTransform(Joint* target); //Get transform relative to root joint/model space

private:
	std::map<uIndex, Joint*> _joints; //Map of <joint index, joint>

	char* _name; //Name identifier
	uIndex _jointCount; //Number of joints present on this skeleton
};


//Defines a single pose of a skeleton
struct SkeletonPose {
	Skeleton* _skeleton; //Skeleton this pose is meant for

	JointPose* _pose; //Array of joint poses

	
	
};

