/***************************************************************
 * Plugin Tree Class Implementation                            *
 *                                                             *
 *                                                             *
 ***************************************************************/
#include "CPluginTree.h"

#include <mq/Plugin.h>

CPluginTree::CPluginTree()
{
	TreeRoot = new CPluginInfo(nullptr, gPathPlugins, true);
}

CPluginTree::~CPluginTree()
{
	if (TreeRoot)
	{
		delete TreeRoot;
		TreeRoot = nullptr;
	}
}
