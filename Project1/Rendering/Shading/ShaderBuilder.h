#pragma once
#include <map>
#include "../../Utils/General.h"
namespace Render {
	namespace Shader {
		enum class VertexParts {
			CameraInfo, LightingInfo, UIInfo,
			Position, TextureCoords, Normal, BoneIndices, BoneWeights,
			Out_Pos, Out_Tex, Out_Norm,
			ProjectView, ProjectViewModel,
			UniformModel,
			Pass_Pos, Pass_Tex, Pass_Norm,
			StartMain, EndMain
		};
		enum class FragmentParts {
			Bling_Phong, PBR, Empty
		};

		using VertexDescrition = std::vector<VertexParts>;
		class ShaderBuilder
		{
		private:
			static std::map<VertexParts, std::string> vertexText;
			static std::map<FragmentParts, unsigned> fragmentPrograms;
			static std::map<std::string, unsigned> programPipelines;

			static void setFragmentUniforms(FragmentParts fragPart, unsigned& program);
			static void setVertexUniforms(VertexDescrition vertexDesc, unsigned& program);
			static void loadFragmentData(String dir = "Resources/Shaders/FragmentParts");
			static void loadVertexData(String filePath = "Resources/Shaders/VertexParts.dat");
		public:
			static void init();
			static unsigned requestShader(VertexDescrition vert, FragmentParts frag);
			static std::string getUID(VertexDescrition vert, FragmentParts frag);
			static void cleanUp();
		};
	}
}
