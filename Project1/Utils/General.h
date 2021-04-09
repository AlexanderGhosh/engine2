#pragma once
#include <vector>
#include <array>
#include <string>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/quaternion.hpp>
#include <gtx/string_cast.hpp>
#include <list>
#include <iterator>
#include <chrono>
#include <iostream>
#include <iomanip>
#include <sstream>

#include "Shapes.h"



#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include <fstream>
#include <iostream>
#include <filesystem>

#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#else
#define DBG_NEW new
#endif


#define MAX_BONE_WEIGHTS 4
#define MAX_NUM_BONES 200
#define NOT !
#define AND &&
#define OR ||
#define EQ ==
#define NEQ !=
#define PI 3.14159265359f
constexpr float TWO_PI = 2.0f * PI;
constexpr float INV_PI = 1.0f / PI;
#define RADIANS(x) (x * PI * 0.00555555f)
#define DEGREES(x) (x * INV_PI * 180)

#define Vector3 const glm::vec3&
#define Vector4 const glm::vec4&
#define Quaternion const glm::quat&
#define Matrix3 const glm::mat3&
#define Matrix4 const glm::mat4&
#define String const std::string&
#define Float const float&
#define Unsigned const unsigned&
#define Int const int&

namespace Utils {

    std::vector<std::string> split(const std::string& str, const std::string& delim);

    glm::mat4 translate(glm::mat4 matrix, Vector3 vec);

    glm::mat4 rotate(Matrix4 matrix, Vector3 vec);

    glm::mat4 rotate(Matrix4 matrix, Quaternion vec);

    template<typename T>
    bool contains(const std::list<T>& a, const T& b) {
        auto found = std::find(a.begin(), a.end(), b);
        return found != a.end();
    }

    template<typename T>
    bool contains(const std::vector<T>& a, const T& b) {
        auto found = std::find(a.begin(), a.end(), b);
        return found != a.end();
    }

    bool contains(const std::string& a, const std::string& b);

    bool contained(std::array<glm::vec2, 2> bound, glm::vec2 pos);

    bool contained(std::array<glm::vec3, 2> bound, glm::vec3 pos);

    std::string toUTF8(unsigned int codepoint);

    glm::vec3 xAxis(float mag = 1);

    glm::vec3 yAxis(float mag = 1);

    glm::vec3 zAxis(float mag = 1);

    glm::vec3 zero();

    glm::vec3 fill(const float& num);

    glm::mat3 getSkewMatrix(Vector3 a);

    const std::string getFileName(const std::string& filePath, const bool& includeExtension = false);

    template< typename T>
    const unsigned& findIndex(std::vector<T>& a, T& b) {
        auto c = std::find(a.begin(), a.end(), b);
        return std::distance(a.begin(), c);
    }

    template< typename T>
    T& elementAt(std::list<T>& list, const int& index) {
        if (index > list.size())
            return *list.begin();
        auto s = list.begin();
        std::advance(s, index);
        return *s;
    }

    template< typename T>
    T& getElement(std::vector<T>& list, int index) {
        if (index < 0) {
            index = list.size() + index;
        }
        index = index % list.size();
        return list[index];
    }

    template< typename T, size_t size>
    T& getElement(std::array<T, size> list, int index) {
        if (index < 0) {
            index = size + index;
        }
        index = index % size;
        return list[index];
    }

    glm::vec3 map(const glm::vec3& x, const glm::vec2& from, const glm::vec2& to);

    template<typename T>
    bool find(const std::list<T>& a, T& b) {
        auto it = std::find(a.begin(), a.end(), b);
        bool res = it != a.end();
        if (res)
            b = *it;
        return res;
    }

    /// <summary>
    /// returns true if any are nan or inf regardles of sign
    /// </summary>
    /// <param name="a"></param>
    /// <returns></returns>
    bool nan_inf(const glm::mat3& a);

    glm::vec3 tripProduct(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c);

    // a x b x a
    glm::vec3 tripProduct(const glm::vec3& a, const glm::vec3& b);

    bool isParallel(const glm::vec3& a, const glm::vec3& b);

    bool isPerpendicular(const glm::vec3& a, glm::vec3& b);

    glm::vec3 linePlaneIntersection(const glm::vec3& rayVector, const glm::vec3& rayPoint, const glm::vec3& planeNormal, const glm::vec3& planePoint);

    float isInFront(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& planeNormal);

    glm::vec3 projectPointPlane(const glm::vec3& n, const glm::vec3& o, const glm::vec3& p);

    template<typename T>
    void removeAt(T& a, const unsigned& i) {
        auto t = a.begin();
        std::advance(t, i);
        a.erase(t);
    }

    glm::mat3 inverse(const glm::mat3& a, const bool symetrical = false);
    glm::vec3 round(glm::vec3 var, int dp);
    float round(float var, int dp);

    // doenst go deep
    int countFiles(String dirPath);

    glm::vec3 normalize(Vector3 a);

    /// <summary>
    /// returns the matrix but without nan or inf
    /// </summary>
    /// <param name="a"></param>
    /// <returns></returns>
    glm::mat4 validate(glm::mat4 a, float val = 0);

    /// <summary>
    /// replaces all occorances in the string
    /// </summary>
    std::string replaceAll(std::string str, std::string from, std::string to);

    template <typename BidirectionalIterator>
    void reverse(BidirectionalIterator first, BidirectionalIterator last)
    {
        while ((first != last) && (first != --last)) {
            std::iter_swap(first, last);
            ++first;
        }
    }

    void Log(String msg, String eol = "\n");
    void Log(std::vector<std::string> msg, String eol = "\n");

    /// <summary>
    /// returns vec as a string casted to T eg (int)
    /// </summary>
    /// <typeparam name="T"></typeparam>
    /// <param name="vec"></param>
    /// <returns></returns>
    template<class T>
    std::string to_string(Vector3 vec) {
        std::string res = "";
        for (char i = 0; i < 3; i++) {
            res += std::to_string(static_cast<T>(vec[i])) + ", ";
        }
        res.pop_back();
        res.pop_back();
        return res;
    }
    std::string to_string_precision(Vector3 vec, Int dp);
    std::string to_string_precision(Float vec, Int dp);

    float random(float min = 0, float max = 1);

    class Timer {
    public:
        Timer(const std::string name);
        Timer();
        void start();
        void start(String name);
        void stop();
        bool pause();
        bool resume();
        void log(bool frames = 0);
        const float& getDuration(bool frames = 0);
        void reName(String name);
    private:
        std::string name;
        static const std::chrono::time_point<std::chrono::high_resolution_clock> getNow();
        static float calcDurration(const std::chrono::time_point<std::chrono::high_resolution_clock>& s, const std::chrono::time_point<std::chrono::high_resolution_clock>& e);
        std::chrono::time_point<std::chrono::high_resolution_clock> s, e;
        std::list<std::chrono::time_point<std::chrono::high_resolution_clock>> pausing;
    };
}

