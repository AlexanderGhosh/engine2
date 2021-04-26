#pragma once
#include "../ComponetBase.h"
namespace Component {
	enum class LightType {
		Directional, Point, Spot
	};
	class LightBase : public ComponetBase
	{
	public:
		LightBase();
		LightBase(Vector3 colour, Float brightness = 1);
		~LightBase() = default;
		Type getType() const { return Type::Light; };
		virtual LightType getLightType() const = 0;
		void setParent(GameObject* parent);
		void cleanUp();
		// getters
		Vector3 getPosition() const;
		Vector3 getColour() const;
		Float getBrightness() const;
	protected:
		glm::vec3* position;
		glm::vec3 colour;
		float brightness;
	};
};

