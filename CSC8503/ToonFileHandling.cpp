#include "ToonFileHandling.h"

ToonFileHandling::ToonFileHandling(std::string fileName)
{
	m_FileName = fileName;
}

ToonFileHandling::~ToonFileHandling()
{
	
}

bool ToonFileHandling::WriteData(char* dataString, std::ios_base::openmode mode)
{
	if (mode == std::ios::trunc) { std::remove(m_FileName.c_str()); }
	bool isFilePresent = IsFilePresent();
	m_FileStream.open(m_FileName, isFilePresent ? mode : std::ios::out);
	if (!m_FileStream.is_open()) { return false; }
	m_FileStream << dataString;
	m_FileStream.close();
	return true;
}

bool ToonFileHandling::ReadDataIn(std::string& dataString)
{
	std::string  singleLineString;
	m_FileStream.open(m_FileName);
	if (!m_FileStream.is_open()) { return false; }
	while (std::getline(m_FileStream, singleLineString))
	{
		dataString += singleLineString;
		dataString += '\n';
	}
	m_FileStream.close();
	return true;
}

bool ToonFileHandling::IsFilePresent()
{
	std::ifstream inputFileStream;
	inputFileStream.open(m_FileName);
	bool isFilePresent = inputFileStream.is_open();
	inputFileStream.close();
	return isFilePresent;
}