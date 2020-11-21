#pragma once
#include "UI/Page.h"
#include "UI/Panes/Grid.h"
#include "UI/Elements/ImageBox.h"
#include "UI/Elements/ListBox.h"
#include "GameObject/GameObject.h"
class MainWindow : public UI::Page
{
public:
	MainWindow(const unsigned& viewPortTex, const float& aspectRatio);
private:
	const glm::vec3 bannerColor = { 0.34, 0, 0 };
	GameObject* selected;

	UI::ImageBox bannerBG_top;
	UI::ImageBox bannerBG_right;
	UI::ImageBox bannerBG_bottom;

	UI::ImageBox viewPort;

	UI::ListView objectView;

	void createBanners();
	void setUpViewPort(const unsigned& viewPortTex, const float& aspectRatio);
	void createObjectViewer();

	void updateObjectViewer();

	glm::ivec2 getViewPortShape(const float& aspectRatio, glm::ivec2 size);
};