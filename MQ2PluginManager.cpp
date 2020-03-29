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



#include "../MQ2Plugin.h"
#include "CPluginTree.h"
#include "CPluginToolWnd.h"
#include "MQ2PluginManager.h"
using namespace std;


PreSetup("MQ2PluginManager");

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=//
CPluginToolWnd* PluginWnd = 0;
CPluginTree* PluginTree = 0;
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=//

//=-=-=-=-=- Plugin members =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=//
PLUGIN_API VOID InitializePlugin(VOID)
{
	DebugSpewAlways("Initializing MQ2PluginManager");
	WriteChatColor("\aoLoading MQ2PluginManager v0.0.1 ...");
	AddCommand("/Pluginman", DoPluginTool, 0, 0, 1);
	AddXMLFile("MQUI_PluginManagerWnd.xml");
	PluginTree = new CPluginTree();
}

PLUGIN_API VOID ShutdownPlugin(VOID)
{
	DebugSpewAlways("Shutting down MQ2PluginManager");
	if (PluginWnd)
	{
		delete PluginWnd;
		PluginWnd = 0;
	}
	if (PluginTree)
	{
		delete PluginTree;
		PluginTree = 0;
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
		return;
	}
}
void DestroyPluginWindow()
{
	if (PluginWnd)
	{
		delete PluginWnd;
		PluginWnd = 0;
	}
}
void DoPluginTool(PSPAWNINFO pChar, PCHAR szLine)
{
	char szTmp[MAX_STRING] = { 0 };
	if (!PluginWnd)
	{
		CreatePluginWindow();
	}
	if (!PluginWnd)
	{
		WriteChatColor("/PluginMan: Could not initialize Plugin tool window.", USERCOLOR_DEFAULT);
		return;
	}
	PluginWnd->SetVisible(1);
}
// Called after entering a new zone
PLUGIN_API VOID OnZoned(VOID)
{
	DebugSpewAlways("MQ2PluginManager::OnZoned()");
}

// Called once directly before shutdown of the new ui system, and also
// every time the game calls CDisplay::CleanGameUI()
PLUGIN_API VOID OnCleanUI(VOID)
{
	DebugSpewAlways("MQ2PluginManager::OnCleanUI()");
	DestroyPluginWindow();
}

// Called once directly after the game ui is reloaded, after issuing /loadskin
PLUGIN_API VOID OnReloadUI(VOID)
{
	DebugSpewAlways("MQ2PluginManager::OnReloadUI()");
	CreatePluginWindow();
}

// Called once directly after initialization, and then every time the gamestate changes
PLUGIN_API VOID SetGameState(DWORD GameState)
{
	DebugSpewAlways("MQ2PluginManager::SetGameState()");
	if (GameState == GAMESTATE_INGAME && !PluginWnd)
	{
		CreatePluginWindow();
	}
}
// This is called every time MQ pulses
PLUGIN_API VOID OnPulse(VOID)
{
	// DONT leave in this debugspew, even if you leave in all the others
	//DebugSpewAlways("MQ2PluginManager::OnPulse()");
}

