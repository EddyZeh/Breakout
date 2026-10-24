#pragma once

#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <map>
#include <string>

#include "../graphics/Shader.h"
#include "../graphics/Texture.h"
#include "path.h"

class ResourceManager {
public:
	// Resource storage
	static std::map<std::string, Shader> Shaders;
	static std::map<std::string, Texture> Textures;
	
	static Shader createShader(const std::string& name, bool hasGeomShader = false);
	static Shader createShader(const std::string& vs, const std::string& fs);
	static Shader createShader(const std::string& vs, const std::string& fs, const std::string& gs);

	static Shader& getShader(const std::string& name);

	static Texture createTexture(const std::string& name, bool alpha);
	static Texture& getTexure(const std::string& name);

	static void clear();

	static std::string getTexturePath(const std::string& name);

	static std::string getCubemapPath(const std::string& name);

	static std::string getModelPath(const std::string& name);

	static std::string getLevelPath(const std::string& name);
};


#endif // !RESOURCE_MANAGER_H

