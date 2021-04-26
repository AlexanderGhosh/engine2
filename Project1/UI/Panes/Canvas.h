#pragma once
#include <vector>
#include "../Elements/Element.h"
#include "../../Componets/ComponetBase.h"

namespace UI {
	class Canvas : public Component::ComponetBase
	{
	protected:
		std::vector<Element*> elements;
		int width, height;
	public:
		Canvas(unsigned width = 0, unsigned height = 0);
		~Canvas() = default;
		virtual void addElement(Element* element);
		virtual void addElement(Element& element);

		virtual void render(float deltaTime);
		const std::vector<Element*>& getElements() const;
		const glm::ivec2 getDimentions() const;
		inline Component::Type getType() const { return Component::Type::Canvas; };
		virtual void cleanUp();
		/// <summary>
		/// Returns the first instance of type T
		/// </summary>
		/// <typeparam name="T"></typeparam>
		/// <returns></returns>
		template<class T>
		inline T* getElement() {
			T* res = nullptr;
			for (const UI::Element* e : elements) {
				res = dynamic_cast<T*>(e);
				if (res)
					return res;
			}
			return nullptr;
		}
	};
};

