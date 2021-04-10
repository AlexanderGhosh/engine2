#include "UIRenderer.h"
#include <gtc/matrix_transform.hpp>
#include "../Rendering/Shading/Manager.h"
#include "Panes/Grid.h"
#include "../Primatives/Mesh.h"
#include "../Primatives/Vertex.h"
#include "../Primatives/Buffers/VertexBuffer.h"
#include "../Primatives/Buffers/UniformBuffer.h"

unsigned UI::UIRenderer::shaderId = 0;
Primative::Buffers::VertexBuffer UI::UIRenderer::quadBuffer = { };
Primative::Buffers::StaticBuffer UI::UIRenderer::uiBuffer = { };

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
	quadBuffer = Primative::Buffers::VertexBuffer(mesh);

	uiBuffer = Primative::Buffers::StaticBuffer("m4", 2);
	glm::mat4 proj = glm::ortho(0.0f, screenDim.x, 0.0f, screenDim.y);
	uiBuffer.fill(0, glm::value_ptr(proj));
}

void UI::UIRenderer::render(const UI::Canvas* pane) // draws the quads
{
	glDisable(GL_DEPTH_TEST);
	quadBuffer.bind();
	Render::Shading::Manager::setActive(shaderId);
	for (const UI::Element* element : pane->getElements()) {
		glm::mat4 model = element->getModel();
		model[3][1] = pane->getDimentions().y - model[3][1];
		Render::Shading::Manager::setValue("model", model); 

		const Materials::MatItemBase<glm::vec3>* bg = element->getBackgroundColor();
		unsigned unit = 0;
		bg->tryBindTexture(unit);
		Render::Shading::Manager::setValue("col", *bg, unit);
		/*if (NOT bg.hasTex()) {
			Render::Shading::Manager::setValue("col_vec", glm::vec4(bg.getRaw(), 1));
		}
		else {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, bg.getId());
			bool res = Render::Shading::Manager::setValue("col_vec", glm::vec4(bg.getRaw(), 0));
			res = Render::Shading::Manager::setValue("col_id", 0); // set to texture unit
		}*/

		quadBuffer.draw();
		element->drawContent();
	}
	quadBuffer.unBind(); 
	glEnable(GL_DEPTH_TEST);
}

void UI::UIRenderer::render(const Element* element)
{
	glDisable(GL_DEPTH_TEST);
	quadBuffer.bind();
	Render::Shading::Manager::setActive(shaderId);
	glm::mat4 model = element->getModel();
	// model[3][1] = pane->getDimentions().y - model[3][1];
	Render::Shading::Manager::setValue("model", model);

	const Materials::MatItemBase<glm::vec3>* bg = element->getBackgroundColor();
	unsigned unit = 0;
	bg->tryBindTexture(unit);
	Render::Shading::Manager::setValue("col", *bg, unit);
	/*if (!bg.hasTex()) {
		Render::Shading::Manager::setValue("col_vec", glm::vec4(bg.getRaw(), 1));
	}
	else {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, bg.getId());
		bool res = Render::Shading::Manager::setValue("col_vec", glm::vec4(bg.getRaw(), 0));
		res = Render::Shading::Manager::setValue("col_id", 0); // set to texture unit
	}*/

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
