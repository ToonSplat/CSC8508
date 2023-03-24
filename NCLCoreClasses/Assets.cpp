#include "Assets.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <Windows.h>
#include <tchar.h>

using namespace NCL;

std::string ASSETROOT;
std::string SHADERDIR;
std::string MESHDIR;
std::string TEXTUREDIR;
std::string SOUNDSDIR;
std::string FONTSDIR;
std::string DATADIRECTORY; // DATADIR is an existing type apparently!

void Assets::SetupAssetDirectory() {
#ifndef _DEBUG
	ASSETROOT = ASSETROOTLOCATION;
	SHADERDIR = ASSETROOT + "Shaders/";
	MESHDIR = ASSETROOT + "Meshes/";
	TEXTUREDIR = ASSETROOT + "Textures/";
	SOUNDSDIR = ASSETROOT + "Sounds/";
	FONTSDIR = ASSETROOT + "Fonts/";
	DATADIRECTORY = ASSETROOT + "Data/";
#else
	// This is a hacky solution that will work in time... should do it properly instead but the file path strings are quite different (/ vs \\)
	std::fstream file(ASSETROOTLOCATION + std::string("Data/ItemsToLoad.csv"));
	if (file.is_open()) {
		// If there is a build version just use that directory for now
		ASSETROOT = ASSETROOTLOCATION;
		SHADERDIR = ASSETROOT + "Shaders/";
		MESHDIR = ASSETROOT + "Meshes/";
		TEXTUREDIR = ASSETROOT + "Textures/";
		SOUNDSDIR = ASSETROOT + "Sounds/";
		FONTSDIR = ASSETROOT + "Fonts/";
		DATADIRECTORY = ASSETROOT + "Data/";
		file.close();
		return;
	}


	TCHAR exePath[MAX_PATH];
	if (GetModuleFileName(NULL, exePath, MAX_PATH))
		std::wcout << exePath << std::endl;

	TCHAR* extension = _tcsrchr(exePath, _T('\\'));
	_tcsicmp(extension, _T("/"));
	size_t length = extension - exePath;

	TCHAR newPath[MAX_PATH];

	_tcsncpy_s(newPath, exePath, length);

	length = _tcslen(newPath);
	ASSETROOT = newPath;
	ASSETROOT += "\\Assets\\";
	SHADERDIR = ASSETROOT + "Shaders\\";
	MESHDIR = ASSETROOT + "Meshes\\";
	TEXTUREDIR = ASSETROOT + "Textures\\";
	SOUNDSDIR = ASSETROOT + "Sounds\\";
	FONTSDIR = ASSETROOT + "Fonts\\";
	DATADIRECTORY = ASSETROOT + "Data\\";
#endif
}

bool Assets::ReadTextFile(const std::string &filepath, std::string& result) {
	std::ifstream file(filepath, std::ios::in);
	if (file) {
		std::ostringstream stream;

		stream << file.rdbuf();

		result = stream.str();

		return true;
	}
	else {
		std::cout << __FUNCTION__ << " can't read file " << filepath << "\n";
		return false;
	}
}

bool	Assets::ReadBinaryFile(const std::string& filename, char** into, size_t& size) {
	std::ifstream file(filename, std::ios::binary);

	if (!file) {
		return false;
	}

	file.seekg(0, std::ios_base::end);

	std::streamoff filesize = file.tellg();

	file.seekg(0, std::ios_base::beg);

	char* data = new char[(unsigned int)filesize];

	file.read(data, filesize);

	file.close();

	*into = data;
	size = filesize;

	return data == NULL ? true : false;
}

std::string Assets::GetAssetRoot() {
	return ASSETROOT;
}

std::string Assets::GetShaderDir() {
	return SHADERDIR;
}

std::string Assets::GetMeshDir() {
	return MESHDIR;
}

std::string Assets::GetTextureDir() {
	return TEXTUREDIR;
}

std::string Assets::GetSoundsDir() {
	return SOUNDSDIR;
}

std::string Assets::GetFontsDir() {
	return FONTSDIR;
}

std::string Assets::GetDataDir() {
	return DATADIRECTORY;
}