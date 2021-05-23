#pragma once
#include "General.h""
namespace Utils {
	class Conversions
	{
	public:
		static int toInt(char* data);
		static short toShort(char* data);
		static char toChar(char* data);
		static byte toByte(char* data);

		static float toFloat(char* data);
		static double toDouble(char* data);

		/*template<class T>
		static T convert(char* data) {
			T res = default;
			std::memcpy(&res, data, sizeof(T));
			return res;
		};*/
	};
}

