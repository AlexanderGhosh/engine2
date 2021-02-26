#include "GizmoShapes.h"
#include "../Primatives/Buffers.h" // for the buffer functions
#include "../Rendering/Shading/Manager.h"
#include "../Utils/ResourceLoader.h"

Gizmos::Gizmo::Gizmo() : position(0), colour(1, 1, 0), thickness(1), pointVAO(0), shaderID(0)
{
	ResourceLoader::createShader("Resources/Gizmos/PointShader", true);
	ResourceLoader::createShader("Resources/Gizmos/LineShader", true);
	ResourceLoader::createShader("Resources/Gizmos/CubeShader");
	ResourceLoader::createShader("Resources/Gizmos/CircleShader", true);

	glm::vec3 data(0);
	unsigned vbo;
	glGenBuffers(1, &vbo);
	glGenVertexArrays(1, &pointVAO);
	glBindVertexArray(pointVAO);

	// VBO
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3), &data[0], GL_STATIC_DRAW);


	int stride = sizeof(glm::vec3);
	// layouts in shader
	// pos
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Gizmos::Gizmo::bindShader() const
{
	Render::Shading::Manager::setActive(shaderID);
}

void Gizmos::Gizmo::bindVAO() const
{
	glBindVertexArray(pointVAO);
}

void Gizmos::Gizmo::unbindVAO() const
{
	glBindVertexArray(0);
}

void Gizmos::Gizmo::drawGeneral()
{
	Render::Shading::Manager::setValue("position", position);
	Render::Shading::Manager::setValue("colour", colour);
	glPointSize(thickness); 
	glLineWidth(thickness);
	bindVAO();
	glDrawArrays(GL_POINTS, 0, 1);
	unbindVAO();
	glPointSize(1);
	glLineWidth(1);
}

void Gizmos::Gizmo::setThickness(Unsigned size)
{
	thickness = size;
}

Vector3 Gizmos::Gizmo::getPosition() const
{
	return position;
}

Vector3 Gizmos::Gizmo::getColour() const
{
	return colour;
}

void Gizmos::Gizmo::setPosition(Vector3 pos)
{
	this->position = pos;
}

void Gizmos::Gizmo::setColour(Vector3 colour)
{
	this->colour = colour;
}

Gizmos::Point::Point() : Gizmo()
{
	shaderID = ResourceLoader::getShader("PointShader");
}

Gizmos::Point::Point(Vector3 pos, Vector3 col) : Point() {
	this->position = pos;
	this->colour = col;
}

void Gizmos::Point::draw() {
	bindShader();
	Render::Shading::Manager::setValue("position", position);
	Render::Shading::Manager::setValue("colour", colour);
	drawGeneral();
}

void Gizmos::Point::cleanUp() {

}

Gizmos::Types Gizmos::Point::getType() const
{
	return Types::Point;
}

Gizmos::Line::Line() : Gizmo(), leftOffset(0), rightOffset(0)
{
	shaderID = ResourceLoader::getShader("LineShader");
}

Gizmos::Line::Line(Vector3 pos, Vector3 col) : Line()
{
	this->position = pos;
	this->colour = col;
}

void Gizmos::Line::draw() {
	bindShader();
	Render::Shading::Manager::setValue("left", leftOffset);
	Render::Shading::Manager::setValue("right", rightOffset);
	drawGeneral();
}

void Gizmos::Line::cleanUp() {

}

Gizmos::Types Gizmos::Line::getType() const
{
	return Types::Line;
}

void Gizmos::Line::setLeftOffset(Vector3 left)
{
	this->leftOffset = left;
}

void Gizmos::Line::setRightOffset(Vector3 right)
{
	this->rightOffset = right;
}

Gizmos::Circle::Circle() : Gizmo(), radius(1), up(Utils::yAxis())
{
	shaderID = ResourceLoader::getShader("CircleShader");
}

Gizmos::Circle::Circle(Vector3 pos, Vector3 col) : Circle()
{
	this->position = pos;
	this->colour = col;
}

void Gizmos::Circle::draw() {
	bindShader();
	const float angle = glm::degrees(acosf(glm::dot(Utils::yAxis(), glm::normalize(up))));
	const glm::vec3 axis = Utils::normalize(glm::cross(Utils::yAxis(), up));
	glm::mat4 rot = Utils::rotate(glm::mat4(1), angle * axis);
	Render::Shading::Manager::setValue("rotation", rot);
	Render::Shading::Manager::setValue("radius", radius);
	drawGeneral();
}

void Gizmos::Circle::cleanUp() {

}

Gizmos::Types Gizmos::Circle::getType() const
{
	return Types::Circle;
}

void Gizmos::Circle::setRadius(Float r)
{
	this->radius = r;
}

void Gizmos::Circle::setUp(Vector3 up)
{
	this->up = up;
}

Gizmos::Cuboide::Cuboide() : Gizmo(), dimensions(1), cubeModel()
{
	shaderID = ResourceLoader::getShader("CubeShader");
	cubeModel = ResourceLoader::getModel("cube.obj");
}

Gizmos::Cuboide::Cuboide(Vector3 pos, Vector3 col) : Cuboide()
{
	this->position = pos;
	this->colour = col;
}

void Gizmos::Cuboide::draw() {
	bindShader();
	glm::mat4 model = glm::translate(glm::mat4(1), position);
	model = glm::scale(model, dimensions);
	bool t = Render::Shading::Manager::setValue("model", model);
	t = t AND Render::Shading::Manager::setValue("colour", colour);
	assert(t);
	glPointSize(thickness);
	glLineWidth(thickness);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDisable(GL_CULL_FACE);
	const auto& buffers = cubeModel.getBuffers();
	for (short i = 0; i < buffers.size(); i++) {
		Primative::VertexBuffer& buffer = ResourceLoader::getBuffer(buffers[i]);
		buffer.bind();
		buffer.draw();
		buffer.unBind();
	}
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_CULL_FACE);
	glPointSize(1);
	glLineWidth(1);
}

void Gizmos::Cuboide::cleanUp() {

}

Gizmos::Types Gizmos::Cuboide::getType() const
{
	return Types::Cuboide;
}

void Gizmos::Cuboide::setDimentions(Vector3 dim)
{
	this->dimensions = dim;
}

Gizmos::Sphere::Sphere() : Gizmo(), radius(1), cirlces(), updated(true)
{
	cirlces[1].setUp(Utils::xAxis());
	cirlces[2].setUp(Utils::zAxis());
}

Gizmos::Sphere::Sphere(Vector3 pos, Vector3 col) : Sphere()
{
	this->position = pos;
	this->colour = col;
}

void Gizmos::Sphere::draw()
{
	for (char i = 0; i < 3; i++) {
		if (updated) {
			cirlces[i].setColour(this->colour);
			cirlces[i].setPosition(this->position);
			cirlces[i].setRadius(this->radius);
		}
		cirlces[i].draw();
	}
}

void Gizmos::Sphere::cleanUp()
{
	for (char i = 0; i < 3; i++) {
		cirlces[i].cleanUp();
	}
}

Gizmos::Types Gizmos::Sphere::getType() const
{
	return Types::Sphere;
}

void Gizmos::Sphere::setRadius(Float r)
{
	updated = true;
	this->radius = r;
}

void Gizmos::Sphere::setPosition(Vector3 pos)
{
	updated = true;
	Gizmo::setPosition(pos);
}

void Gizmos::Sphere::setColour(Vector3 colour)
{
	updated = true;
	Gizmo::setColour(colour);
}
