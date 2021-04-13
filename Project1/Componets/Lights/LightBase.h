#pragma once
#include "../Componets.h"
namespace Component {
	enum class LightType {
		Directional, Point, Spot
	};
	class LightBase : public ComponetBase
	{
	public:
		LightBase();
		~LightBase() = default;
		Type getType() const { return Type::Light; };
		virtual LightType getLightType() const = 0;
		void setParent(GameObject* parent);
		virtual void cleanUp() = 0;
	protected:
		glm::vec3* position;
		glm::vec3 colour;
	};
};

