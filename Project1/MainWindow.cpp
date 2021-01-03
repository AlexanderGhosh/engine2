#include "MainWindow.h"
#include <iostream>

void bannerEnterCB(UI::Element* sender);
void bannerLeaveCB(UI::Element* sender);

MainWindow::MainWindow(const unsigned& viewPortTex, const float& aspectRatio) : selected(nullptr),
	bannerBG_top(), bannerBG_bottom(), bannerBG_right(), viewPort(), objectView()
{
	root = DBG_NEW UI::Grid(800, 600);
	createBanners();
	setUpViewPort(viewPortTex, aspectRatio);
	updateObjectViewer();
}
void MainWindow::createBanners()
{
	UI::Grid* r = reinterpret_cast<UI::Grid*>(root);
	r->setColumnCount(10);
	r->setRowCount(10);

	// TOP //
	bannerBG_top.setWidth(800);
	bannerBG_top.setHeight(200);
	bannerBG_top.setBackgroundColor(bannerColor);
	r->addElement(&bannerBG_top);
	r->setColumn(&bannerBG_top, 5);

	bannerBG_top.mouseEnter = bannerEnterCB;
	bannerBG_top.mouseLeave = bannerLeaveCB;

	// RIGHT //
	bannerBG_right.setWidth(200);
	bannerBG_right.setHeight(600);
	bannerBG_right.setBackgroundColor(bannerColor);
	r->addElement(&bannerBG_right);
	r->setColumn(&bannerBG_right, 10);
	r->setRow(&bannerBG_right, 5);

	bannerBG_right.mouseEnter = bannerEnterCB;
	bannerBG_right.mouseLeave = bannerLeaveCB;

	// BOTTOM //
	bannerBG_bottom.setWidth(800);
	bannerBG_bottom.setHeight(200);
	bannerBG_bottom.setBackgroundColor(bannerColor);
	r->addElement(&bannerBG_bottom);
	r->setColumn(&bannerBG_bottom, 5);
	r->setRow(&bannerBG_bottom, 10);

	bannerBG_bottom.mouseEnter = bannerEnterCB;
	bannerBG_bottom.mouseLeave = bannerLeaveCB;
}
void MainWindow::setUpViewPort(const unsigned& viewPortTex, const float& aspectRatio)
{
	UI::Grid* r = reinterpret_cast<UI::Grid*>(root);
	r->addElement(viewPort);
	r->setColumn(&viewPort, 4);
	r->setRow(&viewPort, 5);
	glm::ivec2 shape = getViewPortShape(aspectRatio, { 700, 400 });
	viewPort.setWidth(shape.x);
	viewPort.setHeight(shape.y);
	viewPort.setMargin({ 30 /*+ (700 - shape.x)*/, 0, 0, 0 });
	viewPort.setBackgroundImage(viewPortTex);
}

void MainWindow::createObjectViewer()
{

}

void MainWindow::updateObjectViewer()
{
	if (!selected)
		return;
}

glm::ivec2 MainWindow::getViewPortShape(const float& aspectRatio, glm::ivec2 size)
{
	size.x = size.y * aspectRatio;
	return size;
}

void bannerEnterCB(UI::Element* sender) {
	// sender->setBackgroundColor({ 0.1, 0.1, 1 });
}

void bannerLeaveCB(UI::Element* sender) {
	// sender->setBackgroundColor({ 1, 0.1, 1 });
}