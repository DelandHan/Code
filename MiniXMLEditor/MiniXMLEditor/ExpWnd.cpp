#include "stdafx.h"
#include "ExpWnd.h"
#include "resource.h"

using namespace autownd;
using namespace memory;
using namespace std;

ExpWnd::ExpWnd()
	:theData(nullptr)
{
	theButton.param = 0;
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
	theLeftPanel.setRect({ 50,100 }, { 0,0 });
	theLeftPanel.init(&theMainWnd, LVS_EDITLABELS, 1, true, 0);

	//add right panel
	theRightPanel.setRect({ 50,50 }, { 50,0 });
	theRightPanel.init(&theMainWnd, LVS_EDITLABELS, 1, true, 0);

	//add att panel
	theAttPanel.setRect({ 50,50 }, { 50,50 });
	theAttPanel.init(&theMainWnd, 0, 2, false, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

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
	theLeftPanel.clear();
	theLeftPanel.theParam = param;

	//the up button and the path
	chain.first(); chain.line();
	SetWindowText(thePath.wnd(), chain.at()->data<TCHAR>());
	chain.at(); chain.at()->inject(&theButton.param, 1);

	//redraw left panel
	while (chain.line())
	{
		int type = 0; LPARAM param = 0;
		Bullet *node = chain.at();
		chain.at()->inject(&type, 1); chain.at()->inject(&param, 1);
		theLeftPanel.theObj.at()
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
	theRightPanel.clear();
	if (param == 0) return;
	BulletChain chain(3);
	chain.first()->fill("childs");
	chain.at()->fill(param);
	if (theData->pull(&chain)) return;

	//begin refresh
	theRightPanel.theParam = param;

	chain.first(); 
	chain.line();//ignore the params

	//redraw right panel
	while (chain.line())
	{
		int type = 0; LPARAM param = 0;
		Bullet *node = chain.at();
		chain.at()->inject(&type, 1); chain.at()->inject(&param, 1);
		theRightPanel.theObj.at()
			.setText(node->data<TCHAR>(), node->size() / sizeof(TCHAR))
			.setImage(type == 1 ? 1 : 0)
			.setParam(param)
			.update();
	}

}

void ExpWnd::updateAttlist(LPARAM param)
{
	theAttPanel.clear();
	if (param == 0) return;
	BulletChain chain(2);
	chain.first()->fill("read");
	chain.at()->fill(param);
	theData->pull(&chain);

	//begin refresh
	theAttPanel.theParam = param;
	chain.first();
	while (chain.line())
	{
		Bullet *key = chain.at();
		Bullet *value = chain.at();

		autownd::List::LSet set = theAttPanel.theObj.at();
		set.setText(key->data<TCHAR>(), key->size() / sizeof(TCHAR)).update();
		set.setText(1, value->data<TCHAR>());
	}
	theAttPanel.theObj.at().update();
}

int ExpWnd::beNotified(memory::ParamChain params)
{
	LPNMHDR data = (params.begin() + 1)->second.pdata<NMHDR>();
	ExpPanel *panel = nullptr;
	if (data->hwndFrom == theLeftPanel.theObj.wnd()) panel = &theLeftPanel;
	if (data->hwndFrom == theRightPanel.theObj.wnd()) 
	{
		if (theRightPanel.theParam == 0) return 1;
		panel = &theRightPanel;
	}

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
			updateItemlist(panel->theObj.at(temp->iItem).setParam(0).sync()->lParam);
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
			ListView_SetItemText(panel->theObj.wnd(), info->item.iItem, info->item.iSubItem, chain.at()->data<TCHAR>());
		}
		break;
		case NM_RCLICK:
		{
			if (temp->iItem != -1)
			{
				theContext.obj.show(temp->ptAction.x, temp->ptAction.y, theMainWnd.wnd());
				theContext.param = panel->theObj.at(temp->iItem).setParam(0).sync()->lParam;
			}
			else
			{
				if (panel->theObj.getCount() == 0)
				{
					theData->push({ { "append", panel->theParam} });
					if (panel == &theLeftPanel) updateItemlist(theLeftPanel.theParam);
					else updateChildList(theRightPanel.theParam);
				}
			}
		}
		break;
		default:
			break;
		}
		return 0;
	}

	if (data->hwndFrom == theAttPanel.theObj.wnd())//att panel
	{
		LPNMITEMACTIVATE temp = (LPNMITEMACTIVATE)data;
		if (data->code == NM_CLICK)
		{
			TCHAR buff[255]; RECT rect;
			if (temp->iSubItem == -1) return 1;
			if (temp->iItem == -1 || temp->iItem == theAttPanel.theObj.getCount() - 1) {
				temp->iItem = theAttPanel.theObj.getCount() - 1;
				temp->iSubItem = 0;
			}
			List::LSet set= theAttPanel.theObj.at(temp->iItem);

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
				{"parent",theAttPanel.theObj.wnd()},
				{"rect",&rect},
				{"buff",&theEdit.str[temp->iSubItem][0]}
			});

			return 0;
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
		if (theEdit.obj.wnd()) return 0; //if edit is activing, do nothing.

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
		switch (LOWORD(*(params.begin())->second.data<long>()))
		{
		case ID_EDIT_DEL:
		{
			if (theContext.param == 0) return 0;
			theData->push({ {"del",theContext.param} });
		}
		break;
		case ID_EDIT_INSERTBEFORE:
		{
			if (theContext.param == 0) return 0;
			theData->push({ { "ins_b",theContext.param } });
		}
		break;
		case ID_EDIT_INSERTAFTER:
		{
			if (theContext.param == 0) return 0;
			theData->push({ { "ins_a",theContext.param } });
		}
		break;
		case ID_EDIT_CHANGETYPE:
		{
			if (theContext.param == 0) return 0;
			theData->push({ { "chgtyp",theContext.param } });
		}
		break;
		case ID_EDIT_PASTE:
		{
			OpenClipboard(theLeftPanel.theObj.wnd());
			HANDLE hmem = GetClipboardData(CF_TEXT);
			char * data = (char *)GlobalLock(hmem);

			if (theContext.param == 0)
			{
				theData->push({ { "append", theLeftPanel.theParam },{ "data",data } });
			}
			else
			{
				theData->push({ { "ins_a", theContext.param },{ "data",data } });
			}

			GlobalUnlock(hmem);
			CloseClipboard();
		}
		break;
		default:
			return 0;
		}
		theContext.param = 0;
		updateItemlist(theLeftPanel.theParam);
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
			{ "item",theAttPanel.theParam }
		});
	}
	else
	{
		theData->push({
			{ "setkey",theEdit.str[0].c_str() },
			{ "key",params.begin()->second.pdata<TCHAR>() },
			{ "value",theEdit.str[1].c_str() },
			{ "item",theAttPanel.theParam }
		});
	}

	updateAttlist(theAttPanel.theParam);
	theEdit.str[0].clear(); theEdit.str[1].clear();
	return 1;
}

int ExpWnd::updateLayout(memory::ParamChain params)
{
	RECT rect;
	GetClientRect(theMainWnd.wnd(), &rect);
	OffsetRect(&rect, GetSystemMetrics(SM_CXBORDER), GetSystemMetrics(SM_CXBORDER));

	const int edge_blank = 2;
	const int button_h = 20;

	rect.top += button_h + edge_blank * 2;

	theLeftPanel.move(&rect);
	theRightPanel.move(&rect);
	theAttPanel.move(&rect);

	MoveWindow(theButton.obj.wnd(),
		edge_blank, edge_blank, button_h, button_h, TRUE);

	MoveWindow(thePath.wnd(),
		edge_blank * 2 + button_h, edge_blank, rect.right-rect.left - edge_blank * 3 - button_h, button_h, TRUE);

	return 1;
}

/////////////////////////////////////child objects/////////////////////////////////

ExpWnd::ExpPanel::ExpPanel()
	:theParam(0), theSize(0, 0), thePos(0, 0), theColumnCount(0)
{
}

ExpWnd::ExpPanel::~ExpPanel()
{
}

void ExpWnd::ExpPanel::setRect(vec size, vec pos)
{
	theSize = size;
	thePos = pos;
}

void ExpWnd::ExpPanel::init(WndObj *parent, long addStyle, int column, bool icon, long extendStyle)
{
	//create the control
	parent->addControl(&theObj, WC_LISTVIEW, {
		{ "size",theSize },
		{ "style",addStyle | LVS_REPORT | LVS_SHOWSELALWAYS | LVS_SINGLESEL | LVS_NOCOLUMNHEADER },
		{ "exstyle", WS_EX_CLIENTEDGE },
		{ "pos",thePos }
	});

	//add columns
	for (int i = 0; i < column; i++) {
		theObj.addColumn(i).set(L"Column", 6).set(-2).update();
	}
	theColumnCount = column;

	//add icons
	if (icon) {
		theObj.buildImageList(16, 16);
		theObj.addIcon(IDI_FILE);
		theObj.addIcon(IDI_FOLDER);
	}

	//extend style
	if(extendStyle) theObj.extendStyle(extendStyle);

	//show
	theObj.show();
}

void ExpWnd::ExpPanel::move(const RECT * clientRect)
{
	int width = clientRect->right - clientRect->left, height = clientRect->bottom - clientRect->top;

	MoveWindow(theObj.wnd(),
		clientRect->left + width * thePos.first / 100, clientRect->top + height*thePos.second / 100,
		width*theSize.first / 100, height*theSize.second / 100,
		TRUE
	);

	for (int i = 0; i < theColumnCount; i++) {
		theObj.resizeColumn(i, (width*theSize.first / 100 - 4) / theColumnCount);
	}
}

void ExpWnd::ExpPanel::clear()
{
	theObj.clear();
	theParam = 0;
}
