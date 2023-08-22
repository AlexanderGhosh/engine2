#pragma once
#include "Base.h"
#include "../../../Utils/General.h"
#include <array>
#include <map>

namespace Physics {
	class Simplex;
	class GJK3D : public Narrowphase
	{
	public:
		const CollisionManfold getCollisionData(Collider* a, Collider* b);
	private:
		void EPA(Physics::CollisionManfold& manafold, const Physics::Simplex& prism) const;
		std::pair<std::vector<glm::vec4>, int> getFaceNormals(const std::vector<glm::vec3>& polytope, const std::vector<int>& faces) const;
	};

	class Simplex {
	private:
		std::array<glm::vec3, 4> data_;
		unsigned int size_;

		static bool line(std::array<glm::vec3, 4>& points, glm::vec3& direction);
		static bool triangle(std::array<glm::vec3, 4>& points, glm::vec3& direction);
		static bool prism(std::array<glm::vec3, 4>& points, glm::vec3& direction);

		static bool sameDirection(Vector3 a, Vector3 b);
	public:
		Simplex();
		Simplex(std::initializer_list<glm::vec3> list);

		void add(Vector3 v);

		bool evaluate(glm::vec3& dir);

		inline auto begin() const { return data_.begin(); };
		inline auto end() const { return data_.end(); };

		inline Vector3 operator[](int index) const { return data_[index]; };
	};
}

