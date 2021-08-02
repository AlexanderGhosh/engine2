#pragma once
#include "../Utils/General.h"

namespace EditorInteraction {
	enum class ResourceTypes {
		Model, Shader, Sound, Texture, Script, ERROR
	};
	class ResourceItem
	{
	private:
		ResourceTypes type;
		std::string name;
	public:
		ResourceItem();
		ResourceItem(String name, const ResourceTypes& type);
		~ResourceItem() = default;

		void cleanUp();

		// getters
		const ResourceTypes& getType() const;
		String getName() const;

		// setters
		void setType(const ResourceTypes& type);
		void setName(String name);
	};
}


