#pragma once
#include "glm/vec3.hpp"

//Small header providing various mathematical interpolation functions
class Interpolation {
public:
	//Cubic Ease-In-Out Vector3 Interpolation
	static glm::vec3 easeInOut_Cubic(glm::vec3 a, glm::vec3 b, float time) {
		return glm::vec3(easeInOut_Cubic(a[0], b[0], time), easeInOut_Cubic(a[1], b[1], time), easeInOut_Cubic(a[2], b[2], time));

	}

	//Cubic Ease-In-Out float Interpolation
	static float easeInOut_Cubic(float a, float b, float time) {
		if (time < 1) { return (b * 0.5f * time * time * time + a); }
		time -= 2;
		return (b * 0.5f * (time * time * time + 2.0f) + a);
	}

	//Quadratic Ease-In-Out Vector3 Interpolation
	static glm::vec3 easeInOut_Quadratic(glm::vec3 a, glm::vec3 b, float time) {
		return glm::vec3(easeInOut_Quadratic(a[0], b[0], time), easeInOut_Quadratic(a[1], b[1], time), easeInOut_Quadratic(a[2], b[2], time));
	}

	//Quadratic Ease-In-Out float Interpolation
	static float easeInOut_Quadratic(float a, float b, float time) {
		if (time < 1.0f) { return (b * 0.5f * time * time + a); }
		time--;
		return (-b * 0.5f * (time * (time - 2.0f) - 1.0f) + a);

		return 0;
	}
};