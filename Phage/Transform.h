#pragma once

#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/quaternion.hpp"

class Transform {
public:
	//Constructor: Vec3 Position, Quaternion Rotation, Vec3 Scale
	Transform(glm::vec3 position, glm::quat rotation = glm::quat(1, glm::vec3(1)), glm::vec3 scale = glm::vec3(1)) {
		setPosition(position);
		setRotation(rotation);
		setScale(scale);
	}

	//Constrcutor: Vec3 Position, Vec3 EulerAngles, Vec3 Scale
	Transform(glm::vec3 position, glm::vec3 eulerRotation = glm::vec3(0), glm::vec3 scale = glm::vec3(1)) {
		setPosition(position);
		setRotation(eulerRotation);
		setScale(scale);
	}

	//Constructor: Identity
	Transform() {
		_matPos = glm::mat4(1);
		_matRot = glm::mat4(1);
		_matScale = glm::mat4(1);

		_position = glm::vec3(0);
		_rotation = glm::quat(glm::vec3(0));
		_scale = glm::vec3(1);
	}

	~Transform() {

	}


	//Sets the object's position
	inline void setPosition(glm::vec3 pos) {
		//Set the position matrix
		_matPos = glm::translate(glm::mat4(1), pos);
		//Set the position vector
		_position = pos;
	}

	//Moves the object relative to current position
	inline void translate(glm::vec3 move) {
		//Translate the position matrix
		_matPos = glm::translate(_matPos, move);
		//Add to the position vector
		_position += move;
	}

	//Sets the object's rotation
	inline void setRotation(glm::vec3 eulerAngles) {
		eulerAngles = convertToRadians(eulerAngles);
		//Call the other setRotation with euler converted to quaternion
		setRotation(glm::quat(eulerAngles));
	}

	//Sets the object's rotation
	inline void setRotation(glm::quat quaternion) {
		//Convert quaternion to rotation matrix
		_matRot = glm::mat4_cast(quaternion);
		//Store the quaternion rotation
		_rotation = quaternion;
	}

	//Rotates the object relative to current
	inline void rotate(glm::vec3 eulerAngles) {
		eulerAngles = convertToRadians(eulerAngles);
		//Call the other rotate with euler converted to quaternion
		rotate(glm::quat(eulerAngles));
	}

	inline void Rotate(float x, float y, float z, bool localOrientation)
	{
		auto rotationMatrix = glm::rotate(x, glm::vec3(1, 0, 0));
		rotationMatrix *= glm::rotate(y, glm::vec3(0, 1, 0));
		rotationMatrix *= glm::rotate(z, glm::vec3(0, 0, 1));
		if (localOrientation)
			this->T = this->T * rotationMatrix;
		else
			this->T = rotationMatrix * this->T;
	}

	//Rotates the object relative to current
	inline void rotate(glm::quat quaternion) {
		//Convert the quaternion to a rotation matrix
		glm::mat4 rotMat = glm::mat4_cast(quaternion);
		//Rotate the local rotation matrix
		_matRot *= rotMat;
		//Update the quaternion rotation
		_rotation = glm::quat_cast(_matRot);
	}

	//Orients the object toward pos
	inline void lookAt(glm::vec3 pos) {
		_matRot = glm::lookAt(_position, pos, glm::vec3(0, 1, 0));
	}

	//Sets the scale value
	inline void setScale(glm::vec3 scl) {
		//Set the scale matrix
		_matScale = glm::scale(glm::mat4(1), scl);
		//Set the scale vector
		_scale = scl;
	}

	//Changes the scale value relative to current
	inline void scale(glm::vec3 scl) {
		//Scale the scale matrix
		_matScale = glm::scale(_matScale, scl);
		//Scale the scale vector
		_scale *= scl;
	}

	//Retreive the rotation as euler angles
	inline glm::vec3 getRotationEuler() {
		return glm::eulerAngles(_rotation);
	}

	//Retreive the rotation as quaternion
	inline glm::quat getRotation() {
		return _rotation;
	}
	//Retreive the position
	inline glm::vec3 getPosition() {
		return _position;
	}
	//Retreive the scale
	inline glm::vec3 getScale() {
		return _scale;
	}
	//Retrive the transfomation matrix
	inline glm::mat4 getTransMatrix() {
		return _matPos * _matRot * _matScale;
	}

	//Retrive the alternate transfomation matrix
	inline glm::mat4 getAltTransMatrix() {
		return _matRot * _matPos * _matScale;
	}

	inline glm::vec3 getForwardLocal() {
		return (_rotation * glm::vec3(0, 0, 1));
	}

	inline glm::vec3 getUpLocal() {
		return (_rotation * glm::vec3(0, 1, 0));
	}

	inline glm::vec3 getRightLocal() {
		return (_rotation * glm::vec3(1, 0, 0));
	}

	inline glm::vec3 getForwardWorld() {
		return _position + (_rotation * glm::vec3(0, 0, 1));
	}

	inline glm::vec3 getUpWorld() {
		return _position + (_rotation * glm::vec3(0, 1, 0));
	}

	inline glm::vec3 getRightWorld() {
		return _position + (_rotation * glm::vec3(1, 0, 0));
	}

	inline glm::vec3 convertToRadians(glm::vec3 degrees) {
		return glm::vec3(glm::radians(degrees.x), glm::radians(degrees.y), glm::radians(degrees.z));
	}

	inline float convertToRadians(float degrees) {
		return glm::radians(degrees);
	}

private:
	glm::vec3 _position;
	glm::quat _rotation;
	glm::vec3 _scale;

	glm::mat4 _matPos;
	glm::mat4 _matRot;
	glm::mat4 _matScale;
};
