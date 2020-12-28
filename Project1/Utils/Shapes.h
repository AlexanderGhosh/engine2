#pragma once
#include <array>
#include <list>
#include <vector>
#include <glm.hpp>

namespace Utils {
	namespace Shapes
	{
		template<class T>
		class Edge {
		private:
			T a, b;
		public:
			Edge();
			Edge(const T& a, const T& b);
			T getEdge() const;
			void set(const T& a);
			void set(const T& a, const T& b);
			T& operator[](const char& index);
			bool operator==(const Edge<T>& b) const;
			bool operator!=(const Edge<T>& b) const;

			bool operator==(const T& b) const;
			bool operator!= (const T& b) const;

		};
		template<class T>
		static bool operator==(const T& a, const Edge<T>& b);
		template<class T>
		static bool operator!= (const T& a, const Edge<T>& b);

		template<class T, char size>
		class Face {
		private:
			using Points = std::array<T, size>;
			Points data;
		public:
			Face() : data() { };
			Face(const Points& points) : data(points)
			{
			};
			Face(const std::list<T>& points)
			{
				char i = 0;
				for(auto itt = points.begin(); itt != points.end() && i < size; itt)
					data[i++] = *itt;
			};

			void set(const Points& points)
			{
				data = points;
			};
			int getSize() const
			{
				return size;
			};

			glm::vec3 getNormal() const {
				return glm::normalize(glm::cross(data[1] - data[0], data[3] - data[2]));
			};

			T& operator[](const char& index)
			{
				char i = index;
				while (i < 0)
					i = size + i;
				i %= size;
				return data[i];
			};
			bool operator==(const Face<T, size>& b) const
			{
				for (char i = 0; i < size; i++)
					if (b[i] != data[i])
						return false;
				return true;
			};
			bool operator!=(const Face<T, size>& b) const
			{
				return !(this == b);
			};

			std::array<T, size> getDataArray() const
			{
				return data;
			};
			std::list<glm::vec3> getDataList() const
			{
				std::list<glm::vec3> res(data.begin(), data.end());
				return res;
			};
			std::vector<glm::vec3> getDataVector() const
			{
				return std::vector<glm::vec3>(data.begin(), data.end());
			};
		};
		using Plane = Face <glm::vec3, 3>;
		using LineSegment = Edge<glm::vec3>;
	};
};