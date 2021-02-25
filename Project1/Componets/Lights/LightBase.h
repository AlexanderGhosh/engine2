#pragma once
#include "../Componets.h"
namespace Component {
	enum class LightType {
		Directional, Point
	};
	class LightBase : public ComponetBase
	{
	public:
		virtual LightType getLightType() const = 0;
		Type getType() const { return Type::Light; };
		virtual ~LightBase() = default;
	protected:
		LightBase() { };
	};
};

