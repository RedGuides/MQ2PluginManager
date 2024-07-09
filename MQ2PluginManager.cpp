// MQ2PluginManager.cpp : Defines the entry point for the DLL application.
//

// PLUGIN_API is only to be used for callbacks.  All existing callbacks at this time
// are shown below. Remove the ones your plugin does not use.  Always use Initialize
// and Shutdown for setup and cleanup, do NOT do it in DllMain.

/*
	MQ2PluginManager by ChatWithThisName was written based on MQ2MacroTool.dll by Dencelle.
	Large chunks of the window code was used for the creation. ChatWithThisName mostly fumbled
	around until things worked. There may be some residual code that is not even required by the
	plugin in here, but if it is, I'm unaware of it. Chances are some of these things I just deleted
	entirely and tried to run after a succesful compile only to crash and therefore was readded.

	History -
		MQ2PluginManager created - 08/06/2019
		Added ImGui Version of window to MQSettings 7/6/2024 ~ Grimmier
*/



#include <mq/Plugin.h>
#include "CPluginTree.h"
#include "CPluginToolWnd.h"
#include "MQ2PluginManager.h"
#include "imgui/fonts/IconsFontAwesome.h"

PreSetup("MQ2PluginManager");
PLUGIN_VERSION(2019.0828);

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=//
CPluginToolWnd* PluginWnd = nullptr;
CPluginTree* PluginTree = nullptr;
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=//

void DrawPluginManager_MQSettingsPanel();

//=-=-=-=-=- Plugin members =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=//
PLUGIN_API void InitializePlugin()
{
	DebugSpewAlways("Initializing MQ2PluginManager");
	WriteChatColor("\aoLoading MQ2PluginManager v0.0.2 ...");
	AddCommand("/Pluginman", DoPluginTool, 0, 0, 1);
	AddXMLFile("MQUI_PluginManagerWnd.xml");
	PluginTree = new CPluginTree();
	AddSettingsPanel("plugins/PluginManager", DrawPluginManager_MQSettingsPanel);
}

PLUGIN_API void ShutdownPlugin()
{
	DebugSpewAlways("Shutting down MQ2PluginManager");
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
void DoPluginTool(PSPAWNINFO pChar, PCHAR szLine)
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
	// Toggle for MQ2PluginManager itself
	if (ImGui::Selectable("", false))
	{
		// Gracefully shutdown MQ2PluginManager
		ShutdownPlugin();
		WriteChatf("\atPlugin Manager\ax \arUnloading!\ax");
		// Trying to close ourself with mq:UnloadPlugin causes a crash, so we issue the command for this one.
		DoCommand(GetCharInfo()->pSpawn, "/plugin MQ2PluginManager unload");
		return;
	}
	ImGui::SameLine();
	ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "%s\tMQ2PluginManager", ICON_FA_TOGGLE_ON);

	ImGui::SeparatorText("Plugins");

	std::vector<CPluginInfo*> pluginList = PluginTree->GetCurrentPluginList();

	if (ImGui::BeginTable("PluginTable", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg))
	{
		for (auto& plugin : pluginList)
		{
			const char* pluginName = plugin->GetName();

			// Skip drawing MQ2PluginManager in the table
			if (strcmp(pluginName, "MQ2PluginManager") == 0)
				continue;

			bool isLoaded = mq::IsPluginLoaded(pluginName);
			ImVec4 textColor = isLoaded ? ImVec4(0.0f, 1.0f, 0.0f, 1.0f) : ImVec4(0.867f, 0.410f, 0.510f, 1.0f);

			ImGui::TableNextColumn();
			ImGui::PushID(pluginName); // Ensure unique ID for each row

			// Create a selectable item with the combined string and color it
			if (ImGui::Selectable("", false))
			{
				if (isLoaded)
				{
					mq::UnloadPlugin(pluginName);
					WriteChatf("\atPlugin \ax[\ay%s\ax\] \arUnloaded!\ax", pluginName);
				}
				else
				{
					mq::LoadPlugin(pluginName);
					WriteChatf("\atPlugin \ax[\ay%s\ax\] \agLoaded!\ax", pluginName);
				}
			}
			ImGui::SameLine();
			// Display Toggle and Plugin Name, Colored highlight state.
			ImGui::TextColored(textColor, "%s", fmt::format("{} {}", isLoaded ? ICON_FA_TOGGLE_ON : ICON_FA_TOGGLE_OFF, pluginName).c_str());
			ImGui::PopID();
		}
		ImGui::EndTable();
	}
}

PLUGIN_API void OnCleanUI()
{
	DebugSpewAlways("MQ2PluginManager::OnCleanUI()");
	DestroyPluginWindow();
}

PLUGIN_API void OnReloadUI()
{
	DebugSpewAlways("MQ2PluginManager::OnReloadUI()");
	CreatePluginWindow();
}

PLUGIN_API void SetGameState(int GameState)
{
	DebugSpewAlways("MQ2PluginManager::SetGameState()");
	if (GameState == GAMESTATE_INGAME && !PluginWnd)
	{
		CreatePluginWindow();
	}
}
