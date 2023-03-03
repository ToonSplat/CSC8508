#pragma once
#include <fstream>
#include <filesystem>
#include <stdlib.h>
#include <stdio.h>

class ToonFileHandling
{
	private:
		std::string  m_FileName;
		std::fstream m_FileStream;

	public:
		ToonFileHandling(std::string fileName);
		~ToonFileHandling();
		bool WriteData(char* dataString, std::ios_base::openmode mode = std::ios_base::app);
		bool ReadDataIn(std::string& dataString);

	private:
		bool IsFilePresent();
};