#pragma once
#include <glm/glm.hpp>
#include <stack>
class pMatrixStack
{
private:
	std::stack<glm::mat4> modelStack;
	std::stack<glm::mat4> projectionStack;
	bool modelStackDirty = true;
	bool projectionStackDirty = true;
public:
	/**
	* This creates an empty matrix stack that contains one projection matrix that is initialized as a
	* identity matrix. It also contains a model stack with one identity matrix already on the stack.
	*/
	pMatrixStack();

	/*This is the destructor.*/
	~pMatrixStack();

	/*Sets the projection matrix to be orthographic.
	  Components: 
	  xMin is the minimum x coord
	  xMax is the max x coord
	  yMin is the minimum y coord
	  yMax is the max y coord
	  zMin is the minimum Z coord
	  zMax is the max z coord*/
	void orthographic(float xMin, float xMax, float yMin, float yMax, float zMin, float zMax);

	/*Sets the projection matrix to be a perspective matrix
	  focy is the FOV vertically in degrees
	  aspect is the ratio of the scene defined as width/height
	  nearZ is the near Z clip pane
	  farZ is the far Z clip pane*/
	void perspective(float fovy, float aspect, float nearZ, float farZ);

	/*Model-view matrix is made to look at a given position.
	  posn is the camera's position
	  at is the position the camera must look at
	  up is the camera's up vector*/
	void lookAt(const glm::vec3& posn, const glm::vec3& at, const glm::vec3& up);

	/*Same as previous lookAt function, except that it takes in
	  the coordinates of posn, at, and up instead of glm::vec3s
	*/
	void lookAt(float camX, float camY, float camZ, float atX, float atY, float atZ, float upX, float upY, float upZ);

	/*Returns a copy of the projection matrix*/
	const glm::mat4 getProjectionMatrix() const;

	/*Returns a copy of the model matrix*/
	const glm::mat4 getModelMatrix() const;

	/*Returns copy of the current model matrix.*/
	const float* getModelMatrixValue() const;

	/*Return copy of the current projection matrix.*/
	const float* getProjectionMatrixValue() const;

	/*Returns current copy of the projection matrix.*/
	void getProjectionMatrixValue(float *values) const;

	/*Returns a copy of the current model matrix.*/
	void getModelMatrixValue(float *values) const;

	/*Pushes copy of the current model matrix on the stack.
	  Places a clone of it at the top of the stack.*/
	void pushModelView();

	/*This pops the top-most matrix off the model matrix stack, discards it.*/
	void popModelView();

	/**
	* Pushes copy of the current model matrix onto the stack.
	  Places a clone of it at the top of the stack.
	*/
	void pushProjection();

	/*Pops the top-most matrix off the model matrix stack, discards it.*/
	void popProjection();

	/*Rotates current model matrix by provided quaternion.*/
	void rotateBy(const glm::quat &quat);

	/*Rotate current model matrix by an angle around a vector.
	 angle is the angle by which to rotate
	 vx is the x vector component
	 vy is the y vector component
	 vz is the z vector component
	*/
	void rotateBy(float angle, float vx, float vy, float vz);

	/*Scale current model matrix by the indicated amount.*/
	void scaleBy(float xFactor, float yFactor, float zFactor);

	/*Translate current model matrix by the indicated amount.*/
	void translateBy(float xAmt, float yAmt, float zAmt);

	/*Set projection matrix to be an identity matrix.*/
	void setProjectionToIdentity();

	/*Set modelview matrix to be an identity matrix.*/
	void setModelViewToIdentity();

	/*Determines if the model-view stack has been modified since the flag was last set.*/
	bool isModelViewDirty() const;

	/*Determines if the projectionstack has been modified since the flag was last set.*/
	bool isProjectionDirty() const;

	/*Clears the modification flag on the model-view matrix.*/
	void clearModelViewDirty();

	/*Clears the modification flag on the projection matrix.*/
	void clearProjectionDirty();
};

