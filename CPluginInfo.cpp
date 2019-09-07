/***************************************************************
* Plugin Info Class Implementation                              *
*                                                              *
*                                                              *
***************************************************************/
#include "CPluginInfo.h"

CPluginInfo::CPluginInfo(CPluginInfo *parent, char*name, bool isDirectory)
{
	strcpy_s(Name,name);
	directoryFlag = isDirectory;
	rootFlag = (parent == NULL);
	if(rootFlag)
	{
		Parent = NULL;
		DirectoryLevel = 0;
		directoryFlag = true;
		sprintf_s(FolderPath,"%s", gszINIPath);
		InitSubNodes();
	}
	else
	{
		Parent = parent;
		if(directoryFlag)
		{
			DirectoryLevel = Parent->GetLevel()+1;
			sprintf_s(FolderPath,"%s\\%s",Parent->GetFolderPath(),Name);
			InitSubNodes();
		}
		else
		{
			DirectoryLevel = Parent->GetLevel();
			sprintf_s(FolderPath,"%s",Parent->GetFolderPath());
		}
	}
	std::string dir = FolderPath;
	sprintf_s(DirectoryName,"%s",dir.substr(dir.find_last_of("\\")+1).c_str());
}
CPluginInfo::~CPluginInfo()
{
	if(SubNodes.size()>0)
	{
		unsigned int i;
		for(i = 0; i < SubNodes.size(); i++)
		{
			delete SubNodes[i];
			SubNodes[i] = 0;
		}
	}
}

char* CPluginInfo::GetFolderPath()
{
	return FolderPath;
}
char* CPluginInfo::GetName()
{
	return Name;
}
int CPluginInfo::GetLevel()
{
	return DirectoryLevel;
}
vector<CPluginInfo *> CPluginInfo::GetListItems()
{
	return SubNodes;
}
char* CPluginInfo::GetDirectoryName()
{
	return DirectoryName;
}
bool CPluginInfo::IsDirectory()
{
	return directoryFlag;
}
bool CPluginInfo::IsRoot()
{
	return rootFlag;
}
char* CPluginInfo::GetDirectoryPath()
{
	return FolderPath;
}
CPluginInfo* CPluginInfo::GetParent()
{
	return Parent;
}
CPluginInfo* CPluginInfo::GetInfoForId(int id)
{
	if(id < 0)
	{
		return NULL;
	}
	if(SubNodes.size()-1 < (unsigned int)id)
	{
		return NULL;
	}
	return SubNodes[id];
}
void CPluginInfo::InitSubNodes()
{
	if(!directoryFlag)
	{
		return;
	}
	HANDLE hSearch;
	WIN32_FIND_DATA FileData;
	BOOL fFinished = FALSE;
	CHAR szFilename[MAX_STRING] = {0};
	CHAR szSubFolder[MAX_STRING] = {0};
	sprintf_s(szFilename,"%s\\*.*",GetFolderPath());
	hSearch = FindFirstFile(szFilename, &FileData);
	vector<CPluginInfo *> files;
	if (hSearch == INVALID_HANDLE_VALUE)
	{
		return;
	}
	while (!fFinished)
	{
		if(!( (FileData.cFileName[0]=='.') && ( (FileData.cFileName[1]=='.' && FileData.cFileName[2]==0) || FileData.cFileName[1]==0 ) ))
		{
			if(FileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
			}
			else
			{
				std::string fName = FileData.cFileName;
				if(fName.substr(fName.find_last_of(".")+1) == "dll")
				{

					if (fName.length())
						if (strstr(fName.c_str(), "MQ2")) {
							fName.erase(fName.length()-4);
							char theName[MAX_STRING] = { 0 };
							strcpy_s(theName, fName.c_str());
							files.push_back(new CPluginInfo(this, theName, false));
						}
				}
			}
		}
		if (!FindNextFile(hSearch, &FileData))
			fFinished = TRUE;
	}
	FindClose(hSearch);
	SubNodes.insert(SubNodes.end(),files.begin(),files.end());
}