#include "Explorer.h"

#include "Walnut/UI/UI.h"

void FastFileExplorer::Explorer::Ui()
{
	ImGui::Begin(m_ExplorerName.c_str());

	DrawElements();

	ImGui::End();


	if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Space)) || m_GoToDirPopupOpen)
	{
		ImGui::OpenPopup("Got to directory");
		m_GoToDirPopupOpen = ImGui::BeginPopupModal("Got to directory", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
		if (m_GoToDirPopupOpen);
		{
			if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Escape))) m_GoToDirPopupOpen = false;
			char path[256] = {};
			strcpy(path, m_FileManager.GetCurrentPath().c_str());
			ImGui::SetKeyboardFocusHere();
			if (ImGui::InputText("Directory", path, sizeof(path), ImGuiInputTextFlags_EnterReturnsTrue))
			{
				m_FileManager.SetCurrentPath(path);
				m_GoToDirPopupOpen = false;
			}

			ImGui::EndPopup();
		}
	}
}

void FastFileExplorer::Explorer::DrawElements()
{
	ImGui::Checkbox("Calculate Folder Size", &m_CalculateFolderSize);
	char path[256] = {};
	strcpy(path, m_FileManager.GetCurrentPath().c_str());
	if (ImGui::InputText("Path", path, sizeof(path), ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue))
	{
		m_FileManager.SetCurrentPath(path);
	}

	ImGui::BeginTable(m_ExplorerName.c_str(), 2, ImGuiTableFlags_Borders | ImGuiTableFlags_ContextMenuInBody | ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable);

	ImGui::TableSetupColumn("Name");
	ImGui::TableSetupColumn("Size");
	ImGui::TableHeadersRow();

	std::shared_ptr<std::vector<FastFileExplorer::Folder>> folders = m_FileManager.GetFoldersInCurrentFolder();

	{
		ImGui::TableNextColumn();
		FastFileExplorer::Folder folder = FastFileExplorer::Folder("..\\", m_FileManager.GetCurrentPath() + "..\\", false);
		if (ImGui::Selectable(folder.GetFolderName().c_str(), false, ImGuiSelectableFlags_AllowDoubleClick | ImGuiSelectableFlags_SpanAllColumns) && ImGui::IsMouseDoubleClicked(0))
		{
			std::string folderName = folder.GetFolderName();
			//std::cout << folderName << " - Double-clicked" << std::endl;
			m_FileManager.SetCurrentPath(m_FileManager.GetCurrentPath() + folderName);
		}

		ImGui::TableNextColumn();
		if (m_CalculateFolderSize)
		{
			ImGui::Text(folder.GetSize().c_str());
		}
		else
			ImGui::Text("NA");
	}

	// For every folder
	for (int i = 0; i < folders->size(); i++)
	{
		ImGui::TableNextColumn();
		//if (ImGui::BeginPopupContextItem(("FolderContextMenu_" + folder.GetFolderName()).c_str()))
		//{
		//	// Add items to the context menu for this folder item
		//	if (ImGui::Selectable("Rename"))
		//	{
		//		// Handle "Rename" context menu item
		//		// Replace this with your custom behavior for renaming the folder item
		//	}
		//	if (ImGui::Selectable("Delete"))
		//	{
		//		// Handle "Delete" context menu item
		//		// Replace this with your custom behavior for deleting the folder item
		//	}
		//	ImGui::EndPopup();
		//}

		if (ImGui::Selectable((*folders)[i].GetFolderName().c_str(), false, ImGuiSelectableFlags_AllowDoubleClick | ImGuiSelectableFlags_SpanAllColumns) && ImGui::IsMouseDoubleClicked(0))
		{
			std::string folderName = (*folders)[i].GetFolderName();
			//std::cout << folderName << " - Double-clicked" << std::endl;
			std::string path = m_FileManager.GetCurrentPath() + folderName + "\\";
			m_FileManager.SetCurrentPath(path);
		}

		ImGui::TableNextColumn();
		if (m_CalculateFolderSize)
			ImGui::Text((*folders)[i].GetSize().c_str());
		else
			ImGui::Text("NA");
	}


	// For every file
	std::shared_ptr<std::vector<FastFileExplorer::File>> files = m_FileManager.GetFilesInCurrentFolder();
	for (int i = 0; i < files->size(); i++)
	{
		ImGui::TableNextColumn();
		if (ImGui::Selectable((*files)[i].GetFileName().c_str(), false, ImGuiSelectableFlags_AllowDoubleClick) && ImGui::IsMouseDoubleClicked(0))
		{
			std::string fileName = (*files)[i].GetFileName();
			//std::cout << (*files)[i].GetFileName() << " - Double-clicked" << std::endl;
			m_FileManager.OpenFileInDefaultApp((*files)[i].GetFilePath() + fileName);
		}


		ImGui::TableNextColumn();
		ImGui::Text((*files)[i].GetSize().c_str());
	}



	ImGui::EndTable();
}