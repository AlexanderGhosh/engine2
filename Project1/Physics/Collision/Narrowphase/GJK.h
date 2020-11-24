#pragma once
#include "Base.h"
namespace Physics {
	class GJK : public Narrowphase {
	public:
		const CollisionManfold getCollisionData(Collider* a, Collider* b);
	private:
		struct Edge
		{
			SupportPoint a, b;
			Edge() : a(), b() { };
			Edge(const SupportPoint& a, const SupportPoint& b) : a(a), b(b) { };
		};
		struct Triangle {
			SupportPoint a, b, c;
			glm::vec3 n;
			Triangle(const SupportPoint& a, const SupportPoint& b, const SupportPoint& c) : a(a), b(b), c(c) {
				n = glm::normalize(glm::cross((b.v - a.v), (c.v - a.v)));
			}
			inline const std::vector<const SupportPoint*> all() const { return { &a, &b, &c }; };
		};
		struct Simplex {
			char size;
		private:
			std::array<SupportPoint, 4> values;
		public:
			Simplex() : values{ }, size(0) { };
			Simplex(const std::vector<SupportPoint>& inps);
			SupportPoint& operator[] (const int& index) {
				unsigned i = index % size;
				if (i < 0) {
					i = 4 - i;
				}
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
		};
		void EPA(CollisionManfold& info, Simplex& simplex);
		void extrapolateContactInformation(const Triangle& triangle, CollisionManfold& info);
		bool gjk_simtest(const glm::vec3& a, const glm::vec3& b) {
			return glm::dot(a, b) > 0;
		}; 
		void addEdge(const SupportPoint& a, const SupportPoint& b, std::list<Edge>& lst_edges)
		{
			for (auto it = lst_edges.begin(); it != lst_edges.end(); it++) {
				if (it->a == b && it->b == a) {
					//opposite edge found, remove it and do not add new one
					lst_edges.erase(it);
					return;
				}
			}
			lst_edges.emplace_back(a, b);
		};
	};
};

