#pragma once
#include "MatItemBase.h"

namespace Materials {
	template<class T>
	class MatItemChain : public MatItemBase<T>
	{
	private:
		std::vector<T> rawValues;
		std::vector<unsigned> textureIds;
		std::vector<float> mixValues;
		std::vector<float> timeSeperations;
		float counter;
		int currentIndex;

		inline void addValue(const T& raw) { };
		inline void addValue(Unsigned id) { };
	public:
		MatItemChain();
		MatItemChain(const std::vector<T>& values, const std::vector<unsigned>& ids, const std::vector<float>& mixValues, const std::vector<float>& seperations);
		~MatItemChain() = default;
		const T& getCurrentRaw() const;
		Unsigned getCurrentTexId() const;
		Float getCurrentMixValue() const;

		/// <summary>
		/// binds the texture if it is used and progeresses the unit other wise it doesnt move the unit along
		/// </summary>
		bool tryBindTexture(unsigned& unit) const;
		bool tryBindTexture(Unsigned unit = 0) const;

		void update(float deltaTime);
		void cleanUp();
	};

	template<class T>
	inline MatItemChain<T>::MatItemChain() : MatItemBase<T>(), rawValues(), textureIds(), mixValues(), counter(0), currentIndex(0)
	{
	}

	template<class T>
	inline MatItemChain<T>::MatItemChain(const std::vector<T>& values, const std::vector<unsigned>& ids, 
		const std::vector<float>& mixValues, const std::vector<float>& seperations) : MatItemChain()
	{
		this->rawValues = values;
		this->textureIds = ids;
		this->mixValues = mixValues;
		this->timeSeperations = seperations;
	}

	template<class T>
	inline const T& MatItemChain<T>::getCurrentRaw() const
	{
		if (NOT rawValues.size())
			return rawValues[0];
		return rawValues[currentIndex];
	}

	template<class T>
	inline Unsigned MatItemChain<T>::getCurrentTexId() const
	{
		if (NOT rawValues.size())
			return 0;
		return textureIds[currentIndex];
	}

	template<class T>
	inline Float MatItemChain<T>::getCurrentMixValue() const
	{
		if (NOT rawValues.size())
			return 0;
		return mixValues[currentIndex];
	}

	template<class T>
	inline bool MatItemChain<T>::tryBindTexture(unsigned& unit) const
	{
		return false;
	}

	template<class T>
	inline bool MatItemChain<T>::tryBindTexture(Unsigned unit) const
	{
		return false;
	}

	template<class T>
	inline void MatItemChain<T>::update(float deltaTime)
	{
		counter += deltaTime;
		if (counter >= timeSeperations[currentIndex]) {
			currentIndex++;
			counter = 0;
			currentIndex %= rawValues.size();
		}
	}

	template<class T>
	inline void MatItemChain<T>::cleanUp()
	{
		rawValues.clear();
		textureIds.clear();
		mixValues.clear();
		timeSeperations.clear();
	}
}

