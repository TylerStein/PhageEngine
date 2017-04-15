#include "pSkeleton.h"

Joint::Joint(std::string name, uIndex ID, glm::mat4x3 inverseTransform)
{
	_name = name;
	_ID = ID;
	_inverseTransform = inverseTransform;
}

void Joint::addJointData(uIndex vertexID, float weight)
{
	_vertexID.push_back(vertexID);
	_weight.push_back(weight);

#ifdef _DEBUG
	//printf("Added joint-vertex data (VertID %i) (Weight %f.2)\n", vertexID, weight);
#endif
}


Skeleton::Skeleton(std::string name, std::map<uIndex, Joint*> jointMap)
{
	_name = name;
	_jointMap = jointMap;
	_isOrganized = false;
}

std::string Skeleton::getName() const
{
	return _name;
}

Joint * Skeleton::getJoint(uIndex ID)
{
	Joint* res = _jointMap.at(ID);
	if (res != nullptr) {
		return res;
	}

#ifdef _DEBUG
		printf("Failed to retreive joint with ID %i\n", ID);
#endif
	
	return nullptr;
}

Joint * Skeleton::getJoint(std::string name)
{
	for (auto iter : _jointMap) {
		if (iter.second->Name() == name) {
			return iter.second;
		}
	}

#ifdef _DEBUG
	printf("Failed to retreive joint with name %s\n", name.c_str());
#endif

	return nullptr;
}

void Skeleton::addJoint(Joint* joint)
{
	uIndex id = _jointMap.size();
		if (id == 0) { _rootJoint = joint; }
	joint->SetID(id);
	_jointMap[id] = joint;


#ifdef _DEBUG
	printf("Added a joint (ID %i)\n", joint->ID());
#endif
}



glm::mat4x3 Skeleton::getLocalTransform(Joint * target) const
{
	return target->InverseTransform();
}

glm::mat4x3 Skeleton::getGlobalTransform(Joint * target)
{
	//Recursively transform by parent to get global
	if (target->ParentID() > 0) {
		glm::mat4 parentTransform = getGlobalTransform(getJoint(target->ParentID()));
		glm::mat4 globalTransform = target->InverseTransform();

		globalTransform = globalTransform * parentTransform;

		return globalTransform;
	}

	//Target has no parent, this is global transform
	return target->InverseTransform();
}