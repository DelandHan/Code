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
	if (theDisplayBoard.connectDisplayObj(&theUpButton, { 5,5 }, { 20,20 }, 0)) return 1;


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

void DisplayHub::refreshItemPanel(ItemPool * itemlist)
{
	theItemPanel[0].clear();
	if (itemlist == nullptr) return;
	for (ItemPool::iterator it = itemlist->begin(); it != itemlist->end(); it++)
	{
		theItemPanel[0].addItems(it->first.first, it->first.second == 1 ? 1 : 0, it->second);
	}
}

void DisplayHub::refreshChildPanel(ItemPool * itemlist)
{
	theItemPanel[1].clear();
	if (itemlist == nullptr) return;
	for (ItemPool::iterator it = itemlist->begin(); it != itemlist->end(); it++)
	{
		theItemPanel[1].addItems(it->first.first, it->first.second == 1 ? 1 : 0, it->second);
	}
}

void DisplayHub::refreshAttPanel(AttPool * attlist)
{
	theAttPanel.clear();
	if (attlist == nullptr) return;

	for (AttPool::iterator it = attlist->begin(); it != attlist->end(); it++)
	{
		theAttPanel.addAttribute(it->first, it->second);
	}
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
		if (theInputHub->edit(info->item.lParam, info->item.pszText)) return 1;
	}
	break;
	case NM_RCLICK:
	{
		return 1;
	}
	break;
	case NM_CLICK:
	{
		return 1;
	}
	break;
	default:
		return 1;
	}
	return 0;
}

int DisplayHub::activeAttPanel(LPNMHDR data)
{
	return 0;
}

