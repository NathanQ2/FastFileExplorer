#pragma once

#include "FileManagment/FileManager.h"
#include "FileManagment/Folder.h"
#include "FileManagment/File.h"

namespace FastFileExplorer
{
	/// <summary>
	/// The explorer window
	/// </summary>
	class Explorer
	{
	public:
		Explorer(std::string explorerName)
		{
			m_ExplorerName = explorerName;
		}

		void Ui();
	private:
		void DrawElements();

		FileManager m_FileManager;

		bool m_CalculateFolderSize = false;
		bool m_GoToDirPopupOpen = false;

		std::string m_ExplorerName;
	};
}