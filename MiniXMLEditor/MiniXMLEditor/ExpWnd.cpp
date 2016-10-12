﻿#include "stdafx.h"
#include "ExpWnd.h"
#include "resource.h"

using namespace autownd;
using namespace memory;
using namespace std;

ExpWnd::ExpWnd()
	:theData(nullptr)
{
	theLeftPanel.param = 0;
	theRightPanel.param = 0;
	theButton.param = 0;
	theAttPanel.param = 0;
	theEdit.subitem = 0;
	theContext.param = 0;

	theContext.obj.addMenuItem(L"Del", ID_EDIT_DEL);
	theContext.obj.addMenuItem(L"InsertBefore", ID_EDIT_INSERTBEFORE);
	theContext.obj.addMenuItem(L"InsertAfter", ID_EDIT_INSERTAFTER);
	theContext.obj.addMenuItem(L"ChangeType", ID_EDIT_CHANGETYPE);
}


ExpWnd::~ExpWnd()
{
}

void ExpWnd::init(IModule * module)
{
	//size info
	const int mainwnd_w = 1000, mainwnd_h = 650;

	//static message objects;
	static pair<UINT, MsgProc<ExpWnd>> msg_proc_list[] = {
		{ WM_NOTIFY,{ this, &ExpWnd::beNotified } },
		{ WM_COMMAND,{ this, &ExpWnd::clickButton } },
		{ WM_SIZE,{ this,&ExpWnd::updateLayout } },
		{ 0,{ this,&ExpWnd::setAttribute } }
	};

	static MsgSet mainWndMsgs(msg_proc_list, 3);
	mainWndMsgs.addMsgPair(WM_DESTROY, &autownd::msg_quit);

	theData = module;

	//init main wnd
	Seed mainSeed;
	mainSeed.init({});
	mainSeed.create(&theMainWnd, &mainWndMsgs, {
		{ "menu",IDR_MENU },
		{ "size",vec(mainwnd_w,mainwnd_h) },
		{ "title",L"XMLEditor" }
	});

	//add left panel
	theMainWnd.addControl(&theLeftPanel.obj, WC_LISTVIEW, {
		{ "size",vec(mainwnd_w / 2,mainwnd_h - 50) },
		{ "style",(long)LVS_REPORT | LVS_EDITLABELS | LVS_SHOWSELALWAYS | LVS_SINGLESEL | LVS_NOCOLUMNHEADER | WS_EX_CLIENTEDGE },
		{ "exstyle", WS_EX_CLIENTEDGE },
		{ "pos",vec(20,50) }
	});
	theLeftPanel.obj.addColumn(0).set(L"Items", 6).set(mainwnd_w / 2).update();
	theLeftPanel.obj.buildImageList(16, 16);
	theLeftPanel.obj.addIcon(IDI_FILE);
	theLeftPanel.obj.addIcon(IDI_FOLDER);

	//theLeftPanel.obj.extendStyle(LVS_EX_HEADERINALLVIEWS);//LVS_EX_GRIDLINES
	theLeftPanel.obj.show();

	//add right panel
	theMainWnd.addControl(&theRightPanel.obj, WC_LISTVIEW, {
		{ "size",vec(mainwnd_w / 2,mainwnd_h / 2 - 50) },
		{ "style",(long)LVS_REPORT | LVS_EDITLABELS | LVS_SHOWSELALWAYS | LVS_SINGLESEL | LVS_NOCOLUMNHEADER | WS_EX_CLIENTEDGE },
		{ "exstyle", WS_EX_CLIENTEDGE },
		{ "pos",vec(mainwnd_w / 2 + 20,50) }
	});
	theRightPanel.obj.addColumn(0).set(L"Items", 6).set(mainwnd_w / 2).update();
	theRightPanel.obj.buildImageList(16, 16);
	theRightPanel.obj.addIcon(IDI_FILE);
	theRightPanel.obj.addIcon(IDI_FOLDER);
	theRightPanel.obj.show();

	//add att panel
	theMainWnd.addControl(&theAttPanel.obj, WC_LISTVIEW, {
		{ "size",vec(mainwnd_w / 2,mainwnd_h / 2 - 50) },
		{ "style",(long)LVS_REPORT | LVS_SHOWSELALWAYS | LVS_SINGLESEL | LVS_NOCOLUMNHEADER  },
		{ "exstyle", WS_EX_CLIENTEDGE },
		{ "pos",vec(mainwnd_w / 2 + 20,mainwnd_h / 2 + 50) }
	});
	theAttPanel.obj.addColumn(0).set(L"Key",4).set(120).update();
	theAttPanel.obj.addColumn(1).set(L"Value", 4).set(175).update();

	theAttPanel.obj.extendStyle(LVS_EX_FULLROWSELECT| LVS_EX_GRIDLINES);//LVS_EX_GRIDLINES
	theAttPanel.obj.show();

	//add up button
	theMainWnd.addControl(&theButton.obj, WC_BUTTON, {
		{ "title", L"Up"},
		{ "size",vec(20,20) },
		{ "pos",vec(20,20) },
	});
	theButton.obj.show();

	theMainWnd.addControl(&thePath, WC_STATIC, {
		{ "size",vec(mainwnd_w / 2,20) },
		{ "pos",vec(50,20) },
	});
	thePath.show();

	//set edit callback
	theEdit.obj.setRecv(&(msg_proc_list + 3)->second);

	//finished
	updateLayout({});
	theMainWnd.show();
	updateItemlist(0);
}

void ExpWnd::updateItemlist(LPARAM param)
{
	BulletChain chain(3);
	chain.first()->fill("childs");
	chain.at()->fill(param);
	if (theData->pull(&chain)) return;

	//begin refresh
	theLeftPanel.param = param;

	//the up button and the path
	chain.first(); chain.line();
	SetWindowText(thePath.wnd(), chain.at()->data<TCHAR>());
	chain.at(); chain.at()->inject(&theButton.param, 1);

	//redraw left panel
	theLeftPanel.obj.clear();
	while (chain.line())
	{
		int type = 0; LPARAM param = 0;
		Bullet *node = chain.at();
		chain.at()->inject(&type, 1); chain.at()->inject(&param, 1);
		theLeftPanel.obj.at()
			.setText(node->data<TCHAR>(), node->size() / sizeof(TCHAR))
			.setImage(type == 1 ? 1 : 0)
			.setParam(param)
			.update();
	}

	//update right
	updateChildList(0);
	updateAttlist(0);
}

void ExpWnd::updateChildList(LPARAM param)
{
	theRightPanel.obj.clear();
	if (param == 0) return;
	BulletChain chain(3);
	chain.first()->fill("childs");
	chain.at()->fill(param);
	if (theData->pull(&chain)) return;

	//begin refresh
	theRightPanel.param = param;

	chain.first(); 
	chain.line();//ignore the params

	//redraw right panel
	while (chain.line())
	{
		int type = 0; LPARAM param = 0;
		Bullet *node = chain.at();
		chain.at()->inject(&type, 1); chain.at()->inject(&param, 1);
		theRightPanel.obj.at()
			.setText(node->data<TCHAR>(), node->size() / sizeof(TCHAR))
			.setImage(type == 1 ? 1 : 0)
			.setParam(param)
			.update();
	}

}

void ExpWnd::updateAttlist(LPARAM param)
{
	theAttPanel.obj.clear();
	if (param == 0) return;
	BulletChain chain(2);
	chain.first()->fill("read");
	chain.at()->fill(param);
	theData->pull(&chain);

	//begin refresh
	theAttPanel.param = param;
	chain.first();
	while (chain.line())
	{
		Bullet *key = chain.at();
		Bullet *value = chain.at();

		autownd::List::LSet set = theAttPanel.obj.at();
		set.setText(key->data<TCHAR>(), key->size() / sizeof(TCHAR)).update();
		set.setText(1, value->data<TCHAR>());
	}
	theAttPanel.obj.at().update();
}

int ExpWnd::beNotified(memory::ParamChain params)
{
	LPNMHDR data = (params.begin() + 1)->second.pdata<NMHDR>();
	ListPanel *panel = nullptr;
	if (data->hwndFrom == theLeftPanel.obj.wnd()) panel = &theLeftPanel;
	if (data->hwndFrom == theRightPanel.obj.wnd()) panel = &theRightPanel;

	if (panel)//left panel and right panel
	{
		LPNMITEMACTIVATE temp = (LPNMITEMACTIVATE)data;
		switch (data->code)
		{
		case LVN_ITEMCHANGED:
		{
			if (temp->uNewState)
			{
				updateAttlist(temp->lParam);
				if (panel == &theLeftPanel) updateChildList(temp->lParam);
				theContext.param = temp->lParam;
			}
		}
		break;
		case NM_DBLCLK:
		{
			if (temp->iItem == -1) return 1;
			updateItemlist(panel->obj.at(temp->iItem).setParam(0).sync()->lParam);
		}
		break;
		case LVN_ENDLABELEDIT:
		{
			NMLVDISPINFO* info = (NMLVDISPINFO*)data;
			if (info->item.pszText == nullptr) return 1;
			theData->push({ { "setstr",info->item.pszText },{ "item",info->item.lParam } });
			BulletChain chain(2);
			chain.first()->fill("item");
			chain.at()->fill(info->item.lParam);
			theData->pull(&chain);

			chain.first(); chain.line();
			ListView_SetItemText(panel->obj.wnd(), info->item.iItem, info->item.iSubItem, chain.at()->data<TCHAR>());
		}
		break;
		case NM_RCLICK:
		{
			if (temp->iItem != -1)
			{
				theContext.obj.show(temp->ptAction.x, temp->ptAction.y, theMainWnd.wnd());
				theContext.param = panel->obj.at(temp->iItem).setParam(0).sync()->lParam;
			}
			else
			{
				if (panel->obj.getCount() == 0)
				{
					theData->push({ { "append", panel->param} });
					if (panel == &theLeftPanel) updateItemlist(theLeftPanel.param);
					else updateChildList(theRightPanel.param);
				}
			}
		}
		break;
		default:
			break;
		}
	}

	if (data->hwndFrom == theAttPanel.obj.wnd())//att panel
	{
		LPNMITEMACTIVATE temp = (LPNMITEMACTIVATE)data;
		if (data->code == NM_CLICK)
		{
			TCHAR buff[255]; RECT rect;
			if (temp->iSubItem == -1) return 1;
			if (temp->iItem == -1 || temp->iItem == theAttPanel.obj.getCount() - 1) {
				temp->iItem = theAttPanel.obj.getCount() - 1;
				temp->iSubItem = 0;
			}
			List::LSet set= theAttPanel.obj.at(temp->iItem);

			set.getText(buff, 255, 0); theEdit.str[0] = buff;
			set.getText(buff, 255, 1); theEdit.str[1] = buff;
			set.getRect(&rect, temp->iSubItem);
			//adjust pos
			if (temp->iSubItem == 0) {
				rect.left += 3;
				rect.right = rect.left + (rect.right - rect.left) / 2 - 4;
			}
			else rect.left += 5;
			rect.bottom -= 1;
			
			theEdit.subitem = temp->iSubItem;

			theEdit.obj.init({ 
				{"parent",theAttPanel.obj.wnd()},
				{"rect",&rect},
				{"buff",&theEdit.str[temp->iSubItem][0]}
			});
		}
	}

	return 1;
}

int ExpWnd::clickButton(memory::ParamChain params)
{
	if ((HWND)*(params.begin() + 1)->second.data<long>() == theButton.obj.wnd())
	{
		updateItemlist(theButton.param);
	}
	else
	{
		if (HIWORD(*(params.begin())->second.data<long>()) == 0)
		{
			//file menu
			switch (LOWORD(*(params.begin())->second.data<long>()))
			{
			case ID_FILE_EXIT:
			{
				PostQuitMessage(0);
			}
			break;
			case ID_FILE_OPEN:
			{
				theOpenDialog.open({});		
				theData->push({ {"open",theOpenDialog.getPath().c_str()} });
				updateItemlist(0);
			}
			break;
			case ID_FILE_SAVE:
			{
				theOpenDialog.save({});
				theData->push({ {"save",theOpenDialog.getPath().c_str()} });
			}
			break;
			default:
				break;
			}

			//edit menu
			if (theContext.param == 0) return 0;
			switch (LOWORD(*(params.begin())->second.data<long>()))
			{
			case ID_EDIT_DEL:
			{
				theData->push({ {"del",theContext.param} });
			}
			break;
			case ID_EDIT_INSERTBEFORE:
			{
				theData->push({ { "ins_b",theContext.param } });
			}
			break;
			case ID_EDIT_INSERTAFTER:
			{
				theData->push({ { "ins_a",theContext.param } });
			}
			break;
			case ID_EDIT_CHANGETYPE:
			{
				theData->push({ { "chgtyp",theContext.param } });
			}
			break;
			default:
				break;
			}
			theContext.param = 0;
			updateItemlist(theLeftPanel.param);
		}
	}

	return 1;
}

int ExpWnd::setAttribute(memory::ParamChain params)
{
	if (theEdit.subitem)
	{
		theData->push({
			{ "setkey",theEdit.str[0].c_str() },
			{ "key",theEdit.str[0].c_str() },
			{ "value",params.begin()->second.pdata<TCHAR>() },
			{ "item",theAttPanel.param }
		});
	}
	else
	{
		theData->push({
			{ "setkey",theEdit.str[0].c_str() },
			{ "key",params.begin()->second.pdata<TCHAR>() },
			{ "value",theEdit.str[1].c_str() },
			{ "item",theAttPanel.param }
		});
	}

	updateAttlist(theAttPanel.param);
	theEdit.str[0].clear(); theEdit.str[1].clear();
	return 1;
}

int ExpWnd::updateLayout(memory::ParamChain params)
{
	RECT rect;
	GetClientRect(theMainWnd.wnd(), &rect);
	OffsetRect(&rect, GetSystemMetrics(SM_CXBORDER), GetSystemMetrics(SM_CXBORDER));

	const int edge_blank = 10;
	const int client_w = rect.right - rect.left - edge_blank, client_h = rect.bottom - rect.top - 2 * edge_blank;
	const int button_h = 30;
	const int panel_w = client_w / 2 - edge_blank / 2;

	MoveWindow(theLeftPanel.obj.wnd(),
		edge_blank, edge_blank * 2 + button_h, panel_w, client_h - edge_blank - button_h, TRUE);
	theLeftPanel.obj.resizeColumn(0, client_w / 2 - 3 * edge_blank);

	MoveWindow(theRightPanel.obj.wnd(),
		edge_blank + panel_w, edge_blank * 2 + button_h, panel_w, client_h / 2 - edge_blank / 2 - button_h / 2, TRUE);
	theRightPanel.obj.resizeColumn(0, client_w / 2 - 3 * edge_blank);

	MoveWindow(theAttPanel.obj.wnd(),
		edge_blank + panel_w, edge_blank * 2 + button_h + client_h / 2 - edge_blank / 2 - button_h / 2, panel_w, client_h / 2 - edge_blank / 2 - button_h / 2, TRUE);
	theAttPanel.obj.resizeColumn(0, panel_w / 2 - 4);
	theAttPanel.obj.resizeColumn(1, panel_w / 2);

	MoveWindow(theButton.obj.wnd(),
		edge_blank, edge_blank, button_h, button_h, TRUE);

	MoveWindow(thePath.wnd(),
		edge_blank * 2 + button_h, edge_blank, client_w - edge_blank * 2 - button_h, button_h, TRUE);

	return 1;
}
