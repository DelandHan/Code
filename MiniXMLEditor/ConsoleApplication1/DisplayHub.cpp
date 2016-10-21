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

int DisplayHub::initialize()
{
	if (theDisplayBoard.initialize()) return 1;

	if (theDisplayBoard.connectDisplayObj(theItemPanel, { 0,0 }, { 50,100 }, 1)) return 1;
	if (theDisplayBoard.connectDisplayObj(theItemPanel + 1, { 50,0 }, { 50,50 }, 1)) return 1;
	if (theDisplayBoard.connectDisplayObj(&theAttPanel, { 50,50 }, { 50,50 }, 1)) return 1;
	if (theDisplayBoard.connectDisplayObj(&theUpButton, { 0,5 }, { 100,90 }, 0)) return 1;

	theMenu.addMenuItem(L"Del", 1);

	theDisplayBoard.getMsgSet()->addMsgProc(WM_NOTIFY, this, &DisplayHub::beNotified);
	theDisplayBoard.getMsgSet()->addMsgProc(WM_COMMAND, this, &DisplayHub::onCommand);

	theDisplayBoard.update();

	return 0;
}

int DisplayHub::connectToInputHub(IInputHub * inputhub)
{
	theInputHub = inputhub;
	return inputhub == nullptr ? 1 : 0;
}

void DisplayHub::refreshItemPanel(ItemPool * itemlist, int panelID)
{
//	int sel = ListView_GetNextItem(theItemPanel[panelID].wnd(), -1, LVNI_SELECTED); //get last selection position
	theItemPanel[panelID].clear(); //clear the current panel
	if (itemlist == nullptr) return;
	for (ItemPool::iterator it = itemlist->begin(); it != itemlist->end(); it++)
	{
		theItemPanel[panelID].addItems(it->strW(), it->type() == 1 ? 1 : 0, it->param());
	}
//	if (sel != -1) ListView_SetItemState(theItemPanel[panelID].wnd(), sel, LVIS_SELECTED, LVIS_SELECTED);//re-select it
}

void DisplayHub::refreshAttPanel(AttPool * attlist)
{
	theAttPanel.clear();
	if (attlist == nullptr) return;

	for (AttPool::iterator it = attlist->begin(); it != attlist->end(); it++)
	{
		theAttPanel.addAttribute(it->strW(), it->valueW());
	}

	theAttPanel.addAttribute(wstring(L""), wstring(L""));
}

void DisplayHub::displayPath(std::wstring & path)
{
	SetWindowText(theUpButton.wnd(), path.c_str());
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
	if (lp == (LPARAM)theUpButton.wnd()) return theInputHub->goHighLevel();
	if (lp == (LPARAM)theAttPanel.wnd()) //attpanel finish editing
	{
		wstring *result = theAttPanel.getEditResult();
		return theInputHub->updateAtt(result[0].c_str(), result[1].c_str(), result[2].c_str());
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
		theInputHub->select(param->lParam, id);
	}
	break;
	case NM_DBLCLK:
	{
		if (param->iItem == -1) return 1;
		theInputHub->dbClick(theItemPanel[id].getParam(param->iItem));
	}
	break;
	case LVN_ENDLABELEDIT:
	{
		NMLVDISPINFO* info = (NMLVDISPINFO*)data;
		if (info->item.pszText == nullptr) return 1;
		wstring buff = info->item.pszText;
		if (theInputHub->edit(info->item.lParam, buff)) return 1; //failed

		theItemPanel[id].setItemText(info->item.iItem, info->item.iSubItem, &buff[0]);
	}
	break;
	case NM_RCLICK:
	{
		RECT rect; GetWindowRect(theItemPanel[id].wnd(), &rect);

		if (param->iItem != -1) theMenu.show(param->ptAction.x + rect.left, param->ptAction.y + rect.top, theDisplayBoard.wnd());
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

