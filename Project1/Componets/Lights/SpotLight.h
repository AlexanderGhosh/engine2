#pragma once
#include "LightBase.h"

namespace Component {
	class SpotLight : public LightBase
	{
	private:
		glm::vec3 direction;
		float cutOff, outterCutOff, constant, linear, quadratic;
	public:
		SpotLight();
		SpotLight(Vector3 dir, Float cutOff, Float outterCutOff, Float constant, Float linear, Float quadratic);
		~SpotLight() = default;
		LightType getLightType() const;
		void cleanUp();

		// setters
		void setDirection   (Vector3 dir);
		void setCutOff      (Float cutOff);
		void setOutterCutOff(Float outterCutOff);
		void setConstant    (Float constant);
		void setLinear      (Float linear);
		void setQuadratic   (Float quadratic);

		// getters
		Vector3 getDirection   () const;
		Float   getCutOff      () const;
		Float   getOutterCutOff() const;
		Float   getConstant    () const;
		Float   getLinear      () const;
		Float   getQuadratic   () const;
	};		   
}

