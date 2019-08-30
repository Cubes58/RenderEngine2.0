#include "ResourceManager.h"

#include <fstream>
#include <sstream>
#include <iostream>

#include "GLAD/glad.h"
#include "STB_IMAGE/stb_image.h"

#include "FileSystemHelper.h"
#include "Model.h"
#include "Shader.h"

ResourceManager::ResourceManager() {
	LoadShadersFromFolder("resources/shaders/");
	LoadModelsFromFolder("resources/models/");
}

ResourceManager::~ResourceManager() {
	for (auto &shader : m_Shaders)
		glDeleteProgram(shader.second->GetID());
}

ResourceManager &ResourceManager::Instance() {
	static ResourceManager s_ResourceManager;

	return s_ResourceManager;
}

bool ResourceManager::LoadShaderFromFile(const std::string &p_VertexShaderFile, const std::string &p_FragmentShaderFile, const std::string &p_GeometryShaderFile) {
	// Retrieve the vertex/fragment source code, from the file path.
	std::string vertexCode;
	std::string fragmentCode;
	std::string geometryCode;

	std::ifstream vertexShaderFile;
	std::ifstream fragmentShaderFile;
	try {
		// Open files.
		vertexShaderFile.open(p_VertexShaderFile);
		fragmentShaderFile.open(p_FragmentShaderFile);

		std::stringstream vShaderStream;
		std::stringstream fShaderStream;
		if (vertexShaderFile.is_open() && fragmentShaderFile.is_open()) {
			// Read the files' buffer contents into streams.
			vShaderStream << vertexShaderFile.rdbuf();
			fShaderStream << fragmentShaderFile.rdbuf();
		}
		// Close file handlers.
		vertexShaderFile.close();
		fragmentShaderFile.close();

		// Convert the stream into a string.
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();

		// If the geometry shader path is present, also load a geometry shader.
		if (p_GeometryShaderFile != " ") {
			std::ifstream geometryShaderFile(p_GeometryShaderFile);
			if (geometryShaderFile.is_open()) {
				std::stringstream gShaderStream;
				gShaderStream << geometryShaderFile.rdbuf();
				geometryShaderFile.close();
				geometryCode = gShaderStream.str();
			}
		}
	}
	catch (std::exception p_Exception) {
		std::cerr << "ERROR::SHADER: Failed to read shader files.";
		if (vertexShaderFile.is_open() && fragmentShaderFile.is_open()) {
			// Close file handlers.
			vertexShaderFile.close();
			fragmentShaderFile.close();
		}

		return false;
	}
	catch (...) {
		std::cerr << "SOMETHING BAD WENT WRONG WITH THE SHADER LOAD FROM FILE CODE.";
		if (vertexShaderFile.is_open() && fragmentShaderFile.is_open()) {
			// Close file handlers.
			vertexShaderFile.close();
			fragmentShaderFile.close();
		}
		return false;
	}

	const GLchar *vShaderCode = vertexCode.c_str();
	const GLchar *fShaderCode = fragmentCode.c_str();
	const GLchar *gShaderCode = geometryCode.c_str();

	// Create the shader object from the source code.
	std::shared_ptr<Shader> shader = std::make_shared<Shader>();
	bool success = shader->Compile(vShaderCode, fShaderCode, p_GeometryShaderFile != " " ? gShaderCode : nullptr);
	if (success) {
		m_Shaders.emplace(FileSystemHelper::GetNameFromFile(p_VertexShaderFile), shader);
	}

	return success;
}

bool ResourceManager::LoadShadersFromFolder(const std::string &p_FolderPath) {
	std::vector<FileInformation> shaderFiles = FileSystemHelper::GetFilesInFolder(p_FolderPath);
	FileSystemHelper::RetainRemoveFilesWithExtensions(shaderFiles, { ".vert", ".VERT", ".frag", ".FRAG", ".geom", ".GEOM" });

	struct ShaderGroup {
		std::string m_VertexShaderLocation;
		std::string m_FragmentShaderLocation;
		std::string m_GeometryShaderLocation;

		ShaderGroup() : m_VertexShaderLocation(" "), m_FragmentShaderLocation(" "), m_GeometryShaderLocation(" ") { }
		ShaderGroup(const std::string &p_VertexShaderLocation, const std::string &p_FragmentShaderLocation, const std::string &p_GeometryShaderLocation = " ")
			: m_VertexShaderLocation(p_VertexShaderLocation), m_FragmentShaderLocation(p_FragmentShaderLocation), m_GeometryShaderLocation(p_GeometryShaderLocation) { }
	};

	bool allSuccessful = true;
	std::map<std::string, ShaderGroup> shaders;
	for (auto &shaderFile : shaderFiles) {
		auto iter = shaders.find(shaderFile.m_Name);
		if (iter == shaders.end()) {
			shaders.emplace(shaderFile.m_Name, ShaderGroup());
		}
		if (shaderFile.m_Extension == ".vert" || shaderFile.m_Extension == ".VERT")
			shaders[shaderFile.m_Name].m_VertexShaderLocation = shaderFile.m_Location;
		else if (shaderFile.m_Extension == ".frag" || shaderFile.m_Extension == ".FRAG")
			shaders[shaderFile.m_Name].m_FragmentShaderLocation = shaderFile.m_Location;
		else if (shaderFile.m_Extension == ".geom" || shaderFile.m_Extension == ".GEOM")
			shaders[shaderFile.m_Name].m_GeometryShaderLocation = shaderFile.m_Location;
		else
			allSuccessful = false;
	}

	for (const auto &shader : shaders) {
		bool successful = LoadShaderFromFile(shader.second.m_VertexShaderLocation, shader.second.m_FragmentShaderLocation, shader.second.m_GeometryShaderLocation);

		if (!successful)
			allSuccessful = false;
	}

	return allSuccessful;
}

std::shared_ptr<Shader> ResourceManager::LoadShader(const std::string &p_VertexShaderFile, const std::string &p_FragmentShaderFile, const std::string &p_GeometryShaderFile) {
	std::string shaderName = FileSystemHelper::GetNameFromFile(p_VertexShaderFile);

	auto iter = m_Shaders.find(shaderName);
	if (iter != m_Shaders.end()) {
		return iter->second;
	}

	if (LoadShaderFromFile(p_VertexShaderFile, p_FragmentShaderFile, p_GeometryShaderFile)) {
		return m_Shaders[shaderName];
	}

	return std::shared_ptr<Shader>(nullptr);
}

std::shared_ptr<Shader> ResourceManager::GetShader(const std::string &p_Name) {
	auto iter = m_Shaders.find(p_Name);
	if (iter != m_Shaders.end()) {
		return iter->second;
	}

	// Try to return the default, if the one being requested couldn't be found.
	auto defaultIter = m_Shaders.find("default");
	if (defaultIter != m_Shaders.end()) {
		return defaultIter->second;
	}

	return std::shared_ptr<Shader>(nullptr);
}

unsigned int ResourceManager::LoadOpenGLTexture(const std::string &p_FilePath) {
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char *data = stbi_load(p_FilePath.c_str(), &width, &height, &nrComponents, 0);
	if (data) {
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else {
		std::cout << "Texture failed to load at the path " << p_FilePath << std::endl;
	}
	stbi_image_free(data);

	return textureID;
}

unsigned int ResourceManager::LoadOpenGLCubemapTexture(const std::vector<std::string> &p_CubemapFaces) {
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int imageWidth, imageHeight, nrChannels;
	unsigned char *imageData;

	// Load in the textures - setting them to each face of the cube.
	for (unsigned int i = 0; i < p_CubemapFaces.size(); i++) {
		imageData = stbi_load(p_CubemapFaces[i].c_str(), &imageWidth, &imageHeight, &nrChannels, 0);

		if (imageData)
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
		else
			std::cerr << ("Cubemap texture failed to load at path: " + p_CubemapFaces[i]);

		stbi_image_free(imageData);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}

bool ResourceManager::LoadModelsFromFolder(const std::string &p_FolderName) {
	// Load any models, inside of a certain folder.
	std::vector<FileInformation> modelFiles = FileSystemHelper::GetFilesInFolder(p_FolderName);

	// Assimp doesn't like double back-slashes, for directory locations, so they must manually be changed to a forward slash.
	for (auto &modelFile : modelFiles) {
		while (true) {
			auto position = modelFile.m_Location.find("\\");

			if (position != std::string::npos)
				modelFile.m_Location.replace(position, 1, "/");
			else
				break;
		}
	}

	// Only try to load the model files, with the extensions (.obj, .dae, .fbx...), as Assimp can handle these file formats.
	FileSystemHelper::RetainRemoveFilesWithExtensions(modelFiles, { ".obj", ".dae", ".fbx", ".3ds", ".blend", ".ply", ".stl" });

	bool allLoadedCorrectly = false, loadedModelCorrectly = false;
	for (auto &modelFile : modelFiles) {
		loadedModelCorrectly = LoadModelFromFile(modelFile);

		if (!loadedModelCorrectly)
			allLoadedCorrectly = false;
	}

	return allLoadedCorrectly;
}

bool ResourceManager::LoadModelFromFile(const FileInformation &p_FileLocation) {
	// Load in a model, from a file.
	bool modelLoadedCorrectly = false;
	std::shared_ptr<Model> model = std::make_shared<Model>(p_FileLocation.m_Location, modelLoadedCorrectly);
	if (modelLoadedCorrectly) {
		m_Models.insert(std::pair<std::string, std::shared_ptr<Model>>(p_FileLocation.m_Name, model));
	}
	else
		m_UnsuccessfullyLoadedModels.push_back(p_FileLocation.m_Location);

	return modelLoadedCorrectly;
}

bool ResourceManager::LoadModelFromFile(std::string p_FileLocation) {
	// Load in a model, from a file.
	bool modelLoadedCorrectly = false;

	std::shared_ptr<Model> model = std::make_shared<Model>(p_FileLocation, modelLoadedCorrectly);
	if (modelLoadedCorrectly) {
		FileSystemHelper::RemoveFilePath(p_FileLocation);
		FileSystemHelper::RemoveFileExtension(p_FileLocation);
		m_Models.insert(std::pair<std::string, std::shared_ptr<Model>>(p_FileLocation, model));
	}
	else
		m_UnsuccessfullyLoadedModels.push_back(p_FileLocation);

	return modelLoadedCorrectly;
}

std::shared_ptr<Model> ResourceManager::GetModel(const std::string &p_ModelName) {
	std::map<std::string, std::shared_ptr<Model>>::iterator iter = m_Models.find(p_ModelName);
	if (iter != m_Models.end()) {
		return iter->second;
	}

	// If the model requested cannot be found, then return the default model (question mark).
	std::map<std::string, std::shared_ptr<Model>>::iterator iterToDefault = m_Models.find("default");
	if (iterToDefault != m_Models.end()) {
		return iterToDefault->second;
	}

	return std::shared_ptr<Model>(nullptr);
}