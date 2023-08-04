#include "FileManager.h"

#include <filesystem>
#include <iostream>
#include <Windows.h>


namespace FastFileExplorer
{
    const const std::shared_ptr<std::vector<Folder>> FileManager::GetFoldersInFolder(std::string folderPath)
    {
        if (m_CachedFoldersDir == folderPath)
        {
            m_FoldersCached = true;
            return m_CachedFolders;
        }

        m_FoldersCached = false;

        namespace fs = std::filesystem;

        m_CachedFolders->clear();

        for (const auto& entry : fs::directory_iterator(folderPath)) {
            if (fs::is_directory(entry)) {
                std::string folderName = entry.path().filename().string();

                //std::cout << folderName << std::endl;

                m_CachedFolders->push_back(Folder(folderName, folderPath));
            }
        }

        m_CachedFoldersDir = folderPath;

        return m_CachedFolders;
    }

    const std::shared_ptr<std::vector<File>> FileManager::GetFilesInFolder(std::string filePath)
    {
        if (m_CachedFilesDir == filePath)
        {
            m_FilesCached = true;
            return m_CachedFiles;
        }

        m_FilesCached = false;
        if (m_CachedFiles->size() != 0)
            m_CachedFiles->clear();

        namespace fs = std::filesystem;

        for (const auto& entry : fs::directory_iterator(filePath)) {
            if (fs::is_regular_file(entry)) {
                std::string fileName = entry.path().filename().string();

                //std::cout << fileName << std::endl;

                m_CachedFiles->push_back(File(fileName, filePath));
            }
        }

        m_CachedFilesDir = filePath;

        return m_CachedFiles;
    }


    const std::shared_ptr<std::vector<Folder>> FileManager::GetFoldersInCurrentFolder()
    {
        
        if (m_FoldersCached)
			return m_CachedFolders;

        return GetFoldersInFolder(m_CurrentPath);
    }

    const std::shared_ptr<std::vector<File>> FileManager::GetFilesInCurrentFolder()
    {
        if (m_FilesCached)
            return m_CachedFiles;

        return GetFilesInFolder(m_CurrentPath);
    }
    const std::string FileManager::GetCurrentPath()
    {
        return m_CurrentPath;
    }
    void FileManager::SetCurrentPath(std::string path)
    {
        namespace fs = std::filesystem;
        if(fs::is_directory(path))
        {
            m_CurrentPath = FormatDirectory(path); 
            m_FilesCached = false;
            m_FoldersCached = false;
        }

    }
    bool FileManager::OpenFileInDefaultApp(const std::string& filePath)
    {
        HINSTANCE result = ShellExecuteA(nullptr, "open", filePath.c_str(), nullptr, nullptr, SW_SHOWNORMAL);
        return (intptr_t)result > 32; // Check if the operation was successful
    }

    Folder FileManager::GetCurrentFolder()
    {
        std::string folderName = FastFileExplorer::SplitString(m_CurrentPath, '\\').back();
        std::string folderPath = "";

        //remove the folder name from the path
        for (int i = 0; i < m_CurrentPath.length() - folderName.length() - 1; i++)
            folderPath.push_back((m_CurrentPath)[i]);

        //std::cout << "PATH: " << folderPath << std::endl;

        return Folder(folderName, folderPath);
    }
    std::string FileManager::FormatDirectory(std::string path)
    {
        // simplify the input path by making sure there are no "..\" in the path string
        std::string formattedPath;
        bool upDir = false;
        int slashNum = 0;
        for (int i = 0; i < path.length(); i++)
        {
            if (path[i] == '\\')
                slashNum++;

            if (path[i] == '.' && path[i + 1] == '.' && path[i + 2] == '\\')
            {
                upDir = true;
                continue;
			}
        }

        if (!upDir)
            return path;

        int slashNum2 = 0;
        for (int i = 0; i < path.length(); i++)
        {
            if (path[i] == '\\')
                slashNum2++;

            if (slashNum - 2 == slashNum2 )
            {
                break;
            }

            formattedPath.push_back(path[i]);
        }

        formattedPath.push_back('\\');

        std::cout << "Formatted Path: " << formattedPath << std::endl;
        return formattedPath;
    }
}