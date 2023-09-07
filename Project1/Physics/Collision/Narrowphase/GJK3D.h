#pragma once
#include "Base.h"
#include "../../../Utils/General.h"
#include <array>
#include <map>

namespace Physics {
	class Simplex {
	private:
		std::array<glm::vec3, 4> data_;
		std::array<glm::vec3, 4> directions_;
		unsigned int size_;

		void line(glm::vec3& dir);
		void triangle(glm::vec3& dir);
		bool prism(glm::vec3& dir);

		bool sameDirection(Vector3 a, Vector3 b) const;
	public:
		Simplex();
		Simplex(std::initializer_list<glm::vec3> list);

		void add(Vector3 v, Vector3 dir);
		/// <summary>
		/// reordes the array via indices and fixes the size
		/// </summary>
		/// <param name="order"></param>
		void reorder(std::initializer_list<int> order);

		bool evaluate(glm::vec3& dir);
		void reset();

		inline auto begin_data() const { return data_.begin(); };
		inline auto end_data() const { return data_.end(); };

		inline auto begin_dir() const { return directions_.begin(); };
		inline auto end_dir() const { return directions_.end(); };

		inline Vector3 operator[](int index) const { return data_[index]; };

		inline const auto raw() const { return data_; };
	};

	class GJK3D : public Narrowphase
	{
	public:
		const CollisionDetails getCollisionData(Collider* a, Collider* b);
	private:
		void EPA(Physics::CollisionDetails& manafold, const Physics::Simplex& prism) const;
		void calculateContactPoint(CollisionDetails& manafold, Vector3 v1, Vector3 v2, Vector3 v3, Vector3 dir1, Vector3 dir2, Vector3 dir3) const;
		std::pair<std::vector<glm::vec4>, int> getFaceNormals(const std::vector<glm::vec3>& polytope, const std::vector<int>& faces) const;
		const glm::vec3 support(const Physics::Collider* a, const Physics::Collider* b, Vector3 axis) const;
		Simplex simplex;
	};
}

