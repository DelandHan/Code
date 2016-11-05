#include "stdafx.h"
#include "DisplayHub.h"
#include <map>

using namespace std;
using namespace autownd;

class ItemList
{
public:
	void checkoutSelection(List * panel);
	void checkinSelection(List * panel);

	inline ItemPool * getList() { return &theList; }
private:
	ItemPool theList;
};

void ItemList::checkoutSelection(List * panel)
{
	int pos = -1; 
	while (1)
	{
		pos = panel->getNextSelectedItem(pos);
		if (pos == -1) break;

		autownd::List::LSet set = panel->at(pos);
//		TCHAR buff[255];
		const LPLVITEM lvitem = set
//			.setText(buff, 255)
			.setParam(0).sync();
		theList.push_back({ L"",0,lvitem->lParam });
	}
}

void ItemList::checkinSelection(List * panel)
{
	int pos = panel->getNextSelectedItem(-1); 
	int oldpos = List::eoi;
	ItemPool::iterator it = theList.begin();
	while (pos != -1 || it != theList.end())
	{
		if (pos == -1)
		{
			//add items
			if (oldpos != List::eoi) oldpos++;
			wstring buff = it->strW();
			panel->at(oldpos).setText(&buff[0], buff.size()).setImage(it->type() == 1 ? 1 : 0).setParam(it->param()).update();
			it++;
			continue;
		}
		if (it == theList.end())
		{
			ListView_DeleteItem(panel->wnd(), pos);
			oldpos = pos;
			pos--;
			pos = panel->getNextSelectedItem(pos);
			continue;
		}

		if (it->param() == panel->at(pos).setParam(0).sync()->lParam)
		{
			//panel->at(pos).setText(&it->strW()[0], it->strW().size()).setParam(it->param()).setImage(it->type()).update();
			it++;
		}
		oldpos = pos;
		pos = panel->getNextSelectedItem(pos);

	}
}



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

	theDisplayBoard.getMsgSet()->addMsgProc(WM_NOTIFY, this, &DisplayHub::beNotified);
	theDisplayBoard.getMsgSet()->addMsgProc(WM_COMMAND, this, &DisplayHub::onCommand);

	theDisplayBoard.update();

	return 0;
}

int DisplayHub::connectToDataPool(IDataHub *datapool)
{
	theControlCenter.connect(this, datapool);
	return datapool == nullptr ? 1 : 0;
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
	if (lp == (LPARAM)theUpButton.wnd()) return theControlCenter.goHighLevel();
	if (lp == (LPARAM)theAttPanel.wnd()) //attpanel finish editing
	{
		wstring *result = theAttPanel.getEditResult();
		return theControlCenter.updateAtt(result[0].c_str(), result[1].c_str(), result[2].c_str());
	}

	if (lp == 0)
	{
		int panel = -1; HWND wnd = GetFocus();
		if (wnd == theItemPanel[0].wnd()) panel = 0;
		if (wnd == theItemPanel[1].wnd()) panel = 1;
		if (panel == -1) return 1;

		ItemList checkoutlist;

		checkoutlist.checkoutSelection(theItemPanel + panel);
		theControlCenter.setMenuResult(LOWORD(wp), panel, checkoutlist.getList());
		checkoutlist.checkinSelection(theItemPanel + panel);
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
		theControlCenter.select(param->lParam, id);
	}
	break;
	case NM_DBLCLK:
	{
		if (param->iItem == -1) return 1;
		theControlCenter.dbClick(theItemPanel[id].getParam(param->iItem));
	}
	break;
	case LVN_ENDLABELEDIT:
	{
		NMLVDISPINFO* info = (NMLVDISPINFO*)data;
		if (info->item.pszText == nullptr) return 1;

		ItemPool checkoutpool;
		checkoutpool.push_back({ info->item.pszText ,0,info->item.lParam });
		if (theControlCenter.edit(&checkoutpool)) return 1; //failed

		theItemPanel[id].setItemText(info->item.iItem, info->item.iSubItem, &checkoutpool.back().strW()[0]);
	}
	break;
	case NM_RCLICK:
	{
		LVPool menuPool;
		theControlCenter.getMenu(&menuPool, id, param->iItem);

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
