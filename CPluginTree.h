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

private:
	CPluginInfo* TreeRoot;
};
