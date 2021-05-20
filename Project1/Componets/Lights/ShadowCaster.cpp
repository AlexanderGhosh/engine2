#include "ShadowCaster.h"
#include "../../GameObject/GameObject.h"
#include "../../Scene/GameScene.h"
#include "../../Context.h"
#include "../Camera.h"
#include "DirectionalLight.h"

Component::ShadowCaster::ShadowCaster() : ComponetBase(), lightMatrixInfo(100.0f), camera(nullptr)
{
}

Component::ShadowCaster::ShadowCaster(DirectionalLight* light, Float shadowDistance) : ShadowCaster()
{
	if (NOT light)
		return;
	lightMatrixInfo.DIRECTION = -light->getDirection();
	lightMatrixInfo.SHADOW_DISTANCE = shadowDistance;
}

void Component::ShadowCaster::cleanUp()
{
	camera = nullptr;
	lightMatrixInfo.cleanUp();
}

void Component::ShadowCaster::update(float deltaTime)
{
	if (NOT camera)
		return;
	auto t = parent->getScene()->getActiveContext();
	lightMatrixInfo.update(camera, t->getAspectRatio());
}

Byte_Array Component::ShadowCaster::getByteField() const
{
	return Byte_Array();
}

glm::mat4 Component::ShadowCaster::getLSMatrix() const
{
	return lightMatrixInfo.getMatrix();
}

Vector3 Component::ShadowCaster::getPosition() const
{
	return parent->getLocalTransform()->Position;
}

void Component::ShadowCaster::setLightSource(DirectionalLight* light)
{
	if (NOT light)
		return;
	lightMatrixInfo.DIRECTION = -light->getDirection();
}

void Component::ShadowCaster::setCamera(Camera* camera)
{
	this->camera = camera;
}

// Light Matrix class


Component::ShadowCaster::LightMatrix::LightMatrix() : width(0), height(0), length(0), center(0), farHeight(0), nearHeight(0), farWidth(0), nearWidth(0),
													SHADOW_DISTANCE(15), NEAR_PLANE(0.1), OFFSET(10), firstUpdate(true), DIRECTION(0)
{
}

Component::ShadowCaster::LightMatrix::LightMatrix(Float shadowDistance) : LightMatrix()
{
	SHADOW_DISTANCE = shadowDistance;
}

glm::vec3 Component::ShadowCaster::LightMatrix::toLightSpace(Vector3 sp, Vector3 dir, Float width) const
{
	glm::vec4 point = glm::vec4(sp + dir * width, 1.0f);
	point = getView() * point;
	return glm::vec3(point);
}

std::array<glm::vec3, 8> Component::ShadowCaster::LightMatrix::getVertices(Vector3 up, Vector3 right, Vector3 centerNear, Vector3 centerFar) const
{

	const glm::vec3 upVector = up;
	const glm::vec3 rightVector = right;
	const glm::vec3 downVector = -upVector;
	const glm::vec3 leftVector = -rightVector;

	const glm::vec3 farTop = centerFar + upVector * farHeight;
	const glm::vec3 farBottom = centerFar + downVector * farHeight;

	const glm::vec3 nearTop = centerNear + upVector * nearHeight;
	const glm::vec3 nearBottom = centerNear + downVector * nearHeight;

	std::array<glm::vec3, 8> points{};
	points[0] = toLightSpace(farTop, rightVector, farWidth);
	points[1] = toLightSpace(farTop, leftVector, farWidth);
	points[2] = toLightSpace(farBottom, rightVector, farWidth);
	points[3] = toLightSpace(farBottom, leftVector, farWidth);
	points[4] = toLightSpace(nearTop, rightVector, nearWidth);
	points[5] = toLightSpace(nearTop, leftVector, nearWidth);
	points[6] = toLightSpace(nearBottom, rightVector, nearWidth);
	points[7] = toLightSpace(nearBottom, leftVector, nearWidth);
	return points;
}

glm::mat4 Component::ShadowCaster::LightMatrix::getOrtho() const
{
	if (firstUpdate OR DIRECTION == glm::vec3(0))
		return glm::mat4(1);
	glm::mat4 res(1);
	res[0][0] =  2.0f / width;
	res[1][1] =  2.0f / height;
	res[2][2] = -2.0f / length;
	res[3][3] =  1.0f;
	return res;
}

glm::mat4 Component::ShadowCaster::LightMatrix::getView() const
{
	if (firstUpdate OR DIRECTION == glm::vec3(0))
		return glm::mat4(1);
	const glm::vec3 dir = glm::normalize(DIRECTION);
	glm::mat4 res(1);

	float pitch = acosf(glm::length(glm::vec2(dir.x, dir.z)));
	res = glm::rotate(res, pitch, Utils::xAxis());

	float yaw = DEGREES(atanf(dir.x / dir.z));
	yaw = dir.z > 0 ? yaw - 180 : yaw;
	res = glm::rotate(res, -RADIANS(yaw), Utils::yAxis());

	res = glm::translate(res, -center);
	return res;
}

glm::mat4 Component::ShadowCaster::LightMatrix::getMatrix() const
{
	if (firstUpdate OR DIRECTION == glm::vec3(0))
		return glm::mat4(1);
	return getOrtho() * getView();
}

void Component::ShadowCaster::LightMatrix::update(Camera* camera, float ar)
{
	if (firstUpdate) {
		const float t = tanf(RADIANS(camera->getFOV()));
		farWidth  = SHADOW_DISTANCE * t;
		nearWidth = NEAR_PLANE      * t;

		farHeight  = farWidth  / ar;
		nearHeight = nearWidth / ar;
	}

	const glm::vec3 fwd = camera->getForward();

	const glm::vec3 toFar = fwd * SHADOW_DISTANCE;

	const glm::vec3 toNear = fwd * NEAR_PLANE;

	const glm::vec3 centerNear = toNear + camera->getPosition();

	const glm::vec3 centerFar = toFar + camera->getPosition();

	std::array<glm::vec3, 8> points = getVertices(Utils::yAxis(), camera->getRight(), centerNear, centerFar);

	glm::vec3 min(0), max(0);

	bool first = true;
	for (Vector3 point : points) {
		if (first) {
			min = max = point;
			first = false;
			continue;
		}
		max = glm::max(max, point);
		min = glm::min(min, point);
	}
	max.z += OFFSET;

	width  = max.x - min.x;
	height = max.y - min.y;
	length = max.z - min.z;

	glm::vec4 cen((min + max) * 0.5f, 1.0f);
	glm::mat4 inv = glm::inverse(getView());
	center = glm::vec3(inv * cen);
	firstUpdate = false;
}

void Component::ShadowCaster::LightMatrix::cleanUp()
{
}
