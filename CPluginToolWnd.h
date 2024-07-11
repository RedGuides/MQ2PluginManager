/***************************************************************
 * Plugin Tool Window Class                                    *
 *                                                             *
 * Window listing all available Plugins.                       *
 ***************************************************************/
#pragma once

#include "CPluginTree.h"

#include <mq/Plugin.h>

class CPluginToolWnd : public CCustomWnd
{
public:
	CPluginToolWnd(CPluginTree* tree);
	~CPluginToolWnd() override;

	int WndNotification(CXWnd* pWnd, unsigned int Message, void* pData) override;

	void RefreshPluginList();
	void ShowWnd();
	void HideWnd();
	void ToggleVisibility();

private:
	CPluginTree* PluginTree = nullptr;
	CListWnd* PluginListBox = nullptr;
	bool initialized = false;
	bool isWndActive = false;

	void SetPluginListItems();
	void InitListView();
};
