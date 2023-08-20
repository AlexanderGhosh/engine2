#pragma once
#include "../Utils/General.h"
#include "../Primatives/Model.h"

namespace Gizmos {
	enum class Types {
		Point, Line, Circle, Cuboide, Sphere, Triangle
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
		void draw() override;
		void cleanUp() override;
		Types getType() const override;
	};

	class Line : public Gizmo {
	private:
		glm::vec3 p1_, p2_;
	public:
		Line();
		Line(Vector3 pos, Vector3 col);
		Line(Vector3 from, Vector3 to, bool over);
		~Line() = default;
		void draw() override;
		void cleanUp() override;
		Types getType() const override;
		void setLeftOffset(Vector3 left);
		void setRightOffset(Vector3 right);

		void setPosition(Vector3 pos) override;
	};

	class Circle : public Gizmo {
	private:
		float radius;
		glm::vec3 up;
	public:
		Circle();
		Circle(Vector3 pos, Vector3 col);
		~Circle() = default;
		void draw() override;
		void cleanUp() override;
		Types getType() const override;
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
		void draw() override;
		void cleanUp() override;
		Types getType() const override;
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
		void draw() override;
		void cleanUp() override;
		Types getType() const override;
		void setRadius(Float r);
		void setPosition(Vector3 pos) override;
		void setColour(Vector3 colour) override;
	};

	class Triangle : public Gizmo {
	private:
		std::array<Line, 3> lines;
		std::vector<Point> points;
	public:
		Triangle();
		Triangle(Vector3 p1, Vector3 p2, Vector3 p3, bool drawPoints);
		~Triangle() = default;

		void draw() override;
		void cleanUp() override;
		Types getType() const override;

		void setPosition(Vector3 pos) override;
		void setColour(Vector3 colour) override;
	};
}
