/*
	MQ2PluginManager by ChatWithThisName was written based on MQ2MacroTool.dll by Dencelle.
	Large chunks of the window code was used for the creation. ChatWithThisName mostly fumbled
	around until things worked. There may be some residual code that is not even required by the
	plugin in here, but if it is, I'm unaware of it. Chances are some of these things I just deleted
	entirely and tried to run after a succesful compile only to crash and therefore was readded.
*/

#include <mq/Plugin.h>
#include "CPluginTree.h"
#include "CPluginToolWnd.h"

PreSetup("MQ2PluginManager");
PLUGIN_VERSION(2019.0828);

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=//
CPluginToolWnd* PluginWnd = nullptr;
CPluginTree* PluginTree = nullptr;
std::unordered_set<std::string> LoadedPlugins;
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=//

static void DrawPluginManager_MQSettingsPanel();
static void PluginManagerCommand(PlayerClient*, const char*);

//=-=-=-=-=- Plugin members =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=//
PLUGIN_API void InitializePlugin()
{
	WriteChatColor("\aoLoading MQ2PluginManager v0.0.2 ...");
	AddCommand("/Pluginman", PluginManagerCommand, false, false, true);
	AddXMLFile("MQUI_PluginManagerWnd.xml");
	PluginTree = new CPluginTree();
	AddSettingsPanel("plugins/PluginManager", DrawPluginManager_MQSettingsPanel);

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
	if (PluginWnd)
	{
		delete PluginWnd;
		PluginWnd = nullptr;
	}

	if (PluginTree)
	{
		delete PluginTree;
		PluginTree = nullptr;
	}

	RemoveCommand("/Pluginman");
	RemoveXMLFile("MQUI_PluginManagerWnd.xml");
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

void CreatePluginWindow()
{
	if (PluginWnd)
	{
		return;
	}

	if (pSidlMgr->FindScreenPieceTemplate("PluginManagerWindow"))
	{
		PluginWnd = new CPluginToolWnd(PluginTree);
	}
}

void DestroyPluginWindow()
{
	if (PluginWnd)
	{
		delete PluginWnd;
		PluginWnd = nullptr;
	}
}

static void PluginManagerCommand(PlayerClient*, const char*)
{
	if (!PluginWnd)
	{
		CreatePluginWindow();
	}

	if (!PluginWnd)
	{
		WriteChatColor("/PluginMan: Could not initialize Plugin tool window.", USERCOLOR_DEFAULT);
	}
	else
	{
		PluginWnd->SetVisible(true);
	}
}

void DrawPluginManager_MQSettingsPanel()
{
	ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "MQ2PluginManager");
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

PLUGIN_API void OnCleanUI()
{
	DestroyPluginWindow();
}

PLUGIN_API void OnReloadUI()
{
	CreatePluginWindow();
}

PLUGIN_API void SetGameState(int GameState)
{
	if (GameState == GAMESTATE_INGAME && !PluginWnd)
	{
		CreatePluginWindow();
	}
}
