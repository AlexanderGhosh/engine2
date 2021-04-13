#include "Manager.h"
#include "../../Primatives/Materials/MaterialBase.h"
#include "../../Primatives/Materials/Forward.h"
#include "../../Primatives/Materials/PBR.h"
#include "../Animation/Animation.h"
unsigned Render::Shading::Manager::active = 0;

bool Render::Shading::Manager::setValue(String name, int val) {
	int loc = glGetUniformLocation(Render::Shading::Manager::active, name.c_str());
	if (loc < 0) return false;
	glUniform1i(loc, val);
	return true;
};
bool Render::Shading::Manager::setValue(String name, float val) {
	int loc = glGetUniformLocation(Render::Shading::Manager::active, name.c_str());
	if (loc < 0) return false;
	glUniform1f(loc, val);
	return true;
};
bool Render::Shading::Manager::setValue(String name, glm::vec2 val) {
	int loc = glGetUniformLocation(Render::Shading::Manager::active, name.c_str());
	if (loc < 0) return false;
	glUniform2f(loc, val[0], val[1]);
	return true;
};
bool Render::Shading::Manager::setValue(String name, glm::vec3 val) {
	int loc = glGetUniformLocation(Render::Shading::Manager::active, name.c_str());
	if (loc < 0) return false;
	glUniform3f(loc, val[0], val[1], val[2]);
	return true;
};
bool Render::Shading::Manager::setValue(String name, glm::vec4 val) {
	int loc = glGetUniformLocation(Render::Shading::Manager::active, name.c_str());
	if (loc < 0) return false;
	glUniform4f(loc, val[0], val[1], val[2], val[3]);
	return true;
};
bool Render::Shading::Manager::setValue(String name, glm::mat3 val) {
	int loc = glGetUniformLocation(Render::Shading::Manager::active, name.c_str());
	if (loc < 0) return false;
	glUniformMatrix3fv(loc, 1, GL_FALSE, glm::value_ptr(val));
	return true;
};
bool Render::Shading::Manager::setValue(String name, glm::mat4 val) {
	int loc = glGetUniformLocation(Render::Shading::Manager::active, name.c_str());
	if (loc < 0) return false;
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(val));
	return true;
}
bool Render::Shading::Manager::setValue(String name, const Materials::MaterialBase* mat)
 {
	 if (Utils::isType<Materials::MaterialBase, Materials::Forward>(mat)) {
		 return setValue(name, *dynamic_cast<const Materials::Forward*>(mat));
	 }
	 else {
		 return setValue(name, *dynamic_cast<const Materials::PBR*>(mat));
	 }
 }
bool Render::Shading::Manager::setValue(String name, const Render::Animation::KeyFrame& frame)
 {
	 if (frame.translations.size() == 0)
		 return true;
	 int loc = glGetUniformLocation(Render::Shading::Manager::active, name.c_str());
	 if (loc < 0) return false;
	 glUniformMatrix4fv(loc, frame.translations.size(), GL_FALSE, glm::value_ptr(frame.translations[0])); 
	 return true;
 }
bool Render::Shading::Manager::setValue(String name, const std::vector<glm::mat4>& matrices)
 {
	 int loc = glGetUniformLocation(Render::Shading::Manager::active, name.c_str());
	 if (loc < 0) return false;
	 glUniformMatrix4fv(loc, matrices.size(), GL_FALSE, glm::value_ptr(matrices[0]));
	 return true;
 }
bool Render::Shading::Manager::setValue(String name, const Materials::Forward& fwd)
 {
	 bool valid = true;
	 unsigned unit = 1;

	 valid = valid AND Render::Shading::Manager::setValue(name + ".diffuse",  *fwd.getDiffuse(), unit);
	 valid = valid AND Render::Shading::Manager::setValue(name + ".normals",  *fwd.getNormals(), unit);
	 valid = valid AND Render::Shading::Manager::setValue(name + ".specular", *fwd.getSpecular(), unit);

	 return valid;

	 /*std::string names[] = { ".diffuse", ".specular", ".normals" };
	 auto& diff_spec_norm = fwd.getDiffSpecNorm();
	 short unit = 1;
	 for (short i = 0; i < 3; i++) {
		 vals[i] = Render::Shading::Manager::setValue(name + names[i], diff_spec_norm[i], unit);
		 unit++;
	 }
	 vals[3] = Render::Shading::Manager::setValue(name + ".shininess", fwd.shininess);

	 return vals[0] && vals[1] && vals[2] && vals[3];*/
 }
bool Render::Shading::Manager::setValue(String name, const Materials::PBR& mat)
 {
	 unsigned unit = 1;
	 bool val = true;
	 val = setValue(name + ".albedo", *mat.getAlbedo(), unit) AND val;
	 val = setValue(name + ".normal", *mat.getNormal(), unit) AND val;
	 val = setValue(name + ".metalic", *mat.getMetalic(), unit) AND val;
	 val = setValue(name + ".roughness", *mat.getRoughness(), unit) AND val;
	 val = setValue(name + ".ao", *mat.getAO(), unit) AND val;

	 /*std::string names[] = { ".albedo", ".normal", ".metalic", ".roughness", ".ao" };
	 const auto all = mat.getAll();
	 bool val = true;
	 for (short i = 0; i < 5; i++) {
		 val = val AND Render::Shading::Manager::setValue(name + names[i], all[i], unit);
		 unit++;
	 }*/
	 val = setValue("hdrMap", static_cast<int>(unit++)) AND val;
	 val = setValue("iblMap", static_cast<int>(unit++)) AND val;
	 val = setValue("brdfLUT", static_cast<int>(unit)) AND val;
	 return val;
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
 