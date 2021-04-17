#pragma once
#include "MatItemBase.h"
#include <GL/glew.h>

namespace Materials {
	template<class T>
	class MatItemSingle : public MatItemBase<T> {
	private:
		T raw;
		unsigned textureId;
		float mixValue;
	public:
		MatItemSingle(bool def = true, Float repeatVal = 1.0f);
		MatItemSingle(const T& raw, Float repeatVal = 1.0f);
		MatItemSingle(Unsigned id, Float repeatVal = 1.0f);
		~MatItemSingle() = default;

		// getters
		const T& getCurrentRaw() const;
		Unsigned getCurrentTexId() const;
		Float getCurrentMixValue() const;

		// adders
		void addValue(const T& raw);
		void addValue(Unsigned id);

		// setters
		void setValue(const T& raw);
		void setValue(Unsigned id);
		void setMixValue(Float mixValue);

		void useTexture();
		void useRaw();
		
		const bool hasTexture() const;
		/// <summary>
		/// binds the texture if it is used and progeresses the unit other wise it doesnt move the unit along
		/// </summary>
		bool tryBindTexture(unsigned& unit) const;
		bool tryBindTexture(Unsigned unit = 0) const;

		void update(float deltaTime);
		void cleanUp();
	};

#pragma region Implementation
	template<class T>
	inline MatItemSingle<T>::MatItemSingle(bool def, Float repeatVal) : MatItemBase<T>(repeatVal), raw(), textureId(0), mixValue(0)
	{
	}

	template<class T>
	inline MatItemSingle<T>::MatItemSingle(const T& raw, Float repeatVal) : MatItemSingle(true, repeatVal)
	{
		this->raw = raw;
		useRaw();
	}

	template<class T>
	inline MatItemSingle<T>::MatItemSingle(Unsigned id, Float repeatVal) : MatItemSingle(true, repeatVal)
	{
		this->textureId = id;
		useTexture();
	}

	template<class T>
	inline const T& MatItemSingle<T>::getCurrentRaw() const
	{
		return raw;
	}

	template<class T>
	inline Unsigned MatItemSingle<T>::getCurrentTexId() const
	{
		return textureId;
	}

	template<class T>
	inline Float MatItemSingle<T>::getCurrentMixValue() const
	{
		return mixValue;
	}

	template<class T>
	inline void MatItemSingle<T>::addValue(const T& raw)
	{
		this->raw = raw;
	}

	template<class T>
	inline void MatItemSingle<T>::addValue(Unsigned id)
	{
		this->textureId = id;
	}

	template<class T>
	inline void MatItemSingle<T>::setValue(const T& raw)
	{
		this->raw = raw;
	}

	template<class T>
	inline void MatItemSingle<T>::setValue(Unsigned id)
	{
		this->textureId = id;
	}

	template<class T>
	inline void MatItemSingle<T>::setMixValue(Float mixValue)
	{
		this->mixValue = mixValue;
	}

	template<class T>
	inline void MatItemSingle<T>::useTexture()
	{
		mixValue = 1;
	}

	template<class T>
	inline void MatItemSingle<T>::useRaw()
	{
		mixValue = 0;
	}

	template<class T>
	inline const bool MatItemSingle<T>::hasTexture() const
	{
		return textureId;
	}

	template<class T>
	inline bool MatItemSingle<T>::tryBindTexture(unsigned& unit) const
	{
		if (hasTexture()) {
			glActiveTexture(GL_TEXTURE0 + unit++);
			glBindTexture(GL_TEXTURE_2D, textureId);
			return true;
		}
		return false;
	}

	template<class T>
	inline bool MatItemSingle<T>::tryBindTexture(Unsigned unit) const
	{
		if (hasTexture()) {
			glActiveTexture(GL_TEXTURE0 + unit);
			glBindTexture(GL_TEXTURE_2D, textureId);
			return true;
		}
		return false;
	}

	template<class T>
	inline void MatItemSingle<T>::update(float deltaTime)
	{
	}

	template<class T>
	inline void MatItemSingle<T>::cleanUp()
	{
	}
#pragma endregion
}