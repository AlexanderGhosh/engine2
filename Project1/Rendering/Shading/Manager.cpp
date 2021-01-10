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
 bool Render::Shading::Manager::setValue(const std::string& name, const Materials::MatItem& fwd, short& texUnit)
 {
	 if (!fwd.hasTex() || texUnit < 0) {
		 texUnit--;
		 return Render::Shading::Manager::setValue(name + "_vec", glm::vec4(fwd.getRaw(), 1));
	 }
	 Render::Shading::Manager::setValue(name + "_vec", glm::vec4(fwd.getRaw(), 0));
	 return Render::Shading::Manager::setValue(name + "_id", texUnit); // set to texture unit
 }
 bool Render::Shading::Manager::setValue(const std::string& name, Materials::Forward& fwd)
 {
	 bool vals[] = { 0, 0, 0, 0 };
	 std::string names[] = { ".diffuse", ".specular", ".normals" };
	 auto& diff_spec_norm = fwd.getDiffSpecNorm();
	 for (short i = 0; i < 3; i++) {
		 vals[i] = Render::Shading::Manager::setValue(name + names[i], diff_spec_norm[i], i);
	 }
	 vals[3] = Render::Shading::Manager::setValue(name + ".shininess", fwd.shininess);

	 return vals[0] && vals[1] && vals[2] && vals[3];
 }
 bool Render::Shading::Manager::setValue(const std::string& name, Materials::PBR& mat)
 {
	 bool vals[] = { 0, 0, 0, 0, 0 };
	 std::string names[] = { ".albedo", ".normal", ".metalic", ".roughness", ".ao" };
	 const auto all = mat.getAll();
	 short unit = 0;
	 for (short i = 0; i < 5; i++) {
		 vals[i] = Render::Shading::Manager::setValue(name + names[i], all[i], unit);
		 unit++;
	 }
	 setValue("hdrMap", unit++);
	 setValue("iblMap", unit++);
	 setValue("brdfLUT", unit);
	 return vals[0] && vals[1] && vals[2] && vals[3] && vals[4];
 }

 void Render::Shading::Manager::setActive(const unsigned& shaderId)
 {
	 Manager::active = shaderId;
	 glUseProgram(shaderId);
 }

 void Render::Shading::Manager::cleanUp()
 {
	 glUseProgram(0);
 }
 