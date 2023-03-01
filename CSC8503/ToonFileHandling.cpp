#include "ToonFileHandling.h"

ToonFileHandling::ToonFileHandling(std::string fileName)
{
	m_FileName = fileName;
}

ToonFileHandling::~ToonFileHandling()
{
	
}

bool ToonFileHandling::WriteData(const std::string& dataString)
{
	bool isFilePresent = IsFilePresent();
	std::fstream outputFileStream;
	outputFileStream.open(m_FileName, isFilePresent ? std::ios_base::app : std::ios::out);
	if (!outputFileStream.is_open()) { return false; }
	outputFileStream << dataString;
	outputFileStream.close();
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

bool ToonFileHandling::CreateFile()
{
	return false;
}

bool ToonFileHandling::IsFilePresent()
{
	std::ifstream inputFileStream;
	inputFileStream.open(m_FileName);
	bool isFilePresent = inputFileStream.is_open();
	inputFileStream.close();
	return isFilePresent;
}