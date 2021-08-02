#include "GUISlider.h"
#include "../ElementContainers/GUIContainerBase.h"
#include "../GUIConstraint.h"
#include "../../EventSystem/Handler.h"

GUI::GUISlider::GUISlider() : sliderBar(), sliderMax(1), sliderMin(0), sliderPointer(), sliderValue(0), increments(), sliderMargin(50.0f)
{
	scroll = [](GUIElementBase* sender) {
		/*GUISlider* slider = dynamic_cast<GUISlider*>(sender);
		if (Utils::sign(Events::Handler::scrollAmmount.y) == 1) {
			slider->setSliderValue(slider->getSliderValue() + slider->getIncrement());
		}
		else {
			slider->setSliderValue(slider->getSliderValue() - slider->getIncrement());
		}*/
	};
}

GUI::GUISlider::GUISlider(Float sliderMin, Float sliderMax) : GUISlider()
{
	this->sliderMin = sliderMin;
	this->sliderMax = sliderMax;
	sliderValue = 0.5 * (sliderMax - sliderMin);
}

void GUI::GUISlider::cleanUp()
{
	GUIElementBase::cleanUp();
	sliderBar.cleanUp();
	sliderPointer.cleanUp();
}

void GUI::GUISlider::render()
{
	sliderPointer.render(getPointerModel());
	sliderBar.render(getSliderModel());
	GUIElementBase::render();
}

void GUI::GUISlider::setParent(GUIContainerBase* parent)
{
	GUIElementBase::setParent(parent);
	float x = dimentions.x - sliderMargin * 2.0f;
	increments = x / (sliderMax - sliderMin);
}

void GUI::GUISlider::setSliderAlbedo(Materials::MatItemBase<glm::vec4>* albedo)
{
	sliderBar.setAlbedo(albedo);
}

void GUI::GUISlider::setPointerAlbedo(Materials::MatItemBase<glm::vec4>* albedo)
{
	sliderPointer.setAlbedo(albedo);
}

void GUI::GUISlider::setSliderValue(Float value)
{
	sliderValue = value;
}

Float GUI::GUISlider::getSliderValue() const
{
	return sliderValue;
}

Float GUI::GUISlider::getIncrement() const
{
	return increments;
}

glm::mat4 GUI::GUISlider::getPointerModel()
{
	glm::vec2 pos = position;
	glm::vec2 dim = dimentions;

	dim.y *= 0.45f;
	pos.y -= (dimentions.y - dim.y) * 0.5f;

	pos.x += sliderMargin + increments * sliderValue;
	dim.x = dim.y * 0.5f;

	pos += (parent ? parent->getPosition() : glm::vec2(0));
	pos.y = GUIConstraint::getScreenDimentions().y - (pos.y + dimentions.y);

	glm::mat4 res(1);
	res = glm::translate(res, glm::vec3(pos, 0));
	res = glm::scale(res, glm::vec3(dim, 0));

	return res;
}

glm::mat4 GUI::GUISlider::getSliderModel()
{
	glm::vec2 pos = position;
	glm::vec2 dim = dimentions;

	dim.y *= 0.225f * 0.666666f;
	pos.y -= (dimentions.y - dim.y) * 0.5f;

	pos.x += sliderMargin;
	dim.x -= sliderMargin * 2.0f;

	pos += (parent ? parent->getPosition() : glm::vec2(0));
	pos.y = GUIConstraint::getScreenDimentions().y - (pos.y + dimentions.y);

	glm::mat4 res(1);
	res = glm::translate(res, glm::vec3(pos, 0));
	res = glm::scale(res, glm::vec3(dim, 0));

	return res;
}