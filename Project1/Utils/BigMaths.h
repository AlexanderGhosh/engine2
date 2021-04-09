#pragma once
#include "General.h"

#define Vector12 const BigMaths::vec12&
#define Vector6 const BigMaths::vec6&
#define Matrix12 const BigMaths::mat12&
#define Matrix6 const BigMaths::mat6&
namespace BigMaths {
    class vec6;
    class vec12;
    class mat6;
    class mat12;

    class vec6 {
    public:
        float v1, v2, v3, v4, v5, v6;
    public:
        vec6(float fill = 0);
        vec6(Vector3 a, Vector3 b);
        vec6(std::array<float, 6> a);
        vec6(std::vector<float> a);
        vec6(float v1, float v2, float v3 = 0, float v4 = 0, float v5 = 0, float v6 = 0);
        ~vec6();
        Int size() const;
        vec6 copy() const;
        std::array<glm::vec3, 2> split() const;
        glm::vec3 get(Int index) const;

        float& operator[](Int index);
        Float operator[](Int index) const;
        /// <summary>
        /// combines the vector into a f12
        /// </summary>
        vec12 operator()(Vector6 b) const;
        vec6 operator+(Vector6 b) const;
        vec6 operator-(Vector6 b) const;
        vec6 operator*(Vector6 a) const;

        vec6 operator*(Float a) const;
        vec6 operator/(Float a) const;
    };

    class vec12 {
    public:
        float v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12;
    public:
        vec12(float fill = 0);
        vec12(Vector3 a, Vector3 b, Vector3 c, Vector3 d);
        vec12(Vector6 a, Vector6 b);
        vec12(std::array<float, 12> a);
        vec12(std::vector<float> a);
        vec12(float v1, float v2, float v3 = 0, float v4 = 0, float v5 = 0, float v6 = 0, float v7 = 0, float v8 = 0, float v9 = 0, float v10 = 0, float v11 = 0, float v12 = 0);
        ~vec12();
        Int size() const;
        vec12 copy() const;
        std::array<BigMaths::vec6, 2> split() const;
        std::array<glm::vec3, 4> splitDeep() const;
        glm::vec3 get(Int index) const;
        void set(Int index, Vector3 value);

        float& operator[](Int index);
        Float operator[](Int index) const;
        vec12 operator+(Vector12 b) const;
        vec12 operator-(Vector12 b) const;
        vec12 operator*(Vector12 a) const;

        vec12 operator*(Float a) const;
        vec12 operator/(Float a) const;
    };
    /// <summary>
    /// column major notation
    /// </summary>
    class mat6 {
    public:
        vec6 v1, v2, v3, v4, v5, v6;
    public:
        mat6(float center = 1);
        mat6(Matrix3 a, Matrix3 b);
        ~mat6();
        Int length() const;

        vec6& operator[](Int index);
        Vector6 operator[](Int index) const;

        vec6 operator*(Vector6 a) const;
    };
    vec6 operator* (Vector6 a, Matrix6 b);

    /// <summary>
    /// column major notation
    /// </summary>
    class mat12 {
    public:
        vec12 v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12;
    public:
        mat12(float center = 1);
        /// <summary>
        /// fills the mat12 with the diagonals of the given matrices
        /// </summary>
        mat12(Matrix3 a, Matrix3 b, Matrix3 c, Matrix3 d);
        mat12(Matrix6 a, Matrix6 b);
        ~mat12();
        Int length() const;

        vec12& operator[](Int index);
        Vector12 operator[](Int index) const;

        vec12 operator* (Vector12 a) const;
    };
    vec12 operator* (Vector12 a, Matrix12 b);

    float dot(Vector6 a, Vector6 b);
    float dot(Vector12 a, Vector12 b);

    inline vec6 inverse(Vector6 a) { return {}; };
    inline vec12 inverse(Vector12 a) { return {}; };
    inline mat6 inverse(Matrix6 a) { return {}; };
    inline mat12 inverse(Matrix12 a) { return {}; };
};