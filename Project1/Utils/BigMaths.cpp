#include "BigMaths.h"

#pragma region vec6
BigMaths::vec6::vec6(float fill) : v1(fill), v2(fill), v3(fill), v4(fill), v5(fill), v6(fill)
{
}

BigMaths::vec6::vec6(Vector3 a, Vector3 b) : vec6()
{
	v1 = a.x;
	v2 = a.y;
	v3 = a.z;
	v4 = b.x;
	v5 = b.y;
	v6 = b.z;
}

BigMaths::vec6::vec6(std::array<float, 6> a) : vec6()
{
	v1 = a[0];
	v2 = a[1];
	v3 = a[2];
	v4 = a[3];
	v5 = a[4];
	v6 = a[5];
}

BigMaths::vec6::vec6(std::vector<float> a) : vec6()
{
	if (NOT a.size())
		return;
	v1 = a[0 % a.size()];
	v2 = a[1 % a.size()];
	v3 = a[2 % a.size()];
	v4 = a[3 % a.size()];
	v5 = a[4 % a.size()];
	v6 = a[5 % a.size()];
}

BigMaths::vec6::vec6(float v1, float v2, float v3, float v4, float v5, float v6) : vec6()
{
	this->v1 = v1;
	this->v2 = v2;
	this->v3 = v3;
	this->v4 = v4;
	this->v5 = v5;
	this->v6 = v6;
}

BigMaths::vec6::~vec6()
{
}

Int BigMaths::vec6::size() const
{
	return 6;
}

BigMaths::vec6 BigMaths::vec6::copy() const
{
	return vec6(v1, v2, v3, v4, v5, v6);
}

std::array<glm::vec3, 2> BigMaths::vec6::split() const
{
	return std::array<glm::vec3, 2> {
		glm::vec3(v1, v2, v3), glm::vec3(v4, v5, v6)
	};
}

glm::vec3 BigMaths::vec6::get(Int index) const
{
	if (index < 0 OR index > 1) {
		return glm::vec3(0);
	}
	return split()[index];
}

float& BigMaths::vec6::operator[](Int index)
{
	switch (index)
	{
	case 0:
		return v1;
	case 1:
		return v2;
	case 2:
		return v3;
	case 3:
		return v4;
	case 4:
		return v5;
	case 5:
		return v6;
	default:
		return v1;
	}
}

Float BigMaths::vec6::operator[](Int index) const
{
	switch (index)
	{
	case 0:
		return v1;
	case 1:
		return v2;
	case 2:
		return v3;
	case 3:
		return v4;
	case 4:
		return v5;
	case 5:
		return v6;
	default:
		return v1;
	}
}

BigMaths::vec12 BigMaths::vec6::operator()(Vector6 b) const
{
	return vec12(*this, b);
}

BigMaths::vec6 BigMaths::vec6::operator+(Vector6 b) const
{
	vec6 res = copy();
	for (char i = 0; i < size(); i++) {
		res[i] += b[i];
	}
	return res;
}

BigMaths::vec6 BigMaths::vec6::operator-(Vector6 b) const
{
	vec6 res = copy();
	for (char i = 0; i < size(); i++) {
		res[i] -= b[i];
	}
	return res;
}
BigMaths::vec6 BigMaths::vec6::operator*(Vector6 a) const
{
	vec6 res(0);
	for (char i = 0; i < a.size(); i++)
		res[i] = a[i] * operator[](i);
	return res;
}
BigMaths::vec6 BigMaths::vec6::operator*(Float a) const
{
	vec6 res = copy();
	for (char i = 0; i < size(); i++) {
		res[i] *= a;
	}
	return res;
}
BigMaths::vec6 BigMaths::vec6::operator/(Float a) const
{
	vec6 res = copy();
	for (char i = 0; i < size(); i++) {
		res[i] /= a;
	}
	return res;
}
#pragma endregion

#pragma region vec12
BigMaths::vec12::vec12(float fill) : v1(fill), v2(fill), v3(fill), v4(fill), v5(fill), v6(fill), v7(fill), v8(fill), v9(fill), v10(fill), v11(fill), v12(fill)
{
}

BigMaths::vec12::vec12(Vector3 a, Vector3 b, Vector3 c, Vector3 d) : vec12()
{
	v1 = a.x;
	v2 = a.y;
	v3 = a.z;
	v4 = b.x;
	v5 = b.y;
	v6 = b.z;
	v7 = c.x;
	v8 = c.y;
	v9 = c.z;
	v10 = d.x;
	v11 = d.y;
	v12 = d.z;
}

BigMaths::vec12::vec12(Vector6 a, Vector6 b)
{
	for (char i = 0; i < size(); i++) {
		if (i < 6)
			this->operator[](i) = a[i];
		else
			this->operator[](i) = b[i];
	}
}

BigMaths::vec12::vec12(std::array<float, 12> a) : vec12()
{
	v1 = a[0];
	v2 = a[1];
	v3 = a[2];
	v4 = a[3];
	v5 = a[4];
	v6 = a[5];
	v7 = a[6];
	v8 = a[7];
	v9 = a[8];
	v10 = a[9];
	v11 = a[10];
	v12 = a[11];
}

BigMaths::vec12::vec12(std::vector<float> a) : vec12()
{
	if (NOT a.size())
		return;
	v1 = a[0 % a.size()];
	v2 = a[1 % a.size()];
	v3 = a[2 % a.size()];
	v4 = a[3 % a.size()];
	v5 = a[4 % a.size()];
	v6 = a[5 % a.size()];
	v7 = a[6 % a.size()];
	v8 = a[7 % a.size()];
	v9 = a[8 % a.size()];
	v10 = a[9 % a.size()];
	v11 = a[10 % a.size()];
	v12 = a[11 % a.size()];
}

BigMaths::vec12::vec12(float v1, float v2, float v3, float v4, float v5, float v6, float v7, float v8, float v9, float v10, float v11, float v12) : vec12()
{
	this->v1 = v1;
	this->v2 = v2;
	this->v3 = v3;
	this->v4 = v4;
	this->v5 = v5;
	this->v6 = v6;
	this->v7 = v7;
	this->v8 = v8;
	this->v9 = v9;
	this->v10 = v10;
	this->v11 = v11;
	this->v12 = v12;
}

BigMaths::vec12::~vec12()
{
}

Int BigMaths::vec12::size() const
{
	return 12;
}

BigMaths::vec12 BigMaths::vec12::copy() const
{
	return vec12(v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12);
}

std::array<BigMaths::vec6, 2> BigMaths::vec12::split() const
{
	return std::array<BigMaths::vec6, 2 > {
		vec6(v1, v2, v3, v4, v5, v6), vec6(v7, v8, v9, v10, v11, v12)
	};
}

std::array<glm::vec3, 4> BigMaths::vec12::splitDeep() const
{
	const auto split = this->split();
	const auto split_1 = split[0].split();
	const auto split_2 = split[1].split();
	return std::array<glm::vec3, 4>{
		split_1[0], split_1[1], split_2[0], split_2[1]
	};
}

glm::vec3 BigMaths::vec12::get(Int index) const
{
	switch (index)
	{
	case 0:
		return glm::vec3(v1, v2, v3);
	case 1:
		return glm::vec3(v4, v5, v6);
	case 2:
		return glm::vec3(v7, v8, v9);
	case 3:
		return glm::vec3(v10, v11, v12);
	default:
		return glm::vec3(0);
	}
}

void BigMaths::vec12::set(Int index, Vector3 value)
{
	switch (index)
	{
	case 0:
		v1 = value.x;
		v2 = value.y;
		v3 = value.z;
		break;
	case 1:
		v4 = value.x;
		v5 = value.y;
		v6 = value.z;
		break;
	case 2:
		v7 = value.x;
		v8 = value.y;
		v9 = value.z;
		break;
	case 3:
		v10 = value.x;
		v11 = value.y;
		v12 = value.z;
		break;
	}
}

float& BigMaths::vec12::operator[](Int index)
{
	switch (index)
	{
	case 0:
		return v1;
	case 1:
		return v2;
	case 2:
		return v3;
	case 3:
		return v4;
	case 4:
		return v5;
	case 5:
		return v6;
	case 6:
		return v7;
	case 7:
		return v8;
	case 8:
		return v9;
	case 9:
		return v10;
	case 10:
		return v11;
	case 11:
		return v12;
	default:
		return v1;
	}
}

Float BigMaths::vec12::operator[](Int index) const
{
	switch (index)
	{
	case 0:
		return v1;
	case 1:
		return v2;
	case 2:
		return v3;
	case 3:
		return v4;
	case 4:
		return v5;
	case 5:
		return v6;
	case 6:
		return v7;
	case 7:
		return v8;
	case 8:
		return v9;
	case 9:
		return v10;
	case 10:
		return v11;
	case 11:
		return v12;
	default:
		return v1;
	}
}

BigMaths::vec12 BigMaths::vec12::operator+(Vector12 b) const
{
	vec12 res = copy();
	for (char i = 0; i < size(); i++) {
		res[i] += b[i];
	}
	return res;
}

BigMaths::vec12 BigMaths::vec12::operator-(Vector12 b) const
{
	vec12 res = copy();
	for (char i = 0; i < size(); i++) {
		res[i] += b[i];
	}
	return res;
}
BigMaths::vec12 BigMaths::vec12::operator*(Vector12 a) const
{
	vec12 res(0);
	for (char i = 0; i < a.size(); i++)
		res[i] = a[i] * operator[](i);
	return res;
}
BigMaths::vec12 BigMaths::vec12::operator*(Float a) const
{
	vec12 res = copy();
	for (char i = 0; i < size(); i++) {
		res[i] *= a;
	}
	return res;
}
BigMaths::vec12 BigMaths::vec12::operator/(Float a) const
{
	vec12 res = copy();
	for (char i = 0; i < size(); i++) {
		res[i] /= a;
	}
	return res;
}
#pragma endregion

#pragma region mat6
BigMaths::mat6::mat6(float center) : v1(0), v2(0), v3(0), v4(0), v5(0), v6(0)
{
	if (center == 0)
		return;
	v1[0] = center;
	v2[1] = center;
	v3[2] = center;
	v4[3] = center;
	v5[4] = center;
	v6[5] = center;
}

BigMaths::mat6::mat6(Matrix3 a, Matrix3 b) : mat6(0)
{
	v1[0] = a[0][0];
	v1[1] = a[0][1];
	v1[2] = a[0][2];

	v2[0] = a[1][0];
	v2[1] = a[1][1];
	v2[2] = a[1][2];

	v3[0] = a[2][0];
	v3[1] = a[2][1];
	v3[2] = a[2][2];

	v4[0] = b[0][0];
	v4[1] = b[0][1];
	v4[2] = b[0][2];

	v5[0] = b[1][0];
	v5[1] = b[1][1];
	v5[2] = b[1][2];

	v6[0] = b[2][0];
	v6[1] = b[2][1];
	v6[2] = b[2][2];
}

BigMaths::mat6::~mat6()
{
}

Int BigMaths::mat6::length() const
{
	return 6;
}

BigMaths::vec6& BigMaths::mat6::operator[](Int index)
{
	switch (index)
	{
	case 0:
		return v1;
	case 1:
		return v2;
	case 2:
		return v3;
	case 3:
		return v4;
	case 4:
		return v5;
	case 5:
		return v6;
	default:
		return v1;
	}
}

Vector6 BigMaths::mat6::operator[](Int index) const
{
	switch (index)
	{
	case 0:
		return v1;
	case 1:
		return v2;
	case 2:
		return v3;
	case 3:
		return v4;
	case 4:
		return v5;
	case 5:
		return v6;
	default:
		return v1;
	}
}

BigMaths::vec6 BigMaths::mat6::operator*(Vector6 a) const
{
	vec6 res(0);
	for (char c = 0; c < length(); c++) {
		for (char r = 0; r < length(); r++) {
			res[c] += operator[](r)[c] * a[r];
		}
	}
	return res;
}

BigMaths::vec6 BigMaths::operator*(Vector6 a, Matrix6 b)
{
	vec6 res(0);
	for (char c = 0; c < b.length(); c++) {
		for (char r = 0; r < b.length(); r++) {
			res[c] += b[c][r] * a[r];
		}
	}
	return res;
}

#pragma endregion

#pragma region mat12

BigMaths::mat12::mat12(float center) : v1(0), v2(0), v3(0), v4(0), v5(0), v6(0), v7(0), v8(0), v9(0), v10(0), v11(0), v12(0)
{
	v1[0]   = center;
	v2[1]   = center;
	v3[2]   = center;
	v4[3]   = center;
	v5[4]   = center;
	v6[5]   = center;
	v7[6]   = center;
	v8[7]   = center;
	v9[8]   = center;
	v10[9]  = center;
	v11[10] = center;
	v12[11] = center;
}

BigMaths::mat12::mat12(Matrix3 a, Matrix3 b, Matrix3 c, Matrix3 d) : mat12(0)
{
	// a
	v1[0] = a[0][0];
	v2[1] = a[1][1];
	v3[2] = a[2][2];

	// b
	v4[3] = b[0][0];
	v5[4] = b[1][1];
	v6[5] = b[2][2];

	// c
	v7[6] = c[0][0];
	v8[7] = c[1][1];
	v9[8] = c[2][2];

	// d
	v10[9]  = d[0][0];
	v11[10] = d[1][1];
	v12[11] = d[2][2];
}

BigMaths::mat12::mat12(Matrix6 a, Matrix6 b) : mat12(0)
{
	for (char i = 0; i < a.length(); i++) {
		v1 [  i  ] = a.v1[i];
		v2 [  i  ] = a.v2[i];
		v3 [  i  ] = a.v3[i];
		v4 [  i  ] = a.v4[i];
		v5 [  i  ] = a.v5[i];
		v6 [  i  ] = a.v6[i];
		// b
		v7 [i + 6] = b.v1[i];
		v8 [i + 6] = b.v2[i];
		v9 [i + 6] = b.v3[i];
		v10[i + 6] = b.v4[i];
		v11[i + 6] = b.v5[i];
		v12[i + 6] = b.v6[i];
	}
}

BigMaths::mat12::~mat12()
{
}

Int BigMaths::mat12::length() const
{
	return 12;
}

BigMaths::vec12& BigMaths::mat12::operator[](Int index)
{
	switch (index)
	{
	case 0:
		return v1;
	case 1:
		return v2;
	case 2:
		return v3;
	case 3:
		return v4;
	case 4:
		return v5;
	case 5:
		return v6;
	case 6:
		return v7;
	case 7:
		return v8;
	case 8:
		return v9;
	case 9:
		return v10;
	case 10:
		return v11;
	case 11:
		return v12;
	default:
		return v1;
	}
}

Vector12 BigMaths::mat12::operator[](Int index) const
{
	switch (index)
	{
	case 0:
		return v1;
	case 1:
		return v2;
	case 2:
		return v3;
	case 3:
		return v4;
	case 4:
		return v5;
	case 5:
		return v6;
	case 6:
		return v7;
	case 7:
		return v8;
	case 8:
		return v9;
	case 9:
		return v10;
	case 10:
		return v11;
	case 11:
		return v12;
	default:
		return v1;
	}
}

BigMaths::vec12 BigMaths::mat12::operator*(Vector12 a) const
{
	vec12 res(0);
	for (char c = 0; c < length(); c++) {
		for (char r = 0; r < length(); r++) {
			res[c] += operator[](r)[c] * a[r];
		}
	}
	return res;
}

BigMaths::vec12 BigMaths::operator*(Vector12 a, Matrix12 b)
{
	vec12 res(0);
	for (char c = 0; c < b.length(); c++) {
		for (char r = 0; r < b.length(); r++) {
			res[c] += b[c][r] * a[r];
		}
	}
	return res;
}
#pragma endregion

float BigMaths::dot(Vector6 a, Vector6 b)
{
	float res = 0;
	for (char i = 0; i < a.size(); i++) {
		res += a[i] * b[i];
	}
	return res;
}

float BigMaths::dot(Vector12 a, Vector12 b)
{
	float res = 0;
	for (char i = 0; i < a.size(); i++) {
		res += a[i] * b[i];
	}
	return res;
}
