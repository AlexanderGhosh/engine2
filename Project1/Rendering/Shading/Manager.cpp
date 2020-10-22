#include "Manager.h"
unsigned Render::Shading::Manager::active = 0;

 bool Render::Shading::Manager::setValue(const std::string& name, int val) {
	int loc = glGetUniformLocation(Render::Shading::Manager::active, name.c_str());
	if (loc < 0) return false;
	glUniform1i(loc, val);
	return true;
};
 bool Render::Shading::Manager::setValue(const std::string& name, float val) {
	int loc = glGetUniformLocation(Render::Shading::Manager::active, name.c_str());
	if (loc < 0) return false;
	glUniform1f(loc, val);
	return true;
};
 bool Render::Shading::Manager::setValue(const std::string& name, glm::vec2 val) {
	int loc = glGetUniformLocation(Render::Shading::Manager::active, name.c_str());
	if (loc < 0) return false;
	glUniform2f(loc, val[0], val[1]);
	return true;
};
 bool Render::Shading::Manager::setValue(const std::string& name, glm::vec3 val) {
	int loc = glGetUniformLocation(Render::Shading::Manager::active, name.c_str());
	if (loc < 0) return false;
	glUniform3f(loc, val[0], val[1], val[2]);
	return true;
};
 bool Render::Shading::Manager::setValue(const std::string& name, glm::vec4 val) {
	int loc = glGetUniformLocation(Render::Shading::Manager::active, name.c_str());
	if (loc < 0) return false;
	glUniform4f(loc, val[0], val[1], val[2], val[3]);
	return true;
};
 bool Render::Shading::Manager::setValue(const std::string& name, glm::mat3 val) {
	int loc = glGetUniformLocation(Render::Shading::Manager::active, name.c_str());
	if (loc < 0) return false;
	glUniformMatrix3fv(loc, 1, GL_FALSE, glm::value_ptr(val));
	return true;
};
 bool Render::Shading::Manager::setValue(const std::string& name, glm::mat4 val) {
	int loc = glGetUniformLocation(Render::Shading::Manager::active, name.c_str());
	if (loc < 0) return false;
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(val));
	return true;
}
 bool Render::Shading::Manager::setValue(const std::string& name, const Materials::MatItem& fwd, const short& texUnit)
 {
	 if (!fwd.hasTex()) {
		 return Render::Shading::Manager::setValue(name + "_vec", glm::vec4(fwd.getRaw(), 1));
	 }
	 Render::Shading::Manager::setValue(name + "_vec", glm::vec4(0));
	 return Render::Shading::Manager::setValue(name + "_id", texUnit); // set to texture unit
 }
 bool Render::Shading::Manager::setValue(const std::string& name, Materials::Forward& fwd)
 {
	 bool vals[] = { 0, 0 ,0 };
	 std::string names[] = { ".diffuse", ".specular" };
	 auto& diff_spec = fwd.getDiffSpec();
	 for (short i = 0; i < 2; i++) {
		 vals[i] = !Render::Shading::Manager::setValue(name + names[i], diff_spec[i], i);
	 }
	 vals[2] = !Render::Shading::Manager::setValue(name + ".shininess", fwd.shininess);

	 return vals[0] && vals[1] && vals[2];
 }
 