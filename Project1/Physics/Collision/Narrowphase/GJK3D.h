#pragma once
#include "Base.h"
#include <list>
namespace Physics {
	class GJK3D : public Narrowphase
	{
	public:
		struct Triangle {
			SupportPoint a, b, c;
			glm::vec3 n;
			Triangle(const SupportPoint& a, const SupportPoint& b, const SupportPoint& c) : a(a), b(b), c(c) {
				n = glm::cross(b.v - a.v, c.v - a.v);
				if (n != Utils::zero()) {
					n = glm::normalize(n);
				}
			}
			bool operator ==(const Triangle& j) const {
				return a == j.a && b == j.b && c == j.c;
			}
			bool operator !=(const Triangle& j) const { return !operator==(j); };
			inline const std::vector<const SupportPoint*> all() const { return { &a, &b, &c }; };
		};

		struct Simplex {
			char size;
		private:
			std::array<SupportPoint, 4> values;
		public:
			Simplex() : values{ }, size(0) { };
			Simplex(const std::vector<SupportPoint>& inps) : Simplex()
			{
				for (const SupportPoint& point : inps) {
					//values[size++] = point;
					values[size] = point;
					size++;
				}
				/*
				std::vector<SupportPoint> a = inps;
				if (a.size() > 4)
					a.resize(4);
				size = a.size();
				for (char i = 0; i < size; i++)
					values[i] = a[i];
				*/
			}
			glm::vec3 operator[] (const int& index) {
				return at(index).v;
			}
			SupportPoint& at(const char& index) {
				unsigned i = index % size;
				if (i < 0)
					i = 4 - i;
				return values[i];
			}
			inline void push(SupportPoint p)
			{
				size = glm::min(static_cast<char>(size + 1), static_cast<char>(4));
				for (char i = size - 1; i > 0; i--)
					values[i] = values[i - 1];
				values[0] = p;
			};
			void clear() {
				size = 0;
			}
			void pop() { size--; };
			SupportPoint& a() { return values[0]; };
			SupportPoint& b() { return values[1]; };
			SupportPoint& c() { return values[2]; };
			SupportPoint& d() { return values[3]; };

			inline void set(const SupportPoint& a) { 
				size = 1; 
				values[0] = a; 
			};
			inline void set(const SupportPoint& a, const SupportPoint& b) {
				size = 2;
				values[0] = a;
				values[1] = b;
			};
			inline void set(const SupportPoint& a, const SupportPoint& b, const SupportPoint& c) {
				size = 3;
				values[0] = a;
				values[1] = b;
				values[2] = c;
			};
			inline void set(const SupportPoint& a, const SupportPoint& b, const SupportPoint& c, const SupportPoint& d) {
				size = 4;
				values[0] = a;
				values[1] = b;
				values[2] = c;
				values[3] = d;
			};
		};
		struct Edge
		{
			SupportPoint a, b;
			Edge() : a(), b() { };
			Edge(const SupportPoint& a, const SupportPoint& b) : a(a), b(b) { };
			bool operator == (const Edge& b) const {
				return a == b.a && this->b == b.b/* || this->b == b.a && a == b.b*/;
			}
			bool operator != (const Edge& b) const {
				return !this->operator==(b);
			}
		};
		const CollisionManfold getCollisionData(Collider* a, Collider* b);
	private:
		Triangle* getClosest(std::list<Triangle>& faces, float& dist);
		void EPA(CollisionManfold& info, Simplex& simplex);
	};
}

