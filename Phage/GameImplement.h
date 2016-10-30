#pragma once
class GameImplement
{
public:
	GameImplement(){}
	~GameImplement() {}

	virtual void onStart() = 0;

	virtual void onUpdate(GLdouble deltaTime) = 0;
	virtual void onPostUpdate() = 0;

	virtual void onPreRender() = 0;
	virtual void onRender() = 0;
	virtual void onPostRender() = 0;

	virtual void onEnd() = 0;
};

