#pragma once
#include "../UIElementBase.h"

namespace GUI {
	class GUISlider : public GUIElementBase
	{
	private:
		GUIBaseShape sliderPointer, sliderBar;
		float sliderMin, sliderMax, sliderValue, increments, sliderMargin;
		glm::mat4 getPointerModel();
		glm::mat4 getSliderModel();
	public:
		GUISlider();
		GUISlider(Float sliderMin, Float sliderMax);
		~GUISlider() = default;

		void cleanUp();
		void render();

		// setters
		void setParent(GUIContainerBase* parent);
		void setSliderAlbedo(Materials::MatItemBase<glm::vec4>* albedo);
		void setPointerAlbedo(Materials::MatItemBase<glm::vec4>* albedo);
		void setSliderValue(Float value);

		// getters
		Float getSliderValue() const;
		Float getIncrement() const;
	};
};

