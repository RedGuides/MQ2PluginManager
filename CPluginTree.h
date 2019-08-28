/***************************************************************
* Plugin Tree Class Definition                                  *
*                                                              *
* All Plugins inside the Plugin folder and its subfolders        *
***************************************************************/
#pragma once
#include "../MQ2Plugin.h"
#include "CPluginInfo.h"
#include <vector>


class CPluginTree {
public:
	CPluginTree();
	~CPluginTree();
	std::vector<CPluginInfo *> GetCurrentPluginList();
	CPluginInfo* GetPluginInfo(int infoID);
	void SetCurrentParent(CPluginInfo* newParent);
	bool UpOneLevel();
	std::vector<char *> GetCurrentDirectoryListing();
	void SetCurrentLevel(int level);

private:
	CPluginInfo *TreeRoot;
	CPluginInfo *CurrentNode;
};