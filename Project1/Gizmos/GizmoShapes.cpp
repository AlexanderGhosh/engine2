#include "GizmoShapes.h"
#include "../Primatives/Buffers/VertexBuffer.h" // for the buffer functions
#include "../Rendering/Shading/Manager.h"
#include "../Utils/ResourceLoader.h"

Gizmos::Gizmo::Gizmo() : position(0), colour(1), thickness(1), pointVAO(0), shaderID(0)
{
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
	drawGeneral();
}

void Gizmos::Point::cleanUp() {

}

Gizmos::Types Gizmos::Point::getType() const
{
	return Types::Point;
}

Gizmos::Line::Line() : Gizmo(), p1_(0), p2_(0)
{
	shaderID = ResourceLoader::getShader("LineShader");
}

Gizmos::Line::Line(Vector3 pos, Vector3 col) : Line()
{
	this->position = pos;
	this->colour = col;
}

Gizmos::Line::Line(Vector3 from, Vector3 to, bool over) : Line()
{
	p1_ = from;
	p2_ = to;
	// const glm::vec3 half_d = (to - from) / 2.0f;
	// position = from + half_d;
	// p1_ = -half_d;
	// p2_ = half_d;
}

void Gizmos::Line::draw() {
	bindShader();
	Render::Shading::Manager::setValue("left", p1_);
	Render::Shading::Manager::setValue("right", p2_);
	drawGeneral();
}

void Gizmos::Line::cleanUp() {

}

Gizmos::Types Gizmos::Line::getType() const
{
	return Types::Line;
}

void Gizmos::Line::setStart(Vector3 left)
{
	this->p1_ = left;
}

void Gizmos::Line::setEnd(Vector3 right)
{
	this->p2_ = right;
}

void Gizmos::Line::setPosition(Vector3 pos)
{
	Gizmo::setPosition(pos);
	const glm::vec3 delta = pos - position;
	p1_ += delta;
	p2_ += delta;
}

Gizmos::Circle::Circle() : Gizmo(), radius(0.5), up(Utils::yAxis())
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

Gizmos::Cuboide::Cuboide() : Gizmo(), dimensions(0.5), cubeModel()
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
		Primative::Buffers::VertexBuffer& buffer = ResourceLoader::getBuffer(buffers[i]);
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

Gizmos::Triangle::Triangle() : Gizmo(), lines(), points()
{
}

Gizmos::Triangle::Triangle(Vector3 p1, Vector3 p2, Vector3 p3, bool drawPoints) : Triangle()
{
	lines[0] = Line(p1, p2, true);
	lines[1] = Line(p2, p3, true);
	lines[2] = Line(p3, p1, true);

	if (drawPoints) {
		points.push_back(Point(p1, colour));
		points.push_back(Point(p2, colour));
		points.push_back(Point(p3, colour));
		for (auto& p : points) {
			p.setThickness(1);
		}
	}
}

void Gizmos::Triangle::draw()
{
	for (auto& line : lines) {
		line.draw();
	}
	for (auto& point : points) {
		point.draw();
	}
}

void Gizmos::Triangle::cleanUp()
{
	for (auto& line : lines) {
		line.cleanUp();
	}
	for (auto& point : points) {
		point.cleanUp();
	}
}

Gizmos::Types Gizmos::Triangle::getType() const
{
	return Types::Triangle;
}

void Gizmos::Triangle::setPosition(Vector3 pos)
{
	const glm::vec3 delta = pos - position;
	for (auto& l : lines) {
		l.setPosition(l.getPosition() + delta);
	}
}

void Gizmos::Triangle::setColour(Vector3 colour)
{
	Gizmo::setColour(colour);
	for (auto& line : lines) {
		line.setColour(colour);
	}
	for (auto& point : points) {
		point.setColour(colour);
	}
}

Gizmos::Arrow::Arrow() : Gizmo(), lines()
{
}

Gizmos::Arrow::Arrow(Vector3 start, Vector3 end) : Arrow()
{
	lines[0] = Line(start, end, true);
	const float size = 0.2f;
	const glm::vec3 d = glm::normalize(end - start);

	glm::vec3 axis = Utils::zAxis(size);
	if (d.z != 0) {
		axis = Utils::xAxis(size);
	}

	const glm::vec3 s1 = end - size * d + Utils::zAxis(size);
	const glm::vec3 s2 = end - size * d - Utils::zAxis(size);

	lines[1] = Line(s1, end, true);
	lines[2] = Line(s2, end, true);
}

void Gizmos::Arrow::draw()
{
	for (auto& line : lines) {
		line.draw();
	}
}

void Gizmos::Arrow::cleanUp()
{
	for (auto& line : lines) {
		line.cleanUp();
	}
}

Gizmos::Types Gizmos::Arrow::getType() const
{
	return Types::Arrow;
}

void Gizmos::Arrow::setPosition(Vector3 pos)
{
	const glm::vec3 delta = pos - position;
	for (auto& l : lines) {
		l.setPosition(l.getPosition() + delta);
	}
}

void Gizmos::Arrow::setColour(Vector3 colour)
{
	Gizmo::setColour(colour);
	for (auto& line : lines) {
		line.setColour(colour);
	}
}
