#include "UniformBuffer.h"
#include <GL/glew.h>

#pragma region Static Variables
int Primative::Buffers::StaticBuffer::usedBindingPoint = -1;
int Primative::Buffers::StaticBuffer::maxBingingPoints = -1;
#pragma endregion

#pragma region Constructors CleanUp
Primative::Buffers::StaticBuffer::StaticBuffer() : UBO(0), bindingPoint(-1), positions()
{
	if (maxBingingPoints < 0)
		glGetIntegerv(GL_MAX_UNIFORM_BUFFER_BINDINGS, &maxBingingPoints);
}

Primative::Buffers::StaticBuffer::StaticBuffer(String types, char bindingPoint) : StaticBuffer(Utils::split(types, ", "), bindingPoint)
{
}

Primative::Buffers::StaticBuffer::StaticBuffer(const std::vector<std::string>& types, char bindingPoint) : StaticBuffer() {
	unsigned dataSize = 0, offset = 0;
	for (const std::string& type : types) {
		// floats
		if (type == "f") {
			dataSize = sizeof(float);
		}
		else if (type == "f2") {
			dataSize = sizeof(glm::vec2);
		}
		else if (type == "f3") {
			dataSize = sizeof(glm::vec3);
		}
		else if (type == "f4") {
			dataSize = sizeof(glm::vec4);
		}
		// ints
		else if (type == "i") {
			dataSize = sizeof(int);
		}
		else if (type == "i2") {
			dataSize = sizeof(glm::ivec2);
		}
		else if (type == "i3") {
			dataSize = sizeof(glm::ivec3);
		}
		else if (type == "i4") {
			dataSize = sizeof(glm::ivec4);
		}
		// matrices
		else if (type == "m2") {
			dataSize = sizeof(glm::mat2);
		}
		else if (type == "m3") {
			dataSize = sizeof(glm::mat3);
		}
		else if (type == "m4") {
			dataSize = sizeof(glm::mat4);
		}
		else if (Utils::contains(type, "m4")) {
			std::string num = Utils::replaceAll(type, "m4", "");
			int a = std::stoi(num);
			dataSize = sizeof(glm::mat4) * a;
		}
		positions.push_back({ offset, dataSize });
		offset += dataSize;
	}
	init(offset, bindingPoint);
}

const void Primative::Buffers::StaticBuffer::init(unsigned dataSize, short bindingPoint)
{
	this->bindingPoint = (bindingPoint < 0) ? usedBindingPoint + 1 : bindingPoint;
	this->bindingPoint %= maxBingingPoints;
	usedBindingPoint = this->bindingPoint;
	glGenBuffers(1, &UBO);
	this->bind();

	glBufferData(GL_UNIFORM_BUFFER, dataSize, NULL, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	glBindBufferRange(GL_UNIFORM_BUFFER, this->bindingPoint, UBO, 0, dataSize);
}

void Primative::Buffers::StaticBuffer::cleanUp()
{
	bind();
	glDeleteBuffers(1, &UBO);
	unBind();
	positions.clear();
}
#pragma endregion

#pragma region Getters
const short Primative::Buffers::StaticBuffer::getBindingPoint() const
{
	return bindingPoint;
}
#pragma endregion

#pragma region Fill Bind
void Primative::Buffers::StaticBuffer::fill(unsigned offset, unsigned size, const void* data) const
{
	bind();
	glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
	unBind();
}

void Primative::Buffers::StaticBuffer::fill(unsigned position, const void* data) const
{
	const std::pair<unsigned, unsigned>& pair = positions[position];
	fill(pair.first, pair.second, data);
}

void Primative::Buffers::StaticBuffer::bind() const
{
	glBindBuffer(GL_UNIFORM_BUFFER, UBO);
}

void Primative::Buffers::StaticBuffer::unBind() const
{
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}
#pragma endregion