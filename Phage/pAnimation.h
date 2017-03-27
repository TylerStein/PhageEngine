#pragma once
#include "pAsset.h"
#include "pSkeleton.h"

//Trigger takes a time and callback, can be attached to an animation clip
struct AnimationTrigger {
	AnimationTrigger(void* callbackFunc) {
		_callbackFunc = callbackFunc;
	}

	double _triggerTime;
	void* _callbackFunc;
};

struct AnimationPose {
	AnimationPose(glm::vec3 pos, glm::quat rot, glm::vec3 scl) {
		_position = pos;
		_rotation = rot;
		_scale = scl;
	}

	glm::vec3 _position;
	glm::quat _rotation;
	glm::vec3 _scale;
};

class pAnimationClip : public pAsset {
public:
	pAnimationClip(std::string name);
	~pAnimationClip();

	void setStartTime(float time);
	float getStartTime() const;

	void setEndTime(float time);
	float getEndTime() const;

protected:
	float _startTime; //Start of the clip playing on the timeline
	float _endTime; //End of the clip playing on the timeline

	pAnimationClip* _animation; //Array of animations
	float* _animationStart;


	float _duration;
};

//Animations targeted at transforming nodes
class pBasicAnimationClip : public pAnimationClip {
	//TODO: Non-skeletal animation
	AnimationPose* _animationPoses; //Array of node transform samples/frames
};

class pVertexAnimationClip : public pAnimationClip {
	//TODO: Mesh-vertex animation4
public:
	pVertexAnimationClip(std::string name);
};


//A single skeletal animation keyframe/sampple
struct SkeletalAnimationSample {
	unsigned int _jointCount; //Number of joint poses present
	JointPose* _jointPoses; //Array of joint poses for this frame
};


//Animations targeted at skeletal rigs
class pSkeletalAnimationClip : public pAnimationClip {
	SkeletalAnimationSample* _animationSamples; //Skeletal animation samples/frames
	Skeleton* _skeleton; //Skeleton this animation clip is intended for
};


//Base class for animations
class pAnimation : public pAsset {
public:
	pAnimation(std::string name, double duration = 10.0f, double frameRate = 30.0f, bool loop = false);

	bool isLooping() const;
	double duration() const;
	double framesPerSecond() const;

	void setLooping(bool toLoop);
	void setPlaybackRate(double newFramesPerSecond);


protected:
	pAnimationClip* _animationsFrames;

	double _framesPerSecond; //Desired playback rate
	unsigned int _duration; //Duration of animation clip
	bool _isLooping; //Is this animation supposed to loop?
};


