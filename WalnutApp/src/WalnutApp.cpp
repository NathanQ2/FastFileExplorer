#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"

#include "Walnut/Image.h"
#include "Walnut/UI/UI.h"

#include <iostream>

#include "FileManagment/FileManager.h"
#include "FileManagment/Folder.h"
#include "FileManagment/File.h"

class ExampleLayer : public Walnut::Layer
{
public:
	virtual void OnUIRender() override
	{
		ImGui::Begin("Window1");

		DrawElements();

		ImGui::End();

		if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Space)) || m_GoToDirPopupOpen)
		{
			ImGui::OpenPopup("Got to directory");
			m_GoToDirPopupOpen = ImGui::BeginPopupModal("Got to directory", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
			if (m_GoToDirPopupOpen);
			{
				char path[256] = {};
				strcpy(path, m_FileManager.GetCurrentPath().c_str());
				ImGui::SetKeyboardFocusHere();
				if (ImGui::InputText("Directory", path, sizeof(path), ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
				{
					m_FileManager.SetCurrentPath(path);
					m_GoToDirPopupOpen = false;
				}

				ImGui::EndPopup();
			}
		}

		UI_DrawAboutModal();
	}

	void DrawElements()
	{
		ImGui::Checkbox("Calculate Folder Size", &m_CalculateFolderSize);
		char path[256] = {};
		strcpy(path, m_FileManager.GetCurrentPath().c_str());
		if (ImGui::InputText("Path", path, sizeof(path), ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue))
		{
			m_FileManager.SetCurrentPath(path);
		}
		ImGui::BeginTable("Table", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_ContextMenuInBody | ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Sortable);

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
				std::cout << folderName << " - Double-clicked" << std::endl;
				m_FileManager.SetCurrentPath(m_FileManager.GetCurrentPath() + "\\" + folderName + "\\");
			}

			ImGui::TableNextColumn();
			if (m_CalculateFolderSize)
			{
				ImGui::Text(folder.GetSize().c_str());
			}
			else
				ImGui::Text("0b");
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
				std::cout << folderName << " - Double-clicked" << std::endl;
				m_FileManager.SetCurrentPath(m_FileManager.GetCurrentPath() + "\\" + folderName + "\\");
			}

			ImGui::TableNextColumn();
			if (m_CalculateFolderSize)
				ImGui::Text((*folders)[i].GetSize().c_str());
			else
				ImGui::Text("0b");
		}


		// For every file
		std::shared_ptr<std::vector<FastFileExplorer::File>> files = m_FileManager.GetFilesInCurrentFolder();
		for (int i = 0; i < files->size(); i++)
		{
			ImGui::TableNextColumn();
			if (ImGui::Selectable((*files)[i].GetFileName().c_str(), false, ImGuiSelectableFlags_AllowDoubleClick) && ImGui::IsMouseDoubleClicked(0))
			{
				std::string fileName = (*files)[i].GetFileName();
				std::cout << (*files)[i].GetFileName() << " - Double-clicked" << std::endl;
				m_FileManager.OpenFileInDefaultApp((*files)[i].GetFilePath() + fileName);
			}


			ImGui::TableNextColumn();
			ImGui::Text((*files)[i].GetSize().c_str());
		}



		ImGui::EndTable();
	}

	void UI_DrawAboutModal()
	{
		if (!m_AboutModalOpen)
			return;

		ImGui::OpenPopup("About");
		m_AboutModalOpen = ImGui::BeginPopupModal("About", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
		if (m_AboutModalOpen)
		{
			auto image = Walnut::Application::Get().GetApplicationIcon();
			ImGui::Image(image->GetDescriptorSet(), { 48, 48 });

			ImGui::SameLine();
			Walnut::UI::ShiftCursorX(20.0f);

			ImGui::BeginGroup();
			ImGui::Text("Walnut application framework");
			ImGui::Text("by Studio Cherno.");
			ImGui::EndGroup();

			if (Walnut::UI::ButtonCentered("Close"))
			{
				m_AboutModalOpen = false;
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}
	}

	void ShowAboutModal()
	{
		m_AboutModalOpen = true;
	}
private:
	bool m_AboutModalOpen = false;
	bool m_CalculateFolderSize = false;
	bool m_GoToDirPopupOpen = false;

	FastFileExplorer::FileManager m_FileManager;
};

Walnut::Application* Walnut::CreateApplication(int argc, char** argv)
{
	Walnut::ApplicationSpecification spec;
	spec.Name = "Walnut Example";
	spec.CustomTitlebar = true;

	Walnut::Application* app = new Walnut::Application(spec);
	std::shared_ptr<ExampleLayer> exampleLayer = std::make_shared<ExampleLayer>();
	app->PushLayer(exampleLayer);
	app->SetMenubarCallback([app, exampleLayer]()
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Exit"))
				{
					app->Close();
				}
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Help"))
			{
				if (ImGui::MenuItem("About"))
				{
					exampleLayer->ShowAboutModal();
				}
				ImGui::EndMenu();
			}
		});
	return app;
}