#include "UIRenderer.h"
#include <gtc/matrix_transform.hpp>
#include "../Rendering/Shading/Manager.h"
#include "Panes/Grid.h"
#include "../Primatives/Mesh.h"
#include "../Primatives/Vertex.h"
#include "../Primatives/buffers.h"

unsigned UI::UIRenderer::shaderId = 0;
Primative::VertexBuffer UI::UIRenderer::quadBuffer = { };
Primative::StaticBuffer UI::UIRenderer::uiBuffer = { };

void UI::UIRenderer::init(const unsigned& shaderId, const glm::vec2& screenDim)
{
	UI::UIRenderer::shaderId = shaderId; 
	Primative::Mesh mesh;
	mesh.verts.reserve(4);
	mesh.indices.reserve(6);

	mesh.verts.push_back(Primative::Vertex({  1,  1, 0 }, { 1, 1 }));
	mesh.verts.push_back(Primative::Vertex({ -1,  1, 0 }, { 0, 1 }));
	mesh.verts.push_back(Primative::Vertex({  1, -1, 0 }, { 1, 0 }));
	mesh.verts.push_back(Primative::Vertex({ -1, -1, 0 }, { 0, 0 }));
	mesh.indices = {
		0, 1, 2,
		3, 2, 1
	};
	quadBuffer = Primative::VertexBuffer(mesh);

	uiBuffer = Primative::StaticBuffer("m4", 1);
	// uiBuffer.init(sizeof(glm::mat4), 1);
	glm::mat4 proj = glm::ortho(0.0f, screenDim.x, 0.0f, screenDim.y);
	uiBuffer.fill(0, glm::value_ptr(proj));
}

void UI::UIRenderer::render(const UI::Pane* pane) // draws the quads
{
	glDisable(GL_DEPTH_TEST);
	quadBuffer.bind();
	for (const UI::Element* element : pane->getElements()) {
		Render::Shading::Manager::setActive(shaderId);
		glm::mat4 model = element->getModel();
		model[3][1] = pane->getDimentions().y - model[3][1];
		Render::Shading::Manager::setValue("model", model); 

		const Materials::MatItem& bg = element->getBackgroundColor();
		if (!bg.hasTex()) {
			Render::Shading::Manager::setValue("col_vec", glm::vec4(bg.getRaw(), 1));
		}
		else {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, bg.getId());
			bool res = Render::Shading::Manager::setValue("col_vec", glm::vec4(bg.getRaw(), 0));
			res = Render::Shading::Manager::setValue("col_id", 0); // set to texture unit
		}

		quadBuffer.draw();
		element->drawContent();
	}
	quadBuffer.unBind(); 
	glEnable(GL_DEPTH_TEST);
}

void UI::UIRenderer::render(const Page* pane)
{
	render(pane->getRoot());
}

void UI::UIRenderer::render(const Element* element)
{
	glDisable(GL_DEPTH_TEST);
	quadBuffer.bind();
	Render::Shading::Manager::setActive(shaderId);
	glm::mat4 model = element->getModel();
	// model[3][1] = pane->getDimentions().y - model[3][1];
	Render::Shading::Manager::setValue("model", model);

	const Materials::MatItem& bg = element->getBackgroundColor();
	if (!bg.hasTex()) {
		Render::Shading::Manager::setValue("col_vec", glm::vec4(bg.getRaw(), 1));
	}
	else {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, bg.getId());
		bool res = Render::Shading::Manager::setValue("col_vec", glm::vec4(bg.getRaw(), 0));
		res = Render::Shading::Manager::setValue("col_id", 0); // set to texture unit
	}

	quadBuffer.draw();
	element->drawContent();

	quadBuffer.unBind();
	glEnable(GL_DEPTH_TEST);
}

void UI::UIRenderer::cleanUp()
{
	uiBuffer.cleanUp();
	quadBuffer.cleanUp();
}
