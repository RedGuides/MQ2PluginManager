/***************************************************************
 * Plugin Info Class Implementation                            *
 *                                                             *
 *                                                             *
 ***************************************************************/
#include "CPluginInfo.h"

#include <fmt/format.h>
#include <mq/Plugin.h>

CPluginInfo::CPluginInfo(CPluginInfo* parent, std::string_view name, bool isDirectory)
	: Parent(parent)
	, Name(name)
	, directoryFlag(isDirectory)
	, rootFlag(parent == nullptr)
{
	if (rootFlag)
	{
		FolderPath = gPathPlugins;

		InitSubNodes();
	}
	else
	{
		if (directoryFlag)
		{
			DirectoryLevel = Parent->GetLevel() + 1;
			FolderPath = fmt::format("{}\\{}", Parent->GetFolderPath(), Name);

			InitSubNodes();
		}
		else
		{
			DirectoryLevel = Parent->GetLevel();
			FolderPath = Parent->GetFolderPath();
		}
	}

	std::string_view dir = FolderPath;
	DirectoryName = dir.substr(dir.find_last_of("\\") + 1);
}

CPluginInfo::~CPluginInfo()
{
	for (size_t i = 0; i < SubNodes.size(); i++)
	{
		delete SubNodes[i];
	}
	SubNodes.clear();
}

CPluginInfo* CPluginInfo::GetInfoForId(int id)
{
	if (id >= 0 && id < SubNodes.size())
	{
		return SubNodes[id];
	}

	return nullptr;
}

void CPluginInfo::InitSubNodes()
{
	if (!directoryFlag)
	{
		return;
	}

	CHAR szFilename[MAX_PATH] = { 0 };
	sprintf_s(szFilename, "%s\\*.*", GetFolderPath());

	WIN32_FIND_DATA FileData;
	HANDLE hSearch = FindFirstFile(szFilename, &FileData);
	std::vector<CPluginInfo*> files;
	if (hSearch == INVALID_HANDLE_VALUE)
	{
		return;
	}

	bool fFinished = false;
	while (!fFinished)
	{
		if (!((FileData.cFileName[0] == '.')
			&& ((FileData.cFileName[1] == '.'
				&& FileData.cFileName[2] == 0) || FileData.cFileName[1] == 0)))
		{
			if (!(FileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				if (ci_ends_with(FileData.cFileName, ".dll") && ci_starts_with(FileData.cFileName, "MQ"))
				{
					std::string_view fName = FileData.cFileName;
					fName = fName.substr(0, fName.length() - 4);

					files.push_back(new CPluginInfo(this, fName, false));
				}
			}
		}
		if (!FindNextFile(hSearch, &FileData))
			fFinished = true;
	}

	FindClose(hSearch);
	SubNodes = std::move(files);
}