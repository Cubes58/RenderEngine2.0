#pragma once

#include <string>

#include "glad/glad.h"
#include "glm/glm.hpp"

class Shader {
private:
	unsigned int m_ID;

	bool CreateShader(GLuint &p_ShaderID, const GLenum &p_ShaderType, const GLchar *p_ShaderSource, const std::string &p_TypeInformation = "Shader");
	bool CheckErrors(GLuint p_Object, const std::string &p_Type);

public:
	Shader();

	bool Compile(const GLchar *p_VertexPath, const GLchar *p_FragmentPath, const GLchar *p_GeometryPath = nullptr);

	Shader &Use();

	void SetBool(const std::string &p_Name, bool p_Value) const;
	void SetInt(const std::string &p_Name, int p_Value) const;
	void SetFloat(const std::string &p_Name, float p_Value) const;

	void SetVec2(const std::string &p_Name, const glm::vec2 &p_Value) const;
	void SetVec2(const std::string &p_Name, float p_XValue, float p_YValue) const;

	void SetVec3(const std::string &p_Name, const glm::vec3 &p_Value) const;
	void SetVec3(const std::string &p_Name, float p_XValue, float p_YValue, float p_ZValue) const;

	void SetVec4(const std::string &p_Name, const glm::vec4 &p_Value) const;
	void SetVec4(const std::string &p_Name, float p_XValue, float p_YValue, float p_ZValue, float p_WValue) const;

	void SetMat2(const std::string &p_Name, const glm::mat2 &p_Mat) const;
	void SetMat3(const std::string &p_Name, const glm::mat3 &p_Mat) const;
	void SetMat4(const std::string &p_Name, const glm::mat4 &p_Mat) const;

	unsigned int GetID() const;
	void SetID(unsigned int p_ID);
};