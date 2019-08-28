/***************************************************************
* Plugin Tool Window Class                                      *
*                                                              *
* Window listing all available Plugins.                         *
***************************************************************/
#pragma once
#include "../MQ2Plugin.h"
#include <vector>
#include "CPluginInfo.h"
#include "CPluginTree.h"

class CPluginToolWnd : public CCustomWnd {
public:
	CPluginToolWnd(CPluginTree *tree);
	~CPluginToolWnd();
	void RefreshPluginList();
	void ShowWnd();
	void HideWnd();
	bool IsActive();
	int WndNotification(CXWnd *pWnd, unsigned int Message, void *unknown);
	void ToggleVisibility();
private:
	CPluginTree *PluginTree;
	CListWnd *PluginListBox;
	bool validXML;
	bool initialized;
	bool isWndActive;
	bool checkXML();
	void SetPluginListItems();
	void InitListView();
};

static PMQPLUGIN FindMQ2Plugin(PCHAR szLine);