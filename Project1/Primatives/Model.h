#pragma once
#include <string>
#include <vector>
#include "../Rendering/Animation/Bones.h"
#include "../Utils/General.h"
namespace Primative {
	class Model
	{
	private:
		std::string name;
		/// <summary>
		/// stores the index of the buffers in resourceloader.buffers
		/// </summary>
		std::vector<unsigned> buffers;
		Render::Animation::Skeleton skeleton;
	public:
		Model();
		const std::string& getName() const;
		void setName(const std::string& name);

		const Render::Animation::Skeleton& getSkeleton() const;
		void setSkeleton(const Render::Animation::Skeleton& skeleton);
		void cleanUp();
		
		void addBuffer(Unsigned bufferIndex);
		const std::vector<unsigned>& getBuffers() const;
		Unsigned getBuffer(String meshName) const;

		bool hasMesh() const;
	};
}

