#pragma once
#include <vector>
#include <array>
#include <string>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
namespace Utils {
    inline std::vector<std::string> split(const std::string& str, const std::string& delim)
    {
        std::vector<std::string> tokens;
        size_t prev = 0, pos = 0;
        do
        {
            pos = str.find(delim, prev);
            if (pos == std::string::npos) pos = str.length();
            std::string token = str.substr(prev, pos - prev);
            if (!token.empty()) tokens.push_back(token);
            prev = pos + delim.length();
        } while (pos < str.length() && prev < str.length());
        return tokens;
    };

    inline glm::mat4 rotate(glm::mat4 matrix, const glm::vec3& vec) {
        if (glm::all(glm::equal(vec, { 0, 0, 0 }))) return matrix;
        return glm::rotate(matrix, glm::radians(glm::length(vec)), glm::normalize(vec));
        /*for (short i = 0; i < 3; i++) {
            glm::vec3 d(0);
            d[i] = 1;
            matrix = glm::rotate(matrix, glm::radians(vec[0]), d);
        }
        return matrix;*/
    };
    template<class T>
    inline bool contains(const std::vector<T>& a, const T& b) {
        auto found = std::find(a.begin(), a.end(), b);
        return found != a.end();
    }
    inline bool contains(const std::string& a, const std::string& b) {
        const short s = b.size();
        for (short i = 0; i < a.size() - b.size(); i++) {
            const std::string& sub = a.substr(i, s);
            if (sub == b) {
                return true;
            }
        }
        return false;
    }

    inline bool contained(std::array<glm::vec2, 2> bound, glm::vec2 pos) {
        return glm::all(glm::greaterThanEqual(pos, bound[0]) && glm::lessThanEqual(pos, bound[1]));
    }
    inline bool contained(std::array<glm::vec3, 2> bound, glm::vec3 pos) {
        return glm::all(glm::greaterThanEqual(pos, bound[0]) && glm::lessThanEqual(pos, bound[1]));
    }

    inline std::string toUTF8(unsigned int codepoint)
    {
        std::string out;

        if (codepoint <= 0x7f)
            out.append(1, static_cast<char>(codepoint));
        else if (codepoint <= 0x7ff)
        {
            out.append(1, static_cast<char>(0xc0 | ((codepoint >> 6) & 0x1f)));
            out.append(1, static_cast<char>(0x80 | (codepoint & 0x3f)));
        }
        else if (codepoint <= 0xffff)
        {
            out.append(1, static_cast<char>(0xe0 | ((codepoint >> 12) & 0x0f)));
            out.append(1, static_cast<char>(0x80 | ((codepoint >> 6) & 0x3f)));
            out.append(1, static_cast<char>(0x80 | (codepoint & 0x3f)));
        }
        else
        {
            out.append(1, static_cast<char>(0xf0 | ((codepoint >> 18) & 0x07)));
            out.append(1, static_cast<char>(0x80 | ((codepoint >> 12) & 0x3f)));
            out.append(1, static_cast<char>(0x80 | ((codepoint >> 6) & 0x3f)));
            out.append(1, static_cast<char>(0x80 | (codepoint & 0x3f)));
        }
        return out;
    }
    inline glm::vec3 xAxis(float mag = 1) {
        return glm::vec3(1, 0, 0) * mag;
    }
    inline glm::vec3 yAxis(float mag = 1) {
        return glm::vec3(0, 1, 0) * mag;
    }
    inline glm::vec3 zAxis(float mag = 1) {
        return glm::vec3(0, 0, 1) * mag;
    }
    inline glm::vec3 zero() {
        return glm::vec3(0);
    }
    inline glm::vec3 fill(const float& num) {
        return glm::vec3(num);
    }
    inline const std::string getFileName(const std::string& filePath, const bool& includeExtension = false) {
        std::string dilimeter = "/";
        std::array<std::string, 2> possible = {
            "/", "\\"
        };
        for (const std::string& pos : possible) {
            if (contains(filePath, pos)) {
                dilimeter = pos;
                break;
            }
        }
        auto s = split(filePath, dilimeter);
        std::string res = s.back();
        if (!includeExtension) {
            s = split(res, ".");
            res = s[0];
        }
        return res;
    }
}