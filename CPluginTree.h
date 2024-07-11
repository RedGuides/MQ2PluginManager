/***************************************************************
 * Plugin Tree Class Definition                                *
 *                                                             *
 * All Plugins inside the Plugin folder and its subfolders     *
 ***************************************************************/
#pragma once

#include "CPluginInfo.h"

#include <vector>

class CPluginTree
{
public:
	CPluginTree();
	~CPluginTree();

	const std::vector<CPluginInfo*>& GetCurrentPluginList() const { return TreeRoot->GetListItems(); }
	CPluginInfo* GetPluginInfo(int infoID) const { return TreeRoot->GetInfoForId(infoID); }

private:
	CPluginInfo* TreeRoot;
};
