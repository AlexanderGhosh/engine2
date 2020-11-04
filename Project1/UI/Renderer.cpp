#include "Renderer.h"
#include <gtc/matrix_transform.hpp>
#include "../Rendering/Shading/Manager.h"

unsigned UI::Renderer::shaderId = 0;
Primative::Buffers UI::Renderer::quadBuffer = { };
Primative::StaticBuffer UI::Renderer::uiBuffer = { };

void UI::Renderer::init(const unsigned& shaderId, const glm::vec2& screenDim)
{
	UI::Renderer::shaderId = shaderId; 
	Primative::Mesh mesh;
	mesh.verts.push_back({ { 1, 1, 0 }, { 1, 1 } });
	mesh.verts.push_back({ { -1, 1, 0 }, { 0, 1 } });
	mesh.verts.push_back({ { 1, -1, 0 }, { 1, 0 } });
	mesh.verts.push_back({ { -1, -1, 0 }, { 0, 0 } });
	mesh.indices = {
		0, 1, 2,
		3, 2, 1
	};
	quadBuffer = Primative::Buffers(mesh, GL_TRIANGLES);

	uiBuffer.init(sizeof(glm::mat4), 1);
	glm::mat4 proj = glm::ortho(0.0f, screenDim.x, 0.0f, screenDim.y);
	uiBuffer.fill(0, sizeof(glm::mat4), glm::value_ptr(proj));
}

void UI::Renderer::render(const UI::Pane* pane) // draws the quads
{
	quadBuffer.bind();
	for (const UI::Element* element : pane->getElements()) {
		Render::Shading::Manager::setActive(shaderId);
		const glm::mat4 model = element->getModel();
		Render::Shading::Manager::setValue("model", model); 

		const Materials::MatItem& bg = element->getBackgroundColor();
		if (!bg.hasTex()) {
			Render::Shading::Manager::setValue("col_vec", glm::vec4(bg.getRaw(), 1));
		}
		else {
			glBindTexture(GL_TEXTURE_2D, bg.getId());
			bool res = Render::Shading::Manager::setValue("col_vec", glm::vec4(bg.getRaw(), 0));
			res = Render::Shading::Manager::setValue("col_id", 0); // set to texture unit
		}

		quadBuffer.draw();
		element->drawContent();
	}
	quadBuffer.unBind();
}
