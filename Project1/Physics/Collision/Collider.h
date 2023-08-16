#pragma once
#include <glm.hpp>
#include "../../Componets/ComponetBase.h"
namespace Physics {
	struct AABB;
	class Collider : public Component::ComponetBase
	{
	public:
		float mass;
		glm::mat3 localInertiaTensor;
		glm::vec3 localCentroid; // center of mass
		inline Collider* getCollider() { return dynamic_cast<Collider*>(this); };
		virtual const AABB* constructAABB() { return nullptr; };
		inline Component::Type getType() const { return Component::Type::Collider; };
		virtual glm::vec3 support(const glm::vec3& direction) const = 0;
		void setParent(GameObject* parent);
		void cleanUp();
		virtual ~Collider() = default;
		glm::vec3* position;
		glm::quat* rotation;
		glm::vec3* scale;
	protected:
		Collider(bool add = true);
		virtual void createInertiaTensor() { };
	};

	class AABB : public Collider {
	protected:
		glm::vec3 min, max;
	public:
		inline AABB(bool add = true) : Collider(add) {
			min = glm::vec3(-0.5f);
			max = glm::vec3(0.5f);
		}
		inline AABB(const glm::vec3& min = glm::vec3(-0.5), const glm::vec3& max = glm::vec3(0.5), bool add = true, GameObject* parent = nullptr) : Collider(add), min(min), max(max) { if (parent) setParent(parent); };
		inline AABB(const float& mass, const glm::vec3& centerOfMass, bool add = true) : AABB(add) { 
			this->mass = mass; 
			localCentroid = centerOfMass;
		};
		virtual ~AABB() = default;
		void setParent(GameObject* parent) {
			Collider::setParent(parent);
			createInertiaTensor();
		};
		const AABB* constructAABB() { return this; };
		virtual glm::vec3 support(const glm::vec3& direction) const;
		void createInertiaTensor();

		inline glm::vec3 getMin() const { return (min * *scale + *position) * 1.05f; };
		inline glm::vec3 getMax() const { return (max * *scale + *position) * 1.05f; };
		inline glm::vec3 getMinRaw() const { return min; };
		inline glm::vec3 getMaxRaw() const { return max; };


		inline void setMin(const glm::vec3& min) { this->min = min; };
		inline void setMax(const glm::vec3& max) { this->max = max; };
	};
	class BoxCollider : public AABB {
	private:
		char hi;
		AABB aabb;
	public:
		const AABB* constructAABB();
		glm::vec3 support(const glm::vec3& direction) const;
		inline BoxCollider(const float& mass, const glm::vec3& centerOfMass, bool add = true) : AABB(mass, centerOfMass, add), aabb(false), hi(0) { }; 
		virtual ~BoxCollider() = default;
		void update() {
			int i = 0;
		}
	};

	class ColliderSAT : public Collider {
	protected:
		std::vector<glm::vec3> vertices;
		std::vector<unsigned> indices;
		std::vector<glm::vec3> normals;
		glm::vec3 cachedNormal;
		char faceSize;
		ColliderSAT() : vertices(), indices(), normals(), cachedNormal(), faceSize(3) { };
	public:
		virtual ~ColliderSAT() = default;
		virtual void cleanUp() {
			vertices.clear();
			indices.clear();
			normals.clear();
			cachedNormal = Utils::zero();
			faceSize = 0;
			Collider::cleanUp();
		};
		inline glm::vec3 getVertex(const unsigned& index, const bool useIndices = true) const { return vertices[useIndices ? indices[index % indices.size()] : index % vertices.size()]; };
		inline glm::vec3 getNormal(unsigned index, const bool useIndices = true) const {
			return normals[useIndices ? floor(index / faceSize) : index % normals.size()];
		};
		virtual glm::vec3 getFarthest(const glm::vec3& dir, unsigned& indexRaw, bool translate) const {
			float proj = 0, max = -INFINITY;
			for (unsigned i = 0; i < vertices.size(); i++) {
				auto t = vertices[i];
				if (translate)
					t = t * *scale * *rotation + *position;
				proj = glm::dot(dir, t);
				if (proj > max) {
					max = proj;
					indexRaw = i;
				}
			}
			auto x = vertices[indexRaw];
			if (!translate)
				return x;
			else
				return x * *scale * *rotation + *position;
		}
		// returns the index(in indices) of the first occorance found 
		virtual unsigned rawToIndices(const unsigned& rawIndex) {
			const glm::vec3& v = vertices[rawIndex];
			for (unsigned i = 0; i < indices.size(); i++) {
				if (v == vertices[indices[i]]) {
					return i;
				}
			}
		}
		virtual std::list<glm::vec3> getAllNormals(const bool useCache, const bool removeParallel) {
			std::list<glm::vec3> norms;
			for (const glm::vec3& n0 : normals) {
				bool found = false;
				for (const glm::vec3& n1 : norms) {
					found = removeParallel && Utils::isParallel(n0, n1);
					if (found)
						break;
				}
				if (!found) {
					norms.push_back(glm::normalize(n0));
					if (useCache && norms.back() == cachedNormal)
						std::swap(norms.back(), norms.front());
				}
			}
			assert(norms.size() <= normals.size() && "added to the normals");
			return norms;
		}
		inline void setCache(const glm::vec3& toCache) { cachedNormal = toCache; };
		virtual std::array<float, 2> project(const glm::vec3& axis) const {
			std::array<float, 2> res = {
				INFINITY, -INFINITY
			};
			for (const glm::vec3& vertex : vertices) {
				float p = glm::dot((vertex * *scale) * glm::toMat3(*rotation) + *position, axis);
				res[0] = fminf(p, res[0]);
				res[1] = fmaxf(p, res[1]);
			}
			return res;
		};

		virtual std::list<glm::vec3> getAllFaces(const glm::vec3& vertex, bool localSpace = false, bool getIndices = false) const;
		virtual std::list<glm::vec3> getAllFaces(const unsigned& vertexIndex, bool localSpace = false, bool getIndices = false) const;

		virtual std::list<std::list<glm::vec3>> getAdjacentFaces(std::list<glm::vec3>& face) const;
	};
	class BoxColliderSAT : public ColliderSAT {
	private:
		AABB aabb;
	public:
		~BoxColliderSAT() = default;
		BoxColliderSAT(const float& mass) : ColliderSAT(), aabb(false) {
			this->mass = mass;
			faceSize = 4;
			vertices = {
				//  0          1
				{0.5f, 0.5f, 0.5f}, {-0.5f, -0.5f, -0.5f},
				//  2          3
				{-0.5f, 0.5f, 0.5f}, {0.5f, -0.5f, 0.5f},
				//  4          5
				{0.5f, 0.5f, -0.5f}, {-0.5f, -0.5f, 0.5f},
				//  6          7
				{0.5f, -0.5f, -0.5f}, {-0.5f, 0.5f, -0.5f}
			};
			indices = {
				1, 6, 4, 7, // orange (0 0 0) orange-blue-yellow
				7, 4, 0, 2, // green
				5, 1, 7, 2, // yellow
				3, 5, 2, 0, // red
				6, 3, 0, 4, // white
				5, 3, 6, 1  // blue
			};
			normals = {
				Utils::zAxis(),
				Utils::yAxis(),
				Utils::xAxis(-1),
				Utils::zAxis(-1),
				Utils::xAxis(), 
				Utils::yAxis(-1)
			}; 
		}
		glm::vec3 support(const glm::vec3& direction) const { return Utils::zero();  };
		void update() {
			int i = 0;
		}
		void setParent(GameObject* parent) {
			Collider::setParent(parent);
			const float m = mass * 0.5f;
			const float w = pow(scale->x, 2.0f);
			const float h = pow(scale->y, 2.0f);
			const float d = pow(scale->z, 2.0f);
			localInertiaTensor = {
				{ m * (h + d), 0, 0 },
				{ 0, m * (w + d), 0 },
				{ 0, 0, m * (w + h) }
			};
		};
		const AABB* constructAABB();
		std::list<glm::vec3> getAllNormals(const bool useCache, const bool removeParallel) {
			auto norms = ColliderSAT::getAllNormals(useCache, removeParallel);
			for (glm::vec3& axis : norms) {
				axis = axis * glm::toMat3(*rotation);
			}
			return norms;
		}
	};
};

