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