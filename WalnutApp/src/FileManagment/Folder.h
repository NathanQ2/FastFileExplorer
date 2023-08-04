#pragma once

#include <string>

#include <iostream>

#include <filesystem>

#include "../Utils.h"

namespace FastFileExplorer
{
	class Folder
	{
	public:
		Folder(std::string folderName, std::string folderPath, bool computeSize = true)
			: m_FolderName(folderName), m_ComputeSize(computeSize)
		{
			if (folderPath[folderPath.size() - 1] != '\\')
				m_FolderPath = folderPath + "\\";
			else
				m_FolderPath = folderPath;
		}
		std::string GetFolderName()
		{
			return m_FolderName;
		}
		std::string GetFilePath()
		{
			return m_FolderPath;
		}
		std::string GetSize(bool ignoreCache = false)
		{
			if (!m_ComputeSize)
				return "";

			if (ignoreCache)
			{
				m_CachedSize = false;
				m_Size = "";
			}

			if (m_CachedSize)
				return m_Size;

			std::cout << "no cached folder size for folder \"" << m_FolderName << "\" creating one..." << std::endl;
			
			namespace fs = std::filesystem;

			std::uintmax_t totalSize = 0;

			if (fs::is_directory(m_FolderPath + m_FolderName))
			{
				for (const auto& entry : fs::recursive_directory_iterator(m_FolderPath + m_FolderName)) {
					if (fs::is_regular_file(entry)) {
						totalSize += fs::file_size(entry);
					}
				}
			}
			else
				std::cout << m_FolderPath + m_FolderName << " is not a directory" << std::endl;

			m_Size = ParseSize(totalSize);

			m_CachedSize = true;
			std::cout << "Completed." << std::endl;
			return m_Size;
		}
	private:

		std::string m_FolderName;
		std::string m_FolderPath;
		std::string m_Size;
		bool m_CachedSize = false;
		bool m_ComputeSize = true;
	};
}