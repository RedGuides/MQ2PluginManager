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
*/



#include <mq/Plugin.h>
#include "CPluginTree.h"
#include "CPluginToolWnd.h"
#include "MQ2PluginManager.h"

PreSetup("MQ2PluginManager");
PLUGIN_VERSION(2019.0828);

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=//
CPluginToolWnd* PluginWnd = nullptr;
CPluginTree* PluginTree = nullptr;
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=//

//=-=-=-=-=- Plugin members =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=//
PLUGIN_API void InitializePlugin()
{
	DebugSpewAlways("Initializing MQ2PluginManager");
	WriteChatColor("\aoLoading MQ2PluginManager v0.0.1 ...");
	AddCommand("/Pluginman", DoPluginTool, 0, 0, 1);
	AddXMLFile("MQUI_PluginManagerWnd.xml");
	PluginTree = new CPluginTree();
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
