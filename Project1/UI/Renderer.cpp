#include "Renderer.h"
#include <gtc/matrix_transform.hpp>
#include "../Rendering/Shading/Manager.h"

glm::mat4 UI::Renderer::projectionMatrix = { };
unsigned UI::Renderer::shaderId = 0;
Primative::Buffers UI::Renderer::quadBuffer = { };
Primative::StaticBuffer UI::Renderer::uiBuffer = { };

void UI::Renderer::init(const unsigned& shaderId, const glm::vec2& screenDim)
{
	UI::Renderer::shaderId = shaderId; 
	UI::Renderer::projectionMatrix = glm::ortho(screenDim.x, 0.0f, screenDim.y, 0.0f, -1.0f, 1.0f);
	Primative::Mesh mesh;
	mesh.verts.push_back({ { 1, 1, 0 } });
	mesh.verts.push_back({ { -1, 1, 0 } });
	mesh.verts.push_back({ { 1, -1, 0 } });
	mesh.verts.push_back({ { -1, -1, 0 } });
	mesh.indices = {
		0, 1, 2,
		3, 2, 1
	};
	quadBuffer = Primative::Buffers(mesh, GL_TRIANGLES);

	uiBuffer.init(sizeof(glm::mat4), 1);
	uiBuffer.fill(0, sizeof(glm::mat4), glm::value_ptr(UI::Renderer::projectionMatrix));
}

void UI::Renderer::render(const UI::Pane* pane)
{
	Render::Shading::Manager::setActive(shaderId);
	quadBuffer.bind();
	for (const UI::Element* element : pane->getElements()) {
		const glm::mat4 model = element->getModel();
		Render::Shading::Manager::setValue("model", model);
		quadBuffer.draw();
	}
	quadBuffer.unBind();
}
