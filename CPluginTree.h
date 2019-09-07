/***************************************************************
* Plugin Tree Class Definition                                  *
*                                                              *
* All Plugins inside the Plugin folder and its subfolders        *
***************************************************************/
#pragma once
#include "../MQ2Plugin.h"
#include <vector>
#include "CPluginInfo.h"
using namespace std;
class CPluginTree {
public:
	CPluginTree();
	~CPluginTree();
	vector<CPluginInfo *> GetCurrentPluginList();
	CPluginInfo* GetPluginInfo(int infoID);
	void SetCurrentParent(CPluginInfo* newParent);
	bool UpOneLevel();
	vector<char *> GetCurrentDirectoryListing();
	void SetCurrentLevel(int level);
private:
	CPluginInfo *TreeRoot;
	CPluginInfo *CurrentNode;
};