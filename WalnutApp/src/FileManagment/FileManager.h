#pragma once

#include "File.h"
#include "Folder.h"
#include "../Utils.h"

#include <string>
#include <vector>

namespace FastFileExplorer
{
	class FileManager
	{
	public:
		const std::shared_ptr<std::vector<Folder>> GetFoldersInFolder(std::string folderPath);
		const std::shared_ptr<std::vector<File>> GetFilesInFolder(std::string folderPath);

		const std::shared_ptr<std::vector<Folder>> GetFoldersInCurrentFolder();
		const std::shared_ptr<std::vector<File>> GetFilesInCurrentFolder();

		const std::string GetCurrentPath();
		void SetCurrentPath(std::string path);
		bool OpenFileInDefaultApp(const std::string& filePath);
		Folder GetCurrentFolder();
	private:
		std::string m_CurrentPath = "C:\\Users\\";

		std::shared_ptr<std::vector<FastFileExplorer::File>> m_CachedFiles = std::make_shared<std::vector<FastFileExplorer::File>>();
		std::shared_ptr<std::vector<FastFileExplorer::Folder>> m_CachedFolders = std::make_shared<std::vector<FastFileExplorer::Folder>>();
		std::string m_CachedFoldersDir;
		std::string m_CachedFilesDir;

		bool m_FilesCached = false;
		bool m_FoldersCached = false;
	};
}