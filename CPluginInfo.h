/***************************************************************
 * Plugin Info Class Definition                                *
 *                                                             *
 * Holds information about a single Plugin file                *
 ***************************************************************/
#pragma once

#include <string>
#include <vector>

class CPluginInfo
{
public:
	CPluginInfo(CPluginInfo* parent, std::string_view name, bool isDirectory);
	~CPluginInfo();

	const char* GetFolderPath() const { return FolderPath.c_str(); }
	const char* GetName() const { return Name.c_str(); }
	const std::vector<CPluginInfo*>& GetListItems() const { return SubNodes; }
	bool IsDirectory() const { return directoryFlag; }
	bool IsRoot() const { return rootFlag; }
	const char* GetDirectoryPath() const { return FolderPath.c_str(); }
	CPluginInfo* GetParent() const { return Parent; }
	const char* GetDirectoryName() const { return DirectoryName.c_str(); }
	int GetLevel() const { return DirectoryLevel; };

	CPluginInfo* GetInfoForId(int id);

private:
	void InitSubNodes();

	CPluginInfo* Parent;
	std::string FolderPath;
	std::string Name;
	std::string Description;
	std::string DirectoryName;
	int DirectoryLevel = 0;
	bool directoryFlag;
	bool rootFlag;
	std::vector<CPluginInfo*> SubNodes;
};
