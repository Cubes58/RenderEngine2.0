#pragma once

#include <memory>
#include <string>
#include <vector>
#include <map>

class Scene;
class Model;
class Shader;
struct FileInformation;

#define ResourceManagerInstance ResourceManager::Instance()

class ResourceManager {
private:
	friend Scene;

	std::map<std::string, std::shared_ptr<Model>> m_Models;
	std::map<std::string, std::shared_ptr<Shader>> m_Shaders;

	std::vector<std::string> m_UnsuccessfullyLoadedModels;

	ResourceManager();
	~ResourceManager();

	bool LoadShaderFromFile(const std::string &p_VertexShaderFile, const std::string &p_FragmentShaderFile, const std::string &p_GeometryShaderFile = " ");

public:
	static ResourceManager &Instance();

	bool LoadShadersFromFolder(const std::string &p_FolderPath);
	std::shared_ptr<Shader> LoadShader(const std::string &p_VertexShaderFile, const std::string &p_FragmentShaderFile, const std::string &p_GeometryShaderFile = " ");
	std::shared_ptr<Shader> GetShader(const std::string &p_Name);

	static unsigned int LoadOpenGLTexture(const std::string &p_FilePath);
	static unsigned int LoadOpenGLCubemapTexture(const std::vector<std::string> &p_CubemapFaces);

	bool LoadModelsFromFolder(const std::string &p_FolderName);
	bool LoadModelFromFile(const FileInformation &p_FileLocation);
	bool LoadModelFromFile(std::string p_FileLocation);
	std::shared_ptr<Model> GetModel(const std::string &p_ModelName);
};