#pragma once
#include "../../Utils/General.h"

namespace Materials {
	template<class T>
	class MatItemBase {
	protected:
		float repeatValue;
	public:
		MatItemBase(Float rv = 1.0f);
		~MatItemBase() = default;

		// getters
		virtual const T& getCurrentRaw() const = 0;
		virtual Unsigned getCurrentTexId() const = 0;
		virtual Float getCurrentMixValue() const = 0;
		Float getRepeatValue() const;

		// setters
		virtual void setRepeatValue(Float val);

		// adders
		virtual void addValue(const T& raw) = 0;
		virtual void addValue(Unsigned id) = 0;

		/// <summary>
		/// binds the texture if it is used and progeresses the unit other wise it doesnt move the unit along
		/// </summary>
		virtual bool tryBindTexture(unsigned& unit) const = 0;
		virtual bool tryBindTexture(Unsigned unit = 0) const = 0;

		virtual void update(float deltaTime) = 0;
		virtual void cleanUp() = 0;
	};

#pragma region Implementation
	template<class T>
	inline MatItemBase<T>::MatItemBase(Float rv) : repeatValue(rv)
	{
	}
	template<class T>
	inline Float MatItemBase<T>::getRepeatValue() const
	{
		return repeatValue;
	}
	template<class T>
	inline void MatItemBase<T>::setRepeatValue(Float val)
	{
		repeatValue = val;
	}
#pragma endregion
};