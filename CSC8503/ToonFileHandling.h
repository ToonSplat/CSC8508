#pragma once
#include <fstream>

class ToonFileHandling
{
	private:
		std::string  m_FileName;
		std::fstream m_FileStream;

	public:
		ToonFileHandling(std::string fileName);
		~ToonFileHandling();
		bool WriteData(const std::string& dataString);
		bool ReadDataIn(std::string& dataString);

	private:
		bool CreateFile();
		bool IsFilePresent();
};