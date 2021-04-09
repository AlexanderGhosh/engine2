#pragma once
#include "../../Utils/General.h"
namespace Materials {
	template<class T>
	class MatItem {
	private:
		T raw;
		unsigned textureId;
		bool usesTextureId;
	public:
		MatItem();
		MatItem(const T& raw);
		MatItem(Unsigned id);
		~MatItem() = default;

		// getters
		const T& getRaw() const;
		Unsigned getTexId() const;
		const bool usesTexture() const;

		// setters
		void setValue(const T& raw);
		void setValue(Unsigned id);

		void useTexture();
		void useRaw();
		/// <summary>
		/// binds the texture if it is used and progeresses the unit other wise it doesnt move the unit along
		/// </summary>
		bool bindTexture(unsigned& unit) const;
		bool bindTexture(Unsigned unit = 0) const;

		void cleanUp();
	};

#pragma region Implementation
	template<class T>
	inline MatItem<T>::MatItem() : raw(0), textureId(0), usesTextureId(false)
	{
	}

	template<class T>
	inline MatItem<T>::MatItem(const T& raw) : MatItem()
	{
		this->raw = raw;
		usesTextureId = false;
	}

	template<class T>
	inline MatItem<T>::MatItem(Unsigned id) : MatItem()
	{
		textureId = id;
		usesTextureId = true;
	}

	template<class T>
	inline const T& MatItem<T>::getRaw() const
	{
		return raw;
	}

	template<class T>
	inline Unsigned MatItem<T>::getTexId() const
	{
		return textureId;
	}

	template<class T>
	inline const bool MatItem<T>::usesTexture() const
	{
		return usesTextureId;
	}

	template<class T>
	inline void MatItem<T>::setValue(const T& raw)
	{
		this->raw = raw;
	}

	template<class T>
	inline void MatItem<T>::setValue(Unsigned id)
	{
		this->textureId = id;
	}

	template<class T>
	inline void MatItem<T>::useTexture()
	{
		usesTextureId = true;
	}

	template<class T>
	inline void MatItem<T>::useRaw()
	{
		usesTextureId = false;
	}

	template<class T>
	inline bool MatItem<T>::bindTexture(unsigned& unit) const
	{
		if (usesTextureId) {
			glActiveTexture(GL_TEXTURE0 + unit++);
			glBindTexture(GL_TEXTURE_2D, textureId);
			return true;
		}
		return false;
	}

	template<class T>
	inline bool MatItem<T>::bindTexture(Unsigned unit) const
	{
		if (usesTextureId) {
			glActiveTexture(GL_TEXTURE0 + unit);
			glBindTexture(GL_TEXTURE_2D, textureId);
			return true;
		}
		return false;
	}

	template<class T>
	inline void MatItem<T>::cleanUp()
	{
	}
#pragma endregion
}