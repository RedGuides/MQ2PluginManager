/***************************************************************
 * Plugin Tool Window Class                                    *
 *                                                             *
 * Window listing all available Plugins.                       *
 ***************************************************************/

#include "CPluginToolWnd.h"

#include <mq/Plugin.h>

CPluginToolWnd::CPluginToolWnd(CPluginTree* tree)
	: CCustomWnd("PluginManagerWindow")
{
	PluginListBox = (CListWnd*)GetChildItem("PM_PluginList");
	PluginTree = tree;

	if (PluginListBox == nullptr)
		return;

	InitListView();
	isWndActive = true;
}

CPluginToolWnd::~CPluginToolWnd()
{
}

void CPluginToolWnd::ToggleVisibility()
{
	if (isWndActive)
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
	if (isWndActive)
	{
		return;
	}

	isWndActive = true;
	this->Show(true);
}

void CPluginToolWnd::HideWnd()
{
	if (!isWndActive)
	{
		return;
	}

	isWndActive = false;
	this->Show(false);
}

int CPluginToolWnd::WndNotification(CXWnd* pWnd, unsigned int Message, void* pData)
{
	if (pWnd == PluginListBox)
	{
		if (Message == XWM_LCLICK)
		{
			int CurrentLine = PluginListBox->GetCurSel();
			if (CurrentLine >= 0)
			{
				CPluginInfo* selected = PluginTree->GetPluginInfo(CurrentLine);
				const char* name = selected->GetName();

				if (!IsPluginLoaded(selected->GetName()))
				{
					DoCommandf("/plugin %s load", selected->GetName());

					PluginListBox->SetItemColor(CurrentLine, 0, 0xFF048500);
				}

				PluginListBox->ClearSel(CurrentLine);
			}

			return 0;
		}

		if (Message == XWM_RCLICK)
		{
			int CurrentLine = PluginListBox->GetCurSel();
			if (CurrentLine >= 0)
			{
				CPluginInfo* selected = PluginTree->GetPluginInfo(CurrentLine);

				if (mq::IsPluginLoaded(selected->GetName()))
				{
					DoCommandf("/plugin %s unload", selected->GetName());

					PluginListBox->SetItemColor(CurrentLine, 0, 0xFFCC3333);
				}
				PluginListBox->ClearSel(CurrentLine);
			}

			return 0;
		}
	}

	return CSidlScreenWnd::WndNotification(pWnd, Message, pData);
}

void CPluginToolWnd::SetPluginListItems()
{
	PluginListBox->DeleteAll();

	const std::vector<CPluginInfo*>& v = PluginTree->GetCurrentPluginList();
	int index = 0;
	for (CPluginInfo* pluginInfo : v)
	{
		PluginListBox->AddString(CXStr(), 0);
		PluginListBox->SetItemText(index, 0, pluginInfo->GetName());

		if (pluginInfo->IsDirectory())
		{
			PluginListBox->SetItemColor(index, 0, 0xFFFFFFFF);
		}
		else
		{
			PluginListBox->SetItemColor(index, 0, mq::IsPluginLoaded(pluginInfo->GetName()) ? 0xFF048500 : 0xFFCC3333);
		}

		++index;
	}
}

void CPluginToolWnd::InitListView()
{
	if (!PluginTree)
	{
		initialized = false;
		return;
	}

	SetPluginListItems();
}
