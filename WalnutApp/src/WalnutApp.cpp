#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"

#include "Walnut/Image.h"
#include "Walnut/UI/UI.h"

#include <iostream>

#include "Explorer.h"


class AppLayer : public Walnut::Layer
{
public:
	virtual void OnUIRender() override
	{
		for (int i = 0; i < m_Explorers.size(); i++)
		{
			m_Explorers[i].Ui();
		}
		UI_DrawAboutModal();
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

	void NewExplorer()
	{
		m_Explorers.push_back(FastFileExplorer::Explorer("Window" + std::to_string(m_Explorers.size())));
	}
private:
	bool m_AboutModalOpen = false;

	std::vector<FastFileExplorer::Explorer> m_Explorers = {};
};

Walnut::Application* Walnut::CreateApplication(int argc, char** argv)
{
	Walnut::ApplicationSpecification spec;
	spec.Name = "Fast File Explorer";
	spec.CustomTitlebar = true;
	spec.CenterWindow = true;
	spec.Width /= 2;
	spec.Height /= 2;

	Walnut::Application* app = new Walnut::Application(spec);
	std::shared_ptr<AppLayer> appLayer = std::make_shared<AppLayer>();
	app->PushLayer(appLayer);
	app->SetMenubarCallback([app, appLayer]()
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
				appLayer->ShowAboutModal();
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Window"))
		{
			if (ImGui::MenuItem("New Tab"))
			{
				appLayer->NewExplorer();
			}
			ImGui::EndMenu();
		}
	});
	return app;
}