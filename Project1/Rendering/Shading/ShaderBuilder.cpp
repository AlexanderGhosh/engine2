#include "ShaderBuilder.h"
#include <iostream>
#include <fstream>
#include "Manager.h"
#include <gtc/type_ptr.hpp>

std::map<Render::Shader::VertexParts, std::string> Render::Shader::ShaderBuilder::vertexText = {};
std::map<Render::Shader::FragmentParts, unsigned> Render::Shader::ShaderBuilder::fragmentPrograms = {};
std::map<std::string, Render::Shader::PipelineShader> Render::Shader::ShaderBuilder::programPipelines = {};

void Render::Shader::ShaderBuilder::init()
{
	vertexText.insert({ VertexParts::StartMain, "void main() {" });
	vertexText.insert({ VertexParts::EndMain, "}" });
	loadVertexData();
	loadFragmentData();
}

void Render::Shader::ShaderBuilder::loadFragmentData(String dir)
{
	auto dirIter = std::filesystem::directory_iterator(dir);
	int i = 0;
	for (auto& doc : dirIter)
	{
		std::string line;

		// Read from the text doc
		std::ifstream file = std::ifstream(doc.path());

		// Use a while loop together with the getline() function to read the doc line by line
		int lineNumber = 1;
		FragmentParts fragPart = FragmentParts::Empty;
		std::string shaderText = "";
		while (std::getline(file, line)) {
			if (lineNumber == 1) {
				fragPart = static_cast<FragmentParts>(std::stoi(line));
			}
			else {
				shaderText += line + '\n';
			}
			lineNumber++;
		}
		shaderText.pop_back();
		const char* shaderData = shaderText.c_str();
		fragmentPrograms[fragPart] = glCreateShaderProgramv(GL_FRAGMENT_SHADER, 1, &shaderData);
		if (NOT fragmentPrograms[fragPart]) {
			std::cout << doc.path().string() + " : Frag Failed\n";
		}
		setFragmentUniforms(fragPart, fragmentPrograms[fragPart]);

		// Close the doc
		file.close();
	}
}

void Render::Shader::ShaderBuilder::loadVertexData(String filePath)
{
	std::string line;

	// Read from the text doc
	std::ifstream file = std::ifstream(filePath);

	// Use a while loop together with the getline() function to read the doc line by line
	int lineNumber = 1;
	int nextBegin = 1;
	VertexParts part = VertexParts::CameraInfo;
	while (std::getline(file, line)) {
		line = Utils::trim(line);
		std::vector<std::string> split = Utils::split(line, ",");
		
		if (lineNumber == nextBegin) {
			if (vertexText[part].size() > 0) {
				vertexText[part].pop_back();
			}
			part = static_cast<VertexParts>(std::stoi(split[0]));
			nextBegin = std::stoi(split[1]);
		}
		else {
			vertexText[part] += line + '\n';
		}
		lineNumber++;
	}
	// Close the doc
	file.close();
}

Render::Shader::PipelineShader Render::Shader::ShaderBuilder::requestShader(VertexDescrition vert, FragmentParts frag)
{
	const unsigned s = programPipelines.size();
	PipelineShader& pipeline = programPipelines[getUID(vert, frag)];
	if (s == programPipelines.size()) {
		return pipeline;
	}
	// Create two separable program objects from a 
	// single source string respectively (vertSrc and fragSrc)
	std::string vertexTextCombined;
	for (const VertexParts& part : vert) {
		vertexTextCombined += vertexText[part] + '\n';
	}
	vertexTextCombined.pop_back();
	const char* vertexData = vertexTextCombined.c_str();
	const unsigned vertProg = glCreateShaderProgramv(GL_VERTEX_SHADER, 1, &vertexData);	
	const unsigned fragProg = fragmentPrograms[frag];

	// Generate a program pipeline and attach the programs to their respective stages
	glGenProgramPipelines(1, &pipeline.piplineProgram);
	glBindProgramPipeline(pipeline.piplineProgram);
	glUseProgramStages(pipeline.piplineProgram, GL_VERTEX_SHADER_BIT, vertProg);
	glUseProgramStages(pipeline.piplineProgram, GL_FRAGMENT_SHADER_BIT, fragProg);
	glUseProgramStages(pipeline.piplineProgram, GL_ALL_SHADER_BITS, 0);
	glActiveShaderProgram(pipeline.piplineProgram, pipeline.vertexID);
	pipeline.vertexID = vertProg;
	pipeline.fragmentID = fragProg;
	int location = glGetUniformLocation(pipeline.vertexID, "model");
	glm::mat4 m(1);
	glProgramUniformMatrix4fv(pipeline.vertexID, location, 1, false, glm::value_ptr(m));
	glBindProgramPipeline(0);

	// NEEDS CLEAN UP i think
	return pipeline;
}

std::string Render::Shader::ShaderBuilder::getUID(VertexDescrition vert, FragmentParts frag)
{
	std::string res;
	for (const VertexParts& v : vert) {
		res += std::to_string(static_cast<int>(v));
	}
	res += std::to_string(pow(static_cast<int>(frag), 2));
	return res;
}

void Render::Shader::ShaderBuilder::cleanUp()
{
	for (auto itt = programPipelines.begin(); itt != programPipelines.end();) {
		glDeleteProgramPipelines(1, &(*itt).second.piplineProgram);
		itt = programPipelines.erase(itt);
	}
}

void Render::Shader::ShaderBuilder::setFragmentUniforms(FragmentParts fragPart, unsigned& program)
{
	switch (fragPart)
	{
	case FragmentParts::Bling_Phong:
		#pragma region Bling Phong
		/*GLint colorLoc = glGetUniformLocation(program, "Color");
		glProgramUniform4f(program, colorLoc, 1.f, 0.f, 0.f, 1.f);*/
		break;
		#pragma endregion
	case FragmentParts::PBR:
		/*GLint colorLoc = glGetUniformLocation(program, "Color");
		glProgramUniform4f(program, colorLoc, 1.f, 0.f, 0.f, 1.f);*/
		break;
	case FragmentParts::Empty:
		/*GLint colorLoc = glGetUniformLocation(program, "Color");
		glProgramUniform4f(program, colorLoc, 1.f, 0.f, 0.f, 1.f);*/
		break;
	}
	// Query and set any uniforms
}

void Render::Shader::ShaderBuilder::setVertexUniforms(VertexDescrition vertexDesc, unsigned& program)
{
	return;
	for (const VertexParts& part : vertexDesc) {
		unsigned id = static_cast<unsigned>(part);
		if (id == 13) {
			GLint colorLoc = glGetUniformLocation(program, "Model");
			glProgramUniform4f(program, colorLoc, 1.f, 0.f, 0.f, 1.f);
		}
	}
}

Render::Shader::PipelineShader::PipelineShader() : piplineProgram(), vertexID(), fragmentID()
{
}

Render::Shader::PipelineShader::PipelineShader(Unsigned pipeline, Unsigned vert, Unsigned frag) : PipelineShader()
{
	piplineProgram = pipeline;
	vertexID = vert;
	fragmentID = frag;
}
