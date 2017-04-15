#include "pAnimation.h"
#include "glm/gtc/quaternion.hpp"

#include "Interpolation.h"

Animation::Animation(std::string name, double duration, double framerate) : pAsset() {
	this->name = name;
	_duration = duration;
	_framerate = framerate;
}

glm::vec3 Animation::InterpolateVec3(float ratio, glm::vec3& a, glm::vec3& b, INTERPOLATION type)
{
	switch (type) {
	case LINEAR:
		return glm::mix(a, b, ratio);
		break;
	case QUADRATIC:
		return Interpolation::easeInOut_Quadratic(a, b, ratio);
		break;
	case CUBIC:
		return Interpolation::easeInOut_Cubic(a, b, ratio);
		break;
	}

	return glm::vec3();
}

glm::quat Animation::InterpolateQuat(float ratio, glm::quat& a, glm::quat& b, INTERPOLATION type)
{
	switch (type) {
	case LINEAR:
		return glm::lerp(a, b, ratio);
		break;
	case QUADRATIC:
		return glm::slerp(a, b, ratio);
		break;
	case CUBIC:
		return glm::slerp(a, b, ratio);
		break;
	}

	return glm::quat();
}

SkeletalAnimation::SkeletalAnimation(std::string name, double duration, double framerate) : Animation(name, duration, framerate)
{
	//this->name = name;
	//this->_duration = duration;
	//this->_framerate = framerate;
}

void SkeletalAnimation::Update(double time)
{
	
}

AnimationChannel::AnimationChannel(std::string name)
{
	_targetName = name;
}

void AnimationChannel::AddTranslationKey(NodeKey<glm::vec3> key)
{
	if (!_translationKeys.empty()) {
		//Iterate backwards through frames
		for (auto iter = _translationKeys.end() - 1; iter != _translationKeys.begin(); iter--) {
			//Find the first frame with a lower timestamp than this one
			if (iter->GetTime() <= key.GetTime()) {
				//Emplace the new frame in the ordered array
				_translationKeys.emplace(iter, key);
				return;
			}
		}
	}

	_translationKeys.push_back(key);
}

NodeKey<glm::vec3>* AnimationChannel::FindNextTranslationKey(double time)
{
	//Iterate forwards through frames
	for (auto iter = _translationKeys.begin(); iter != _translationKeys.end(); iter++) {
		//Find the first frame with an equal or higher timestamp than the given time
		if (iter->GetTime() >= time) {
			return iter._Ptr;
		}
	}

	return _translationKeys.end()._Ptr;
}

NodeKey<glm::vec3>* AnimationChannel::FindPrevTranslationKey(double time)
{
	//Iterate backwards through frames
	for (auto iter = _translationKeys.end() - 1; iter != _translationKeys.begin(); iter--) {
		//Find the first frame with an equal or lower timestamp than the given time
		if (iter->GetTime() <= time) {
			return iter._Ptr;
		}
	}

	return _translationKeys.begin()._Ptr;
}

void AnimationChannel::AddRotationKey(NodeKey<glm::quat> key)
{
	if (!_rotationKeys.empty()) {
		//Iterate backwards through frames
		for (auto iter = _rotationKeys.end() - 1; iter != _rotationKeys.begin(); iter--) {
			//Find the first frame with a lower timestamp than this one
			if (iter->GetTime() <= key.GetTime()) {
				//Emplace the new frame in the ordered array
				_rotationKeys.emplace(iter, key);
				return;
			}
		}
	}
	_rotationKeys.push_back(key);
}

NodeKey<glm::quat>* AnimationChannel::FindNextRotationKey(double time)
{
	//Iterate forwards through frames
	for (auto iter = _rotationKeys.begin(); iter != _rotationKeys.end(); iter++) {
		//Find the first frame with an equal or higher timestamp than the given time
		if (iter->GetTime() >= time) {
			return iter._Ptr;
		}
	}

	return _rotationKeys.end()._Ptr;
}

NodeKey<glm::quat>* AnimationChannel::FindPrevRotationKey(double time)
{
	//Iterate backwards through frames
	for (auto iter = _rotationKeys.end() - 1; iter != _rotationKeys.begin(); iter--) {
		//Find the first frame with an equal or lower timestamp than the given time
		if (iter->GetTime() <= time) {
			return iter._Ptr;
		}
	}

	return _rotationKeys.begin()._Ptr;
}

void AnimationChannel::AddScalingKey(NodeKey<glm::vec3> key)
{
	if (!_scalingKeys.empty()) {
		//Iterate backwards through frames
		for (auto iter = _scalingKeys.end() - 1; iter != _scalingKeys.begin(); iter--) {
			//Find the first frame with a lower timestamp than this one
			if (iter->GetTime() <= key.GetTime()) {
				//Emplace the new frame in the ordered array
				_scalingKeys.emplace(iter, key);
				return;
			}
		}
	}

	_scalingKeys.push_back(key);
}

NodeKey<glm::vec3>* AnimationChannel::FindNextScalingKey(double time)
{
	//Iterate forwards through frames
	for (auto iter = _scalingKeys.begin(); iter != _scalingKeys.end(); iter++) {
		//Find the first frame with an equal or higher timestamp than the given time
		if (iter->GetTime() >= time) {
			return iter._Ptr;
		}
	}

	return _scalingKeys.end()._Ptr;
}

NodeKey<glm::vec3>* AnimationChannel::FindPrevScalingKey(double time)
{
	//Iterate backwards through frames
	for (auto iter = _scalingKeys.end() - 1; iter != _scalingKeys.begin(); iter--) {
		//Find the first frame with an equal or lower timestamp than the given time
		if (iter->GetTime() <= time) {
			return iter._Ptr;
		}
	}

	return _scalingKeys.begin()._Ptr;
}

AnimationChannel * SkeletalAnimation::getChannel(std::string name) const
{
	for (auto iter : _channels) {
		if (iter.second->getTargetName() == name) {
			return iter.second;
		}
	}

	printf("Unable to find channel for node named %s\n", name.c_str());

	return nullptr;
}

AnimationChannel * SkeletalAnimation::getChannel(uIndex index) const
{
	return _channels.at(index);
}

void SkeletalAnimation::addChannel(AnimationChannel* channel)
{
	//Channel defaults with an invalid id
	_channels[_channels.size() - 1] = channel;
}

void SkeletalAnimation::mapAnimationChannels()
{
	//Check for valid channels and skeleton
	if (_channels.empty() == false && _skeleton != nullptr) {
		std::map<uIndex, AnimationChannel*> newChannelMap = std::map<uIndex, AnimationChannel*>();

		//Iterate through each animation channel
		for (auto iter : _channels) {
			//Attempt to get a joint from skeleton with a matching name
			Joint* jointref = _skeleton->getJoint(iter.second->getTargetName());
			if (jointref != nullptr) {
				//Match found, update the new map
				newChannelMap[jointref->ID()] = iter.second;
			}
		}

		_channels.clear();
		_channels = std::map<uIndex, AnimationChannel*>(newChannelMap);
		newChannelMap.clear();
	}
}

void SkeletalAnimation::UpdateJoints(double time, Joint & joint, glm::mat4 & parentTransform)
{
	/*glm::mat4 translationMatrix = glm::mat4();
	glm::mat4 rotationMatrix = glm::mat4();
	glm::mat4 scalingMatrix = glm::mat4();

	//Find surrounding scale frames, generate scaling matrix from interpolation
	for (auto iter = _jointScalings.begin(); iter != _jointScalings.end(); iter++) {
		if (_skeleton->getJoint(iter->second) == &joint) {
			auto prevFrame = FindPrevScalingKey(time);
			auto nextFrame = FindNextScalingKey(time);

			//Get the (0 to 1) position-ratio between the surrounding frames at this time
			double relativeEndTime = prevFrame->GetTime() - prevFrame->GetTime();
			double ratio = relativeEndTime / (time - prevFrame->GetTime());

			//Get the interpolated values
			glm::vec3 interp = InterpolateVec3(ratio, prevFrame->GetValue(), nextFrame->GetValue());

			scalingMatrix = glm::scale(glm::mat4(), interp);

			break;
		}
	}

	//Find surrounding rotation frames, generate rotation matrix from interpolation
	for (auto iter = _jointRotations.begin(); iter != _jointRotations.end(); iter++) {
		if (_skeleton->getJoint(iter->second) == &joint) {
			auto prevFrame = FindPrevRotationKey(time);
			auto nextFrame = FindNextRotationKey(time);

			//Get the (0 to 1) position-ratio between the surrounding frames at this time
			double relativeEndTime = prevFrame->GetTime() - prevFrame->GetTime();
			double ratio = relativeEndTime / (time - prevFrame->GetTime());

			//Get the interpolated values
			glm::quat interp = InterpolateQuat(ratio, prevFrame->GetValue(), nextFrame->GetValue());

			rotationMatrix = glm::toMat4(interp);

			break;
		}
	}

	//Find surrounding translation frames, generate translation matrix from interpolation
	for (auto iter = _jointTranslations.begin(); iter != _jointTranslations.end(); iter++) {
		if (_skeleton->getJoint(iter->second) == &joint) {
			auto prevFrame = FindPrevTranslationKey(time);
			auto nextFrame = FindNextTranslationKey(time);

			//Get the (0 to 1) position-ratio between the surrounding frames at this time
			double relativeEndTime = prevFrame->GetTime() - prevFrame->GetTime();
			double ratio = relativeEndTime / (time - prevFrame->GetTime());

			//Get the interpolated values
			glm::vec3 interp = InterpolateVec3(ratio, prevFrame->GetValue(), nextFrame->GetValue());

			translationMatrix = glm::translate(glm::mat4(), interp);

			break;
		}
	}

	//Combine matrices into final transformation matrix
	glm::mat4 transformMatrix = translationMatrix * rotationMatrix * scalingMatrix;

	glm::mat4 globalTransform = parentTransform * transformMatrix;

	joint.SetFinalTransform(transformMatrix);*/
}

/*
void SkeletalAnimation::GenerateJointAnimationMaps()
{
	//Reset joint-animation maps
	_jointTranslations.clear();
	_jointRotations.clear();
	_jointScalings.clear();

	//Map translation keys
	for (unsigned int i = 0; i <= _translationKeyCount; i++) {
		std::string targetJoint = _translationKeys[i].GetTarget();
		unsigned int foundJointIndex = _skeleton->getJoint(targetJoint)->ID();

		if (foundJointIndex != NO_JOINT) {
			_jointTranslations[i] = foundJointIndex;
		}
	}

	//Map rotation keys
	for (unsigned int i = 0; i <= _rotationKeyCount; i++) {
		std::string targetJoint = _rotationKeys[i].GetTarget();
		unsigned int foundJointIndex = _skeleton->getJoint(targetJoint)->ID();

		if (foundJointIndex != NO_JOINT) {
			_jointRotations[i] = foundJointIndex;
		}
	}

	//Map scaling keys
	for (unsigned int i = 0; i <= _scalingKeyCount; i++) {
		std::string targetJoint = _scalingKeys[i].GetTarget();
		unsigned int foundJointIndex = _skeleton->getJoint(targetJoint)->ID();

		if (foundJointIndex != NO_JOINT) {
			_jointScalings[i] = foundJointIndex;
		}
	}
}*/