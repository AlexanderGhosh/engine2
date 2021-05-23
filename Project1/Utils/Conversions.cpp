#include "Conversions.h"

int Utils::Conversions::toInt(char* data)
{
	int res = 0;
	std::memcpy(&res, data, sizeof(int));
	return res;
}

short Utils::Conversions::toShort(char* data)
{
	return 0;
}

char Utils::Conversions::toChar(char* data)
{
	return 0;
}

byte Utils::Conversions::toByte(char* data)
{
	return 0;
}

float Utils::Conversions::toFloat(char* data)
{
	return 0.0f;
}

double Utils::Conversions::toDouble(char* data)
{
	return 0.0;
}
