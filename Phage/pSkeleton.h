#pragma once
#include "glm/mat4x4.hpp"
#include "glm/gtx/quaternion.hpp"
#include "glm/gtx/matrix_decompose.hpp"
#include <map>
#include <vector>

//Index integer
typedef uint8_t uIndex; //0-255 joints
#define BONES_PER_VERTEX 4U
#define MAX_BONES 255U
#define NO_JOINT 999U


struct VertexJointData {
	//The joints, by index, this vertex is affected by
	uIndex JointIDs[BONES_PER_VERTEX];

	//The weight of the associated joints' effect (0-1)
	float VertexWeights[BONES_PER_VERTEX];
};

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

private:
	std::string _name;
	uIndex _ID;
	std::vector<uIndex> _childrenIDs;
	uIndex _parentID;

	std::vector<Joint*> _children;
	Joint* _parent;

	glm::mat4x3 _inverseTransform;
	glm::mat4x3 _finalTransform;
};

class Skeleton {
public:
	Skeleton(std::string name, std::map<uIndex, Joint*> jointMap = std::map<uIndex, Joint*>());

	std::string getName() const;

	Joint* getJoint(uIndex ID);
	Joint* getJoint(std::string name);

	std::vector<Joint*> getAllJoints();

	void addJoint(Joint* joint);

	glm::mat4x3 getLocalTransform(Joint * target) const;
	glm::mat4x3 getGlobalTransform(Joint * target);

	bool isOrganized() const { return _isOrganized; }
	void setOrganized(bool organized) { _isOrganized = organized; }

	void addJointData(unsigned int vertexID, uIndex jointID, float vertexWeight);
	
	std::vector<glm::mat4> getJointTransforms();

	std::vector<VertexJointData*> getOrderedJointData();
	std::map<unsigned int, VertexJointData*> getVertexJointData() const { return _vertexJointData; }

	std::vector<unsigned int> quickSort(std::vector<unsigned int> arr, unsigned int left, unsigned int right);

private:
	Joint* _rootJoint;
	std::map<uIndex, Joint*> _jointMap;

	//Map <vertID, bone/weight data>
	std::map<unsigned int, VertexJointData*> _vertexJointData;
	std::string _name;
	bool _isOrganized;
};
