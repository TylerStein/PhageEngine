#pragma once
#include "pAsset.h"
#include "pSkeleton.h"

/*Animation frame interpolation method
* Linear - No easing, default
* Quadratic - Smooth ease-in-out
* Cubic - Alternate smooth ease-in-out */
enum INTERPOLATION { LINEAR, QUADRATIC, CUBIC }; 

/*Post-animation behavior
* First - Return to first frame and pause
* Stop - Stop animation on the last frame
* Repeat - Return to the first frame and play */
enum ANIMDEFAULT { FIRST, STOP, REPEAT }; 

template <typename ValueType>
struct NodeKey {
public:
	NodeKey(double time, std::string target, ValueType value) {
		_time = time;
		_target = target;
		_value = value;
	}

	double GetTime() const {
		return _time;
	}

	std::string GetTarget() const {
		return _target;
	}

	ValueType GetValue() const {
		return _value;
	}

private:
	double _time;
	std::string _target;
	ValueType _value;
};

class Animation : public pAsset {
public:
	Animation(std::string name, double duration = 10.0, double framerate = 30.0);

	virtual void Update(double time) = 0;
	double GetDuration() const { return _duration; }
	double GetFrameRate() const { return _framerate; }


	glm::vec3 InterpolateVec3(float ratio, glm::vec3& a, glm::vec3& b, INTERPOLATION type = LINEAR);
	glm::quat InterpolateQuat(float ratio, glm::quat& a, glm::quat& b, INTERPOLATION type = LINEAR);

protected:
	double _duration;
	double _framerate;
};

//Holds a target name and the frames meant for it
class AnimationChannel {
public:
	AnimationChannel(std::string name);

	std::string getTargetName() const { return _targetName; }

	//Add a translation key
	void AddTranslationKey(NodeKey<glm::vec3> key);
	void AddTranslationKey(double time, std::string target, glm::vec3 translate) { AddTranslationKey(NodeKey<glm::vec3>(time, target, translate)); }

	//Get the closest translation frame after specified time
	NodeKey<glm::vec3>* FindNextTranslationKey(double time);
	//Get the closest translation frame before the specified time
	NodeKey<glm::vec3>* FindPrevTranslationKey(double time);

	//Add a rotation key
	void AddRotationKey(NodeKey<glm::quat> key);
	void AddRotationKey(double time, std::string target, glm::quat rotate) { AddRotationKey(NodeKey<glm::quat>(time, target, rotate)); }

	//Get the closest rotation frame after specified time
	NodeKey<glm::quat>* FindNextRotationKey(double time);
	//Get the closest rotation frame before the specified time
	NodeKey<glm::quat>* FindPrevRotationKey(double time);

	//Add a scaling key
	void AddScalingKey(NodeKey<glm::vec3> key);
	void AddScalingKey(double time, std::string target, glm::vec3 scaling) { AddScalingKey(NodeKey<glm::vec3>(time, target, scaling)); }

	//Get the closest scaling frame after specified time
	NodeKey<glm::vec3>* FindNextScalingKey(double time);
	//Get the closest scaling frame before the specified time
	NodeKey<glm::vec3>* FindPrevScalingKey(double time);

	//Get translation keys array
	std::vector<NodeKey<glm::vec3>> GetTranslationKeys() const { return _scalingKeys; }

	//Get rotation keys array
	std::vector<NodeKey<glm::quat>> GetRotationKeys() const { return _rotationKeys; }

	//Get scaling keys array
	std::vector<NodeKey<glm::vec3>> GetScalingKeys() const { return _scalingKeys; }

	//Get the number of translation keys present
	unsigned int GetTranslationKeyCount() const { return _translationKeys.size(); }
	//Get the number of rotation keys present
	unsigned int GetRotationKeyCount() const { return _rotationKeys.size(); }
	//Get the number of scaling keys present
	unsigned int GetScalingKeyCount() const { return _scalingKeys.size(); }

private:
	std::vector<NodeKey<glm::vec3>> _translationKeys;
	std::vector<NodeKey<glm::quat>> _rotationKeys;
	std::vector<NodeKey<glm::vec3>> _scalingKeys;

	std::string _targetName;
};

class SkeletalAnimation : public Animation {
public:
	SkeletalAnimation(std::string name, double duration = 10.0, double framerate = 30.0);

	//Update the associated skeleton's pose based on given time and held frames (Inherited via Animation)
	virtual void Update(double time) override;

	

	//Set the target skeleton for this animation
	void SetSkeleton(Skeleton* skeleton) { 
		_skeleton = skeleton; 
		mapAnimationChannels();
	}



	//Get target skeleton reference
	Skeleton* GetSkeleton() const { return _skeleton; }

	AnimationChannel* getChannel(std::string name) const;
	AnimationChannel* getChannel(uIndex index) const;
	void addChannel(AnimationChannel* channel);

	//Links animation channels to joint IDs (requires a skeleton and at least one animation channel)
	void mapAnimationChannels();

private:
	//Searches through each set of frames and matches them to skeleton joints by name
	void UpdateJoints(double time, Joint& joint, glm::mat4& parentTransform);

	Skeleton* _skeleton;

	//Maps animation channel to a joint ID
	std::map<uIndex, AnimationChannel*> _channels;
};