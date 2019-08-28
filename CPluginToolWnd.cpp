/***************************************************************
* Plugin Tool Window Class                                      *
*                                                              *
* Window listing all available Plugins.                         *
***************************************************************/
#include "CPluginToolWnd.h"

CPluginToolWnd::CPluginToolWnd(CPluginTree *tree) : CCustomWnd("PluginManagerWindow")
{
	SetWndNotification(CPluginToolWnd);
	validXML = checkXML();
	if(!validXML)
	{
		return;
	}
	PluginTree = tree;
	InitListView();
	isWndActive = true;
}

CPluginToolWnd::~CPluginToolWnd()
{
}

void CPluginToolWnd::ToggleVisibility()
{
	if(isWndActive)
	{
		HideWnd();
	}
	else
	{
		ShowWnd();
	}
}

void CPluginToolWnd::RefreshPluginList()
{
	SetPluginListItems();
}

void CPluginToolWnd::ShowWnd()
{
	if(isWndActive)
	{
		return;
	}
	isWndActive = true;
	((CXWnd *)this)->Show(1,1);
}

void CPluginToolWnd::HideWnd()
{
	if(!isWndActive)
	{
		return;
	}
	isWndActive = false;
	((CXWnd *)this)->Show(0,0);
}

bool CPluginToolWnd::IsActive()
{
	return isWndActive;
}

int CPluginToolWnd::WndNotification(CXWnd *pWnd, unsigned int Message, void *unknown)
{ 
	CHAR szTemp[MAX_STRING]={0}, szBuffer[MAX_STRING]={0}, szMessageThing[MAX_STRING]={0};
	int CurrentLine;
	if (pWnd == (CXWnd*)PluginListBox)
	{
		if (Message == XWM_LCLICK)
		{
			CurrentLine = PluginListBox->GetCurSel();
			if (CurrentLine >= 0)
			{
				char name[MAX_STRING] = { 0 };
				CPluginInfo* selected = PluginTree->GetPluginInfo(CurrentLine);
				sprintf_s(name, "%s", selected->GetName());
				int endName = (int)(strstr(name, ".") - name);
				if (endName >= 0 && endName < MAX_STRING)
					name[endName] = 0;
				if (int Loaded = FindMQ2Plugin(name) ? 0 : 1) {
					char szOutput[MAX_STRING] = { 0 };
					sprintf_s(szOutput, "/plugin %s", selected->GetName());
					DoCommand(GetCharInfo()->pSpawn, szOutput);
					PluginListBox->SetItemColor(CurrentLine, 0, 0xFF048500);
				}
				PluginListBox->ClearSel(CurrentLine);
			}
		} else if (Message == XWM_RCLICK) {
			CurrentLine = PluginListBox->GetCurSel();
			if (CurrentLine >= 0)
			{
				char name[MAX_STRING] = { 0 };
				CPluginInfo* selected = PluginTree->GetPluginInfo(CurrentLine);
				sprintf_s(name, "%s", selected->GetName());
				int endName = (int)(strstr(name, ".") - name);
				if (endName >= 0 && endName < MAX_STRING)
					name[endName] = 0;
				if (int Loaded = FindMQ2Plugin(name) ? 1 : 0) {
					char szOutput[MAX_STRING] = { 0 };
					sprintf_s(szOutput, "/plugin %s unload", selected->GetName());
					DoCommand(GetCharInfo()->pSpawn, szOutput);
					PluginListBox->SetItemColor(CurrentLine, 0, 0xFFCC3333);
				}
				PluginListBox->ClearSel(CurrentLine);
			}
		}
	}
	return CSidlScreenWnd::WndNotification(pWnd,Message,unknown); 
}

bool CPluginToolWnd::checkXML()
{
	bool checkflag = true;
	if (!(PluginListBox=(CListWnd*)GetChildItem("PM_PluginList")))
	{
		checkflag = false;
	}
	return checkflag;
}

void CPluginToolWnd::SetPluginListItems()
{
	PluginListBox->DeleteAll();
	std::vector<CPluginInfo *> v = PluginTree->GetCurrentPluginList();
	unsigned int i;
	unsigned int maxWidthCol1 = 0;
	unsigned int maxWidthCol2 = 0;
	for(i = 0; i < v.size(); i++)
	{
#if defined(ROF2EMU) && defined(UFEMU)
		PluginListBox->AddString(&CXStr(""),0,0,0);
#else
		PluginListBox->AddString(CXStr(""), 0,0,0);
#endif
		PluginListBox->SetItemText(i,0,&CXStr(v[i]->GetName()));
		if(v[i]->IsDirectory())
		{
			PluginListBox->SetItemColor(i,0,0xFFFFFFFF);
		}
		else
		{
			int endName = 0;
			char szOutput[MAX_STRING] = { 0 };
			CPluginInfo* selected = PluginTree->GetPluginInfo(i);
			sprintf_s(szOutput, "%s", selected->GetName());
			endName = (int)(strstr(szOutput, ".")-szOutput);
			if (endName >=0 && endName < MAX_STRING)
				szOutput[endName] = 0;
			if (FindMQ2Plugin(szOutput))
				PluginListBox->SetItemColor(i,0,0xFF048500);
			else
				PluginListBox->SetItemColor(i, 0, 0xFFCC3333);
		}
	}
}

void CPluginToolWnd::InitListView()
{
	if(!PluginTree)
	{
		initialized = false;
		return;
	}
	SetPluginListItems();
}

PMQPLUGIN FindMQ2Plugin(PCHAR szLine)
{
	PMQPLUGIN pPlugin = pPlugins;
	while (pPlugin)
	{
		if (!_stricmp(szLine, pPlugin->szFilename))
		{
			return pPlugin;
		}

		pPlugin = pPlugin->pNext;
	}
	return nullptr;
}