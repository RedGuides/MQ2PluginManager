/*
	MQ2PluginManager by ChatWithThisName was written based on MQ2MacroTool.dll by Dencelle.
	Large chunks of the window code was used for the creation. ChatWithThisName mostly fumbled
	around until things worked. There may be some residual code that is not even required by the
	plugin in here, but if it is, I'm unaware of it. Chances are some of these things I just deleted
	entirely and tried to run after a succesful compile only to crash and therefore was readded.
*/

#include <mq/Plugin.h>
#include "CPluginTree.h"
#include "imgui/ImGuiUtils.h"

PreSetup("MQ2PluginManager");
PLUGIN_VERSION(2019.0828);

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=//
CPluginTree* PluginTree = nullptr;
std::unordered_set<std::string> LoadedPlugins;
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=//

static void DrawPluginManager_MQSettingsPanel();
static void PluginManagerCommand(PlayerClient*, const char*);
static bool s_showWindow = true;
static void ImGui_ToggleWindow();

//=-=-=-=-=- Plugin members =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=//
PLUGIN_API void InitializePlugin()
{
	WriteChatColor("\aoLoading MQ2PluginManager...");
	WriteChatColor("\at/pluginman \ayToggles the PluginManager UI Window");
	AddCommand("/Pluginman", PluginManagerCommand, false, false, true);
	PluginTree = new CPluginTree();
	AddSettingsPanel("plugins/PluginManager", DrawPluginManager_MQSettingsPanel);
	s_showWindow = GetPrivateProfileBool("PluginManager", "ShowGui", true, INIFileName);

	// Initialize the LoadedPlugins set
	const std::vector<CPluginInfo*>& pluginList = PluginTree->GetCurrentPluginList();
	for (auto& plugin : pluginList)
	{
		const char* pluginName = plugin->GetName();
		if (mq::IsPluginLoaded(pluginName))
		{
			LoadedPlugins.insert(pluginName);
		}
	}
}

PLUGIN_API void ShutdownPlugin()
{
	if (PluginTree)
	{
		delete PluginTree;
		PluginTree = nullptr;
	}

	RemoveCommand("/Pluginman");
	RemoveSettingsPanel("plugins/PluginManager");
}

PLUGIN_API void OnLoadPlugin(const char* PluginName)
{
	// TODO: Update the plugin manager window
	LoadedPlugins.insert(PluginName);
}

PLUGIN_API void OnUnloadPlugin(const char* PluginName)
{
	// TODO: Update the plugin manager window
	LoadedPlugins.erase(PluginName);
}

static void PluginManagerCommand(PlayerClient*, const char*)
{
	ImGui_ToggleWindow();
}

static void DrawGUI()
{
	ImGui::TextColored(ImVec4(0.0f, 1.0f, 1.0f, 1.0f), "/pluginman");
	ImGui::SameLine();
	ImGui::Text("Toggles the GUI window.");
	ImGui::SeparatorText("Plugins");

	if (ImGui::BeginTable("PluginTable", 2))
	{
		const std::vector<CPluginInfo*>& pluginList = PluginTree->GetCurrentPluginList();
		for (const auto& plugin : pluginList)
		{
			const char* pluginName = plugin->GetName();

			// Skip drawing MQ2PluginManager in the table
			if (strcmp(pluginName, "MQ2PluginManager") == 0)
				continue;

			ImGui::TableNextColumn();
			ImGui::PushID(pluginName); // Ensure unique ID for each row

			// Check if the plugin is loaded
			bool isLoaded = LoadedPlugins.find(pluginName) != LoadedPlugins.end();

			// Create a checkbox for each plugin
			if (ImGui::Checkbox(pluginName, &isLoaded))
			{
				if (isLoaded)
				{
					mq::LoadPlugin(pluginName, true);

					WriteChatf("\atPlugin \ax[\ay%s\ax] \agLoaded!\ax", pluginName);
					LoadedPlugins.insert(pluginName);  // Update the set
				}
				else
				{
					mq::UnloadPlugin(pluginName, true);

					WriteChatf("\atPlugin \ax[\ay%s\ax] \arUnloaded!\ax", pluginName);
					LoadedPlugins.erase(pluginName);  // Update the set
				}
			}
			
			ImGui::PopID();
		
		}
		ImGui::EndTable();
	}
}

void DrawPluginManager_MQSettingsPanel()
{
	DrawGUI();
}

PLUGIN_API void OnUpdateImGui()
{
	if (!s_showWindow)
		return;

	ImGui::SetNextWindowSize(ImVec2(800, 440), ImGuiCond_FirstUseEver);
	if (ImGui::Begin("PluginManager##Gui", &s_showWindow, ImGuiWindowFlags_None))
	{
		DrawGUI();
	}
	ImGui::End();

	// save state when window is manually closed.
	if (!s_showWindow)
	{
		WritePrivateProfileBool("PluginManager", "ShowGui", s_showWindow, INIFileName);
	}
}

void ImGui_ToggleWindow()
{
	s_showWindow = !s_showWindow;
	WritePrivateProfileBool("PluginManager", "ShowGui", s_showWindow, INIFileName);
	WriteChatColor("\aoToggeling Plugin Manager UI...");
}
