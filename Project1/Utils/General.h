#pragma once
#include <vector>
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
}