#pragma once
#include <glm.hpp>
#include <vector>
#include <string>
namespace Render {
	namespace Animation {
		class Bone {
		private:
			std::string name;
			glm::mat4 transformation;
		public:
			Bone();
			const std::string& getName() const;
			void setName(const std::string& name);
			const glm::mat4& getTransformation() const;
			void setTransformation(const glm::mat4& matrix);
		};

		class Skeleton {
		private:
			std::vector<Bone> bones;
		public:
			Skeleton();
			Bone& operator[] (const int& index);
			Bone& operator[] (const std::string& index);
			Skeleton operator +(const Skeleton& skel) const;
			void addBone(const Bone& bone);
			const std::vector<Bone>& getBones() const;

			bool hasBones() const;

			void cleanUp();
		};

		class BoneDetails {
		public:
			BoneDetails();
			/// <summary>
			/// index of the bone in the parent skeleton
			/// </summary>
			float boneIndex;
			float boneWeight;
			const Bone& getBone() const;
		};
	}
}

