#include "pSkeleton.h"

Joint::Joint(std::string name, uIndex ID, glm::mat4x3 inverseTransform)
{
	_name = name;
	_ID = ID;
	_inverseTransform = inverseTransform;
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

std::vector<Joint*> Skeleton::getAllJoints()
{
	std::vector<Joint*> res;

	for (auto iter = _jointMap.begin(); iter != _jointMap.end(); iter++) {
		res.push_back(iter->second);
	}

	return res;
}

void Skeleton::addJoint(Joint* joint)
{
	uIndex id = _jointMap.size();
	if (id == 0) { _rootJoint = joint; }
	joint->SetID(id);
	_jointMap[id] = joint;


#ifdef _DEBUG
	//printf("Added a joint (ID %i)\n", joint->ID());
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

void Skeleton::addJointData(unsigned int vertexID, uIndex jointID, float vertexWeight)
{
	if (_vertexJointData.find(vertexID) == _vertexJointData.end()) {
		_vertexJointData[vertexID] = new VertexJointData();
	}

	VertexJointData* ref = _vertexJointData[vertexID];

	for (int i = 0; i < BONES_PER_VERTEX; i++) {
		if (ref->VertexWeights[i] == 0.0) {
			ref->JointIDs[i] = jointID;
			ref->VertexWeights[i] = vertexWeight;
			return;
		}
	}

#ifdef _DEBUG
	printf("Trying to add too much joint data!\n");
#endif
}

std::vector<glm::mat4> Skeleton::getJointTransforms()
{
	std::vector<glm::mat4> res = std::vector<glm::mat4>();

	for (int i = 0; i < _jointMap.size(); i++) {
		res.push_back(_jointMap[i]->FinalTransform());
	}

	return res;
}

std::vector<VertexJointData*> Skeleton::getOrderedJointData()
{
	std::vector<VertexJointData*> res = std::vector<VertexJointData*>();

	for (int i = 0; i < _jointMap.size(); i++) {
		res.push_back(_vertexJointData[i]);
	}

	/*std::vector<unsigned int> sortedIndeces = std::vector<unsigned int>();

	for (auto iter : _vertexJointData) {
		sortedIndeces.push_back(iter.first);
	}

	sortedIndeces = quickSort(sortedIndeces, 0, sortedIndeces.size() - 1);

	for (auto iter : sortedIndeces) {
		res.push_back(_vertexJointData[iter]);
	}*/

	return res;
}

std::vector<unsigned int> Skeleton::quickSort(std::vector<unsigned int> arr, unsigned int left, unsigned int right)
{
	unsigned int i = left, j = right;
	unsigned int tmp;
	unsigned int pivot = arr[(left + right) / 2];

	//Partition
	while (i <= j) {
		while (arr[i] < pivot) {
			i++;
		}
		while (arr[j] > pivot) {
			j--;
		}
		if (i <= j) {
			tmp = arr[i];
			arr[i] = arr[j];
			arr[j] = tmp;
			i++;
			j--;
		}
	}

	//Recursion
	if (left < j) { arr = quickSort(arr, left, j); }
	if (i < right) { arr = quickSort(arr, i, right); }

	return arr;
}
