/***************************************************************
* Plugin Tree Class Implementation                              *
*                                                              *
*                                                              *
***************************************************************/
#include "CPluginTree.h"
CPluginTree::CPluginTree()
{
	TreeRoot = new CPluginInfo(NULL, gszINIPath, true);
	CurrentNode = TreeRoot;
}
CPluginTree::~CPluginTree()
{
	if(TreeRoot)
	{
		delete TreeRoot;
		TreeRoot = 0;
	}
}
vector<CPluginInfo *> CPluginTree::GetCurrentPluginList()
{
	return CurrentNode->GetListItems();
}
CPluginInfo* CPluginTree::GetPluginInfo(int infoID)
{
	return CurrentNode->GetInfoForId(infoID);
}
void CPluginTree::SetCurrentParent(CPluginInfo* newParent)
{
	CurrentNode = newParent;
}
bool CPluginTree::UpOneLevel()
{
	if(CurrentNode->IsRoot())
	{
		return false;
	}
	CurrentNode = CurrentNode->GetParent();
	return true;
}
void CPluginTree::SetCurrentLevel(int level)
{
	if(level >= CurrentNode->GetLevel())
		return;
	CPluginInfo *newCurrent = CurrentNode;
	while(!newCurrent->IsRoot() && newCurrent->GetLevel() != level)
	{
		newCurrent = newCurrent->GetParent();
	}
	SetCurrentParent(newCurrent);
}
vector<char *> CPluginTree::GetCurrentDirectoryListing()
{
	vector<char *> result;
	CPluginInfo *info;
	info = CurrentNode;
	while(!info->IsRoot())
	{
		result.insert(result.begin(),info->GetDirectoryName());
		info = info->GetParent();
	}
	result.insert(result.begin(),info->GetDirectoryName());
	return result;
}