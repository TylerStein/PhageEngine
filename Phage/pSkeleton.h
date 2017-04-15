#pragma once
#include "glm/mat4x4.hpp"
#include "glm/gtx/quaternion.hpp"
#include "glm/gtx/matrix_decompose.hpp"
#include <map>
#include <vector>

//Index integer
typedef uint8_t uIndex; //0-255 joints
#define BONES_PER_VERTEX 256
#define NO_JOINT 999U

/*
struct VertexBoneData {
	std::vector<uIndex> IDs = std::vector<uIndex>(BONES_PER_VERTEX);
	std::vector<float> weights = std::vector<float>(BONES_PER_VERTEX);
};

//Defines a single joint/bone
struct Joint {
	Joint(std::string name) {
		_name = name;

		_parentIndex = 0;
		_inverseTransform = glm::mat4x3();
		_finalTransform = glm::mat4x3();
	}

	void addBoneData(unsigned int ID, float weight) {
		_vertexBoneData.IDs.push_back(ID);
		_vertexBoneData.weights.push_back(weight);
	}

	VertexBoneData _vertexBoneData;

	std::string _name; //Human-readable name
	uIndex _parentIndex; //Parent bone index ID
	glm::mat4x4 _inverseTransform; //Inverse transform matrix
	glm::mat4x4 _finalTransform;
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
	std::string _jointName; //Target joint's name
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

	void setName(char* name) {
		_name = name;
	}

	void setJoints(std::map<uIndex, Joint*> jointMap) {
		_joints = jointMap;
	}

	//Get a joint by index
	Joint* getJoint(uIndex idx) {
		return _joints[idx];
	}

	//Search for a joint in the skeleton (slow search)
	Joint* getJoint(std::string name) {
		return getJoint(getJointIndex(name));
	}

	//Search for a joint in the skeleton and return its index (slow)
	unsigned int getJointIndex(std::string jointName) {
		for (auto iter = _joints.begin(); iter != _joints.end(); iter++) {
			if (iter->second->_name == jointName) {
				return iter->first;
			}
		}
		return NO_JOINT; //Joint not found
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
*/



class Joint {
public:
	Joint(std::string name, uIndex ID = 0, glm::mat4x3 inverseTransform = glm::mat4x3());

	std::string Name() const {
		return _name;
	}

	uIndex ID() const {
		return _ID;
	}

	std::vector<uIndex> ChildrenIDs() const {
		return _childrenIDs;
	}

	uIndex ParentID() const {
		return _parentID;
	}

	glm::mat4x3 InverseTransform() const {
		return _inverseTransform;
	}

	void SetInverseTransform(glm::mat4x3 inverseTransform) {
		_inverseTransform = inverseTransform;
	}

	glm::mat4x3 FinalTransform() const {
		return _finalTransform;
	}

	void SetFinalTransform(glm::mat4x3 finalTransform) {
		_finalTransform = finalTransform;
	}

	void SetID(uIndex newID) { _ID = newID; }

	void AddChild(Joint* child) {
		_children.push_back(child); 
		child->SetParent(this->_ID); 

		_childrenIDs.push_back(child->ID());
	}

	std::vector<Joint*> getChildren() const { return _children; }
	std::vector<uIndex> getChildrenIDs() const { return _childrenIDs; }

	void SetParent(uIndex parentID) { _parentID = parentID; }
	void addJointData(uIndex vertexID, float weight);

private:
	std::string _name;
	uIndex _ID;
	std::vector<uIndex> _childrenIDs;
	uIndex _parentID;

	std::vector<Joint*> _children;
	Joint* _parent;

	std::vector<uIndex> _vertexID; //IDs of vertices this bone should affect
	std::vector<float> _weight; //Weights of the vertex-transform effect

	glm::mat4x3 _inverseTransform;
	glm::mat4x3 _finalTransform;
};

class Skeleton {
public:
	Skeleton(std::string name, std::map<uIndex, Joint*> jointMap = std::map<uIndex, Joint*>());

	std::string getName() const;

	Joint* getJoint(uIndex ID);
	Joint* getJoint(std::string name);

	void addJoint(Joint* joint);

	glm::mat4x3 getLocalTransform(Joint * target) const;
	glm::mat4x3 getGlobalTransform(Joint * target);

	bool isOrganized() const { return _isOrganized; }
	void setOrganized(bool organized) { _isOrganized = organized; }
private:
	Joint* _rootJoint;
	std::map<uIndex, Joint*> _jointMap;
	std::string _name;
	bool _isOrganized;
};
