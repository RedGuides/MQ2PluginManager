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

static MQPlugin* FindMQ2Plugin(const char* szLine)
{
	MQPlugin* pPlugin = pPlugins;
	while (pPlugin)
	{
		if (!_stricmp(const_cast<char*>(szLine), pPlugin->szFilename))
		{
			return pPlugin;
		}
		pPlugin = pPlugin->pNext;
	}
	return nullptr;
}


void DrawPluginManager_MQSettingsPanel() {
	ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "MQ2PluginManager");

	std::vector<CPluginInfo*> pluginList = PluginTree->GetCurrentPluginList();

	ImGui::SeparatorText("Plugins");

	if (ImGui::BeginTable("PluginTable", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg)) {
		int idx = 0;
		for (auto& plugin : pluginList) {
			const char* pluginName = plugin->GetName();
			bool isLoaded = FindMQ2Plugin(pluginName) != nullptr;
			ImVec4 textColor = isLoaded ? ImVec4(0.0f, 1.0f, 0.0f, 1.0f) : ImVec4(0.867f, 0.410f, 0.510f, 1.0f);

			if (idx == 0) {
				ImGui::TableNextRow();
			}

			ImGui::TableSetColumnIndex(idx);
			ImGui::PushID(pluginName); // Ensure unique ID for each row

			// Combine plugin name and toggle icon into one string
			std::string combinedLabel = std::string(isLoaded ? ICON_FA_TOGGLE_ON : ICON_FA_TOGGLE_OFF) + "\t" + (pluginName) ;

			// Create a selectable item with the combined string and color it
			if (ImGui::Selectable("", false)) {
				if (strcmp(pluginName, "MQ2PluginManager") == 0) {
					// Gracefully shutdown MQ2PluginManager
					ShutdownPlugin();
					char unloadCmd[MAX_STRING];
					sprintf_s(unloadCmd, "/plugin %s unload", pluginName);
					DoCommand(GetCharInfo()->pSpawn, const_cast<char*>(unloadCmd));
				}
				else {
					// Toggle other plugins state
					if (isLoaded) {
						char unloadCmd[MAX_STRING];
						sprintf_s(unloadCmd, "/plugin %s unload", pluginName);
						DoCommand(GetCharInfo()->pSpawn, const_cast<char*>(unloadCmd));
					}
					else {
						char loadCmd[MAX_STRING];
						sprintf_s(loadCmd, "/plugin %s", pluginName);
						DoCommand(GetCharInfo()->pSpawn, const_cast<char*>(loadCmd));
					}
				}
			}
			ImGui::SameLine();
			ImGui::TextColored(textColor, "%s", combinedLabel.c_str());

			ImGui::PopID();
			idx = (idx == 0) ? 1 : 0;
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
