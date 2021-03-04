#include "Shapes.h"

template<class T>
Utils::Shapes::Edge<T>::Edge() : a(), b() { }

template<class T>
Utils::Shapes::Edge<T>::Edge(const T& a, const T& b) : a(a), b(b) { }
template<class T>
T Utils::Shapes::Edge<T>::getEdge() const
{
	return b - a;
}
template<class T>
void Utils::Shapes::Edge<T>::set(const T& a)
{
	this->a = a;
}
template<class T>
void Utils::Shapes::Edge<T>::set(const T& a, const T& b)
{
	this->a = a;
	this->b = b;
}
template<class T>
T& Utils::Shapes::Edge<T>::operator[](const char& index)
{
	char i = index % 2;
	if (i)
		return a;
	else
		return b;
}

template<class T>
bool Utils::Shapes::Edge<T>::operator==(const Edge<T>& b) const
{
	return this->a == b[1] && this->b == b[0];
}

template<class T>
bool Utils::Shapes::Edge<T>::operator!=(const Edge<T>& b) const
{
	return !(this == b);
}

template<class T>
bool Utils::Shapes::Edge<T>::operator==(const T& b) const
{
	return getEdge() == b;
}

template<class T>
bool Utils::Shapes::Edge<T>::operator!=(const T& b) const
{
	return !(this == b);
}

template<class T>
bool Utils::Shapes::operator==(const T& a, const Edge<T>& b)
{
	return b == a;
}

template<class T>
bool Utils::Shapes::operator!=(const T& a, const Edge<T>& b)
{
	return !(a == b);
}

Utils::Shapes::Frustrum::Frustrum() : planes()
{
}

void Utils::Shapes::Frustrum::init(float fov, float near, float far, glm::vec3 pos, glm::vec3 fwd, glm::vec3 up, glm::vec3 right, glm::vec2 dimetions, bool normalised)
{
	if (!normalised) {
		fwd = glm::normalize(fwd);
		up = glm::normalize(up);
		right = glm::normalize(right);
	}

	const float tang = glm::tan(glm::radians(fov * 0.5f));
	const float ratio = dimetions.x / dimetions.y;

	const float nearHeight = near * tang;
	const float nearWidth = nearHeight * ratio;

	const float farHeight = far * tang;
	const float farWidth = farHeight * ratio;

	const glm::vec3 nearCenter = pos + fwd * near;
	const glm::vec3 farCenter = pos + fwd * far;

	PlaneAlternate nearPlane, farPlane, topPlane, bottomPlane, rightPlane, leftPlane;
	// normals
	nearPlane.normal   = fwd;
	farPlane.normal    = -fwd;
	topPlane.normal    = glm::normalize((nearCenter + up * nearHeight) - pos) * -right;
	bottomPlane.normal = glm::normalize((nearCenter - up * nearHeight) - pos) * -right;
	leftPlane.normal   = glm::normalize((nearCenter + right * nearWidth) - pos) * up;
	rightPlane.normal  = glm::normalize((nearCenter - right * nearWidth) - pos) * up;
	// points
	nearPlane.point    = nearCenter;
	farPlane.point     = farCenter;
	topPlane.point     = nearCenter + up * nearHeight;
	bottomPlane.point  = nearCenter - up * nearHeight;
	leftPlane.point    = nearCenter + right * nearWidth;
	rightPlane.point   = nearCenter - right * nearWidth;

	/*
	aux = (nc + fwd.y * height) - p;
	aux.normalize();
	normal = aux * fwd.x;
	pl[TOP].setNormalAndPoint(normal,nc+fwd.y*nh);
	*/
}

bool Utils::Shapes::Frustrum::shouldDraw(const std::array<glm::vec3, 8>& aabb) const
{
	for (char i = 0; i < 8; i++) {
		for (char j = 0; j < 6; j++) {
			const PlaneAlternate& p = planes[j];
			if (p.distance(aabb[i]) < 0) {
				return false;
			}
		}
	}
	return true;
}

bool Utils::Shapes::Frustrum::shouldDraw(const std::vector<glm::vec3>& aabb) const
{
	for (char i = 0; i < aabb.size(); i++) {
		for (char j = 0; j < 6; j++) {
			const PlaneAlternate& p = planes[j];
			if (p.distance(aabb[i]) < 0) {
				return false;
			}
		}
	}
	return true;
}

Utils::Shapes::PlaneAlternate::PlaneAlternate() : normal(0), point(0)
{
}

Utils::Shapes::PlaneAlternate::PlaneAlternate(const glm::vec3& n, const glm::vec3& p) : normal(n), point(p)
{
}

float Utils::Shapes::PlaneAlternate::distance(glm::vec3 a) const
{
	return glm::dot(normal, a - point);
}
