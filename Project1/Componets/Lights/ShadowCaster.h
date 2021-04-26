#pragma once
#include "../Componets.h"

namespace Component {
	class Camera;
	class DirectionalLight;
	class ShadowCaster : public ComponetBase
	{
	private:
		class LightMatrix {
			float width, height, length, farHeight, nearHeight, farWidth, nearWidth;;
			glm::vec3 center;
			bool firstUpdate;
			glm::vec3 toLightSpace(Vector3 sp, Vector3 dir, Float width) const;
			std::array<glm::vec3, 8> getVertices(Vector3 up, Vector3 right, Vector3 centerNear, Vector3 centerFar) const;
		public:
			float SHADOW_DISTANCE, NEAR_PLANE, OFFSET;
			glm::vec3 DIRECTION;
			LightMatrix();
			~LightMatrix() = default;
			glm::mat4 getOrtho() const;
			glm::mat4 getView() const;
			glm::mat4 getMatrix() const;
			void update(Camera* camera, float ar);
			void cleanUp();
		};
		float nearPlane, farPlane;
		LightMatrix lightMatrixInfo;
		Camera* camera;
	public:
		ShadowCaster();
		~ShadowCaster() = default;
		void cleanUp();
		void update(float deltaTime);
		inline Type getType() const { return Type::ShadowCaster; };
		glm::mat4 getLSMatrix(const glm::ivec2& screenDimentions) const;
		Vector3 getPosition() const;
		void setLightSource(DirectionalLight* light);
		void setCamera(Camera* camera);
	};
}

