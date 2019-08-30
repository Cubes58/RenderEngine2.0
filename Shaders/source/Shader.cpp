#include "Shader.h"

#include <fstream>
#include <sstream>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

Shader::Shader() : m_ID(999999) {

}

bool Shader::Compile(const GLchar *p_VertexPath, const GLchar *p_FragmentPath, const GLchar *p_GeometryPath) {
	GLuint sVertex;
	GLuint sFragment;
	GLuint gShader;

	// Vertex Shader:
	CreateShader(sVertex, GL_VERTEX_SHADER, p_VertexPath, "VERTEX");
	// Fragment Shader:
	CreateShader(sFragment, GL_FRAGMENT_SHADER, p_FragmentPath, "FRAGMENT");
	// Geometry Shader:
	if (p_GeometryPath != nullptr)
		CreateShader(gShader, GL_GEOMETRY_SHADER, p_GeometryPath, "GEOMETRY");

	// Shader Program:
	m_ID = glCreateProgram();
	glAttachShader(m_ID, sVertex);
	glAttachShader(m_ID, sFragment);
	if (p_GeometryPath != nullptr)
		glAttachShader(m_ID, gShader);

	glLinkProgram(m_ID);
	bool successful = CheckErrors(m_ID, "PROGRAM");

	// Clean up:
	glDeleteShader(sVertex);
	glDeleteShader(sFragment);
	if (p_GeometryPath != nullptr)
		glDeleteShader(gShader);

	std::cout << "\n\n";
	return successful;
}

bool Shader::CreateShader(GLuint &p_ShaderID, const GLenum &p_ShaderType, const GLchar *p_ShaderSource, const std::string &p_TypeInformation) {
	std::cout << "\nSHADER SOURCE CODE:" << p_ShaderType << "	" << "SHADER TYPE: " << p_TypeInformation << "\n" << p_ShaderSource << "\n";

	p_ShaderID = glCreateShader(p_ShaderType);
	glShaderSource(p_ShaderID, 1, &p_ShaderSource, NULL);
	glCompileShader(p_ShaderID);

	return CheckErrors(p_ShaderID, p_TypeInformation);
}

bool Shader::CheckErrors(GLuint p_Object, const std::string &p_Type) {
	GLint success;
	GLchar infoLog[1024];
	if (p_Type != "PROGRAM") {
		glGetShaderiv(p_Object, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(p_Object, 1024, NULL, infoLog);
			std::cout << "| SHADER: Compile-time error: Type: " << p_Type << "\n" << infoLog << "\n -- --------------------------------------------------- -- ";
			return false;
		}
	}
	else {
		glGetProgramiv(p_Object, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(p_Object, 1024, NULL, infoLog);
			std::cout << "| SHADER: Compile-time error: Type: " << p_Type << "\n" << infoLog << "\n -- --------------------------------------------------- -- ";
			return false;
		}
	}
	return true;
}

Shader &Shader::Use() {
	glUseProgram(m_ID);
	return *this;
}

void Shader::SetBool(const std::string &p_Name, bool p_Value) const {
	glUniform1i(glGetUniformLocation(m_ID, p_Name.c_str()), static_cast<GLint>(p_Value));
}

void Shader::SetInt(const std::string &p_Name, int p_Value) const {
	glUniform1i(glGetUniformLocation(m_ID, p_Name.c_str()), p_Value);
}

void Shader::SetFloat(const std::string &p_Name, float p_Value) const {
	glUniform1f(glGetUniformLocation(m_ID, p_Name.c_str()), p_Value);
}

void Shader::SetVec2(const std::string &p_Name, const glm::vec2 &p_Value) const {
	glUniform2fv(glGetUniformLocation(m_ID, p_Name.c_str()), 1, &p_Value[0]);
}
void Shader::SetVec2(const std::string &p_Name, float p_XValue, float p_YValue) const {
	glUniform2f(glGetUniformLocation(m_ID, p_Name.c_str()), p_XValue, p_YValue);
}
// ------------------------------------------------------------------------
void Shader::SetVec3(const std::string &p_Name, const glm::vec3 &p_Value) const {
	glUniform3fv(glGetUniformLocation(m_ID, p_Name.c_str()), 1, &p_Value[0]);
}
void Shader::SetVec3(const std::string &p_Name, float p_XValue, float p_YValue, float p_ZValue) const {
	glUniform3f(glGetUniformLocation(m_ID, p_Name.c_str()), p_XValue, p_YValue, p_ZValue);
}
// ------------------------------------------------------------------------
void Shader::SetVec4(const std::string &p_Name, const glm::vec4 &p_Value) const {
	glUniform4fv(glGetUniformLocation(m_ID, p_Name.c_str()), 1, &p_Value[0]);
}
void Shader::SetVec4(const std::string &p_Name, float p_XValue, float p_YValue, float p_ZValue, float p_WValue) const {
	glUniform4f(glGetUniformLocation(m_ID, p_Name.c_str()), p_XValue, p_YValue, p_ZValue, p_WValue);
}
// ------------------------------------------------------------------------
void Shader::SetMat2(const std::string &p_Name, const glm::mat2 &p_Mat) const {
	glUniformMatrix2fv(glGetUniformLocation(m_ID, p_Name.c_str()), 1, GL_FALSE, &p_Mat[0][0]);
}
// ------------------------------------------------------------------------
void Shader::SetMat3(const std::string &p_Name, const glm::mat3 &p_Mat) const {
	glUniformMatrix3fv(glGetUniformLocation(m_ID, p_Name.c_str()), 1, GL_FALSE, &p_Mat[0][0]);
}
// ------------------------------------------------------------------------
void Shader::SetMat4(const std::string &p_Name, const glm::mat4 &p_Mat) const {
	glUniformMatrix4fv(glGetUniformLocation(m_ID, p_Name.c_str()), 1, GL_FALSE, &p_Mat[0][0]);
}

unsigned int Shader::GetID() const {
	return m_ID;
}

void Shader::SetID(unsigned int p_ID) {
	m_ID = p_ID;
}