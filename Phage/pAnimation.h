#pragma once
#include "pSceneNode.h"
#include "pSkeleton.h"

//Trigger takes a time and callback, can be attached to an animation clip
struct AnimationTrigger {
	AnimationTrigger(void* callbackFunc) {
		_callbackFunc = callbackFunc;
	}

	double _triggerTime;
	void* _callbackFunc;
};


//Base class for animations
class pAnimationClip : public pAsset {
public:
	pAnimationClip(std::string name, double duration, double frameRate, bool loop = false);

	bool isLooping() const;
	double duration() const;
	double framesPerSecond() const;

	void setLooping(bool toLoop);
	void setPlaybackRate(double newFramesPerSecond);
protected:
	double _framesPerSecond; //Desired playback rate
	unsigned int _duration; //Duration of animation clip
	bool _isLooping; //Is this animation supposed to loop?
};


//A single skeletal animation keyframe/sampple
struct SkeletalAnimationSample {
	unsigned int _jointCount; //Number of joint poses present
	JointPose* _jointPoses; //Array of joint poses for this frame
};

//Animations targeted at skeletal rigs
class pSkeletalAnimationClip : public pAnimationClip {
	SkeletalAnimationSample* _animationSamples; //Skeletal animation frames
	Skeleton* _skeletaon; //Skeleton this animation clip is intended for
};


//Animations targeted at transforming nodes
class pBasicAnimationClip : public pAnimationClip {
	//TODO: Non-skeletal animation
};

class pVertexAnimationClip : public pAnimationClip {
	//TODO: Mesh-vertex animation
};