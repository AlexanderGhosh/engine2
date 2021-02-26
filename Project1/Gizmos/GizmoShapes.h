#pragma once
#include "../Utils/General.h"
#include "../Primatives/Model.h"

namespace Gizmos {
	enum class Types {
		Point, Line, Circle, Cuboide, Sphere
	};
	class Gizmo {
	protected:
		glm::vec3 position, colour;
		float thickness;
		unsigned pointVAO, shaderID;
		Gizmo();
		void bindShader() const;
		void bindVAO() const;
		void unbindVAO() const;
		void drawGeneral();
	public:
		virtual void draw() = 0;
		virtual void cleanUp() = 0;
		virtual Types getType() const = 0;
		
		void setThickness(Unsigned size);
		Vector3 getPosition() const;
		Vector3 getColour() const;

		virtual void setPosition(Vector3 pos);
		virtual void setColour(Vector3 colour);
	};
	class Point : public Gizmo {
	public:
		Point();
		Point(Vector3 pos, Vector3 col);
		~Point() = default;
		void draw();
		void cleanUp();
		Types getType() const;
	};
	class Line : public Gizmo {
	private:
		glm::vec3 leftOffset, rightOffset;
	public:
		Line();
		Line(Vector3 pos, Vector3 col);
		~Line() = default;
		void draw();
		void cleanUp();
		Types getType() const;
		void setLeftOffset(Vector3 left);
		void setRightOffset(Vector3 right);
	};
	class Circle : public Gizmo {
	private:
		float radius;
		glm::vec3 up;
	public:
		Circle();
		Circle(Vector3 pos, Vector3 col);
		~Circle() = default;
		void draw();
		void cleanUp();
		Types getType() const;
		void setRadius(Float r);
		void setUp(Vector3 up);
	};
	class Cuboide : public Gizmo {
	private:
		glm::vec3 dimensions;
		Primative::Model cubeModel;
	public:
		Cuboide();
		Cuboide(Vector3 pos, Vector3 col);
		~Cuboide() = default;
		void draw();
		void cleanUp();
		Types getType() const;
		void setDimentions(Vector3 dim);
	};
	class Sphere : public Gizmo {
	private:
		float radius;
		bool updated;
		std::array<Circle, 3> cirlces;
	public:
		Sphere();
		Sphere(Vector3 pos, Vector3 col);
		~Sphere() = default;
		void draw();
		void cleanUp();
		Types getType() const;
		void setRadius(Float r);
		void setPosition(Vector3 pos);
		void setColour(Vector3 colour);
	};
}
