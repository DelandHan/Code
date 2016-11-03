#include "stdafx.h"
#include "DisplayHub.h"
#include <map>

using namespace std;
using namespace autownd;

DisplayHub::DisplayHub()
{
}


DisplayHub::~DisplayHub()
{
}

int DisplayHub::initialize(IDataHub * datasource)
{
	if (theDisplayBoard.initialize()) return 1;

	if (theDisplayBoard.connectDisplayObj(theItemPanel, { 0,0 }, { 50,100 }, 1)) return 1;
	if (theDisplayBoard.connectDisplayObj(theItemPanel + 1, { 50,0 }, { 50,50 }, 1)) return 1;
	if (theDisplayBoard.connectDisplayObj(&theAttPanel, { 50,50 }, { 50,50 }, 1)) return 1;
	if (theDisplayBoard.connectDisplayObj(&theUpButton, { 0,5 }, { 100,90 }, 0)) return 1;

	theDisplayBoard.getMsgSet()->addMsgProc(WM_NOTIFY, this, &DisplayHub::beNotified);
	theDisplayBoard.getMsgSet()->addMsgProc(WM_COMMAND, this, &DisplayHub::onCommand);

	theDisplayBoard.update();

	theCenterUnit.connect(this, datasource);

	return 0;
}

void DisplayHub::refreshItemPanel(ItemPool * itemlist, int panelID)
{
//	int sel = ListView_GetNextItem(theItemPanel[panelID].wnd(), -1, LVNI_SELECTED); //get last selection position
	theItemPanel[panelID].clear(); //clear the current panel
	theAttPanel.clear();
	if (itemlist == nullptr) return;
	for (ItemPool::iterator it = itemlist->begin(); it != itemlist->end(); it++)
	{
		if (it->getType() == 2)
		{
			theAttPanel.addAttribute(it->strW(), it->getValue());
		}
		else
			theItemPanel[panelID].addItems(it->strW(), it->getType() == 1 ? 1 : 0, it->param());
	}
//	if (sel != -1) ListView_SetItemState(theItemPanel[panelID].wnd(), sel, LVIS_SELECTED, LVIS_SELECTED);//re-select it
}

void DisplayHub::displayPath(std::wstring & path)
{
	SetWindowText(theUpButton.wnd(), path.c_str());
}

void DisplayHub::getClipboard(std::string & buff)
{
	OpenClipboard(theDisplayBoard.wnd());
	HANDLE hmem = GetClipboardData(CF_TEXT);
	char * data = (char *)GlobalLock(hmem);

	buff = data;

	GlobalUnlock(hmem);
	CloseClipboard();

}

int DisplayHub::beNotified(WPARAM wp, LPARAM lp)
{
	LPNMHDR data = (LPNMHDR)lp;

	if (data->hwndFrom == theAttPanel.wnd()) return activeAttPanel(data);
	if (data->hwndFrom == theItemPanel[0].wnd()) return activeItemPanel(0,data);
	if (data->hwndFrom == theItemPanel[1].wnd()) return activeItemPanel(1, data);

	return 1;
}

int DisplayHub::onCommand(WPARAM wp, LPARAM lp)
{
	if (lp == (LPARAM)theUpButton.wnd()) return theCenterUnit.goHighLevel();
	if (lp == (LPARAM)theAttPanel.wnd()) //attpanel finish editing
	{
		const wstring *result = theAttPanel.getEditResult();
		return theCenterUnit.updateAtt(result[0].c_str(), result[1].c_str(), result[2].c_str());
	}

	if (lp == 0)
	{
		return theCenterUnit.setMenuResult(LOWORD(wp));
	}
	return 0;
}

int DisplayHub::activeItemPanel(int id, LPNMHDR data)
{
	LPNMITEMACTIVATE param = (LPNMITEMACTIVATE)data;
	switch (data->code)
	{
	case LVN_ITEMCHANGED:
	{
		theCenterUnit.select(param->lParam, id);
	}
	break;
	case NM_DBLCLK:
	{
		if (param->iItem == -1) return 1;
		theCenterUnit.dbClick(theItemPanel[id].getParam(param->iItem));
	}
	break;
	case LVN_ENDLABELEDIT:
	{
		NMLVDISPINFO* info = (NMLVDISPINFO*)data;
		if (info->item.pszText == nullptr) return 1;
		wstring buff = info->item.pszText;
		if (theCenterUnit.edit(info->item.lParam, buff)) return 1; //failed

		theItemPanel[id].setItemText(info->item.iItem, info->item.iSubItem, &buff[0]);
	}
	break;
	case NM_RCLICK:
	{
		LPARAM sel = 0;
		if (param->iItem != -1) sel = theItemPanel[id].getParam(param->iItem);

		LVPool menuPool;
		theCenterUnit.getMenu(&menuPool, id, sel);

		if (menuPool.size() == 0) return 1;
		
		ContextMenu theMenu;
		for (LVPool::iterator it = menuPool.begin(); it != menuPool.end(); it++)
		{
			theMenu.addMenuItem(&it->strW()[0], it->param());
		}
		
		RECT rect; GetWindowRect(theItemPanel[id].wnd(), &rect);
		theMenu.show(param->ptAction.x + rect.left, param->ptAction.y + rect.top, theDisplayBoard.wnd());

		return 0;
	}
	break;
	
	default:
		return 1;
	}
	return 0;
}

int DisplayHub::activeAttPanel(LPNMHDR data)
{
	LPNMITEMACTIVATE param = (LPNMITEMACTIVATE)data;

	switch (data->code)
	{
	case NM_CLICK:
	{
		if (param->iItem == -1 || param->iItem == theAttPanel.getCount() - 1) {
			param->iItem = theAttPanel.getCount() - 1;
			param->iSubItem = 0;
		}
		theAttPanel.startEditing(param->iItem, param->iSubItem, theDisplayBoard.getMsgSet()->retrieve(WM_COMMAND));
	}
	break;
	default:
		return 1;
	}
	return 0;
}

