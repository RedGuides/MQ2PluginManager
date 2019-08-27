/***************************************************************
* Plugin Info Class Definition                                  *
*                                                              *
* Holds information about a single Plugin file                  *
***************************************************************/
#pragma once
#include "../MQ2Plugin.h"
#include <vector>
using namespace std;

class CPluginInfo {
public:
	CPluginInfo(CPluginInfo *parent, char*name, bool isDirectory);
	~CPluginInfo();
	char* GetFolderPath();
	char* GetName();
	vector<CPluginInfo *> GetListItems();
	bool IsDirectory();
	bool IsRoot();
	char* GetDirectoryPath();
	CPluginInfo* GetParent();
	CPluginInfo* GetInfoForId(int id);
	char* GetDirectoryName();
	int GetLevel();
private:
	CPluginInfo *Parent;
	char FolderPath[MAX_STRING];
	char Name[MAX_STRING];
	char Description[MAX_STRING];
	char DirectoryName[MAX_STRING];
	int DirectoryLevel;
	bool directoryFlag;
	bool rootFlag;
	bool fileError;
	bool augmentedFile;
	vector<CPluginInfo *> SubNodes;
	void InitSubNodes();
};