#pragma once

#include <string>

#include <fstream>

#include "../Utils.h"

namespace FastFileExplorer
{
	class File
	{
	public:
		File(std::string fileName, std::string filePath)
			: m_FileName(fileName), m_FilePath(filePath)
		{
			std::ifstream file = std::ifstream(filePath + "\\" + fileName, std::ios::binary | std::ios::ate);

			std::uintmax_t fileSizeInBytes{};

			if (file.is_open())
			{
				std::streampos fileSize = file.tellg();
				file.close();

				if (fileSize != -1)
				{
					fileSizeInBytes = static_cast<std::uintmax_t>(fileSize);
				}
			}
			else {
				m_Size = "0b";

				return;
			}

			m_Size = FastFileExplorer::ParseSize(fileSizeInBytes);
		}
		std::string GetFileName()
		{
			return m_FileName;
		}
		std::string GetFilePath()
		{
			return m_FilePath;
		}
		std::string GetSize()
		{
			return m_Size;
		}
	private:
		std::string m_FileName;
		std::string m_FilePath;
		std::string m_Size;
	};
}