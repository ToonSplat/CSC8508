/*
Part of Newcastle University's Game Engineering source code.

Use as you see fit!

Comments and queries to: richard-gordon.davison AT ncl.ac.uk
https://research.ncl.ac.uk/game/
*/
#pragma once
#include <string>

namespace NCL::Assets {
	extern void SetupAssetDirectory();
	extern bool ReadTextFile(const std::string& filepath, std::string& result);
	extern bool ReadBinaryFile(const std::string& filepath, char** into, size_t& size);

	extern std::string GetAssetRoot();
	extern std::string GetShaderDir();
	extern std::string GetMeshDir();
	extern std::string GetTextureDir();
	extern std::string GetSoundsDir();
	extern std::string GetFontsDir();
	extern std::string GetDataDir();
}