#include "stdafx.h"
#include "ExpWnd.h"
#include "resource.h"

using namespace autownd;
using namespace memory;

class MsgNotify
	:public IMsgProcess
{
public:
	MsgNotify(ExpWnd * pro) { theWnd = pro; }
	~MsgNotify() {}
	int handleMsg(WndObj *obj, WPARAM wp, LPARAM lp) override {
		theWnd->beNotified((LPNMHDR)lp);
		return 1;
	}
private:
	ExpWnd * theWnd;
};

class MsgCommand
	:public IMsgProcess
{
public:
	MsgCommand(ExpWnd * pro) { theWnd = pro; }
	~MsgCommand() {}
	int handleMsg(WndObj *obj, WPARAM wp, LPARAM lp) override {
		if (obj) theWnd->clickButton((HWND)lp, HIWORD(wp));//click button
		else theWnd->setAttribute((TCHAR*)wp);//finish editing
		return 1;
	}
private:
	ExpWnd * theWnd;

};

ExpWnd::ExpWnd()
	:theData(nullptr)
{
	theLeftPanel.param = 0;
	theButton.param = 0;
	theRightPanel.param = 0;
	theEdit.subitem = 0;
}


ExpWnd::~ExpWnd()
{
}

void ExpWnd::init(IModule * module)
{
	//size info
	const int mainwnd_w = 1000, mainwnd_h = 650;

	//static message objects;
	static MsgSet mainWndMsgs;
	static MsgNotify notify(this);
	static MsgCommand command(this);
	static MsgProc<ExpWnd> msg_sizing(this, &ExpWnd::updateLayout);
	mainWndMsgs.addMsgPair(WM_DESTROY, &autownd::msg_quit);
	mainWndMsgs.addMsgPair(WM_NOTIFY, &notify);
	mainWndMsgs.addMsgPair(WM_COMMAND, &command);
	mainWndMsgs.addMsgPair(WM_SIZE, &msg_sizing);

	theData = module;

	theMainWnd.setMsgSet(&mainWndMsgs);

	//init main wnd
	Seed mainSeed;
	mainSeed.init({});
	mainSeed.create(&theMainWnd, {
		{ "size",vec(mainwnd_w,mainwnd_h) }
	});

	//add left panel
	theMainWnd.addControl(&theLeftPanel.obj, WC_LISTVIEW, {
		{ "size",vec(mainwnd_w / 2,mainwnd_h - 50) },
		{ "style",(long)LVS_REPORT | LVS_EDITLABELS | LVS_SHOWSELALWAYS | LVS_SINGLESEL | LVS_NOCOLUMNHEADER | WS_EX_CLIENTEDGE },
		{ "exstyle", WS_EX_CLIENTEDGE },
		{ "pos",vec(20,50) }
	});
	theLeftPanel.obj.addColumn(0).set(L"Items",6).set(mainwnd_w / 2).update();
	theLeftPanel.obj.buildImageList(16, 16);
	theLeftPanel.obj.addIcon(103);
	theLeftPanel.obj.addIcon(105);

	//theLeftPanel.obj.extendStyle(LVS_EX_HEADERINALLVIEWS);//LVS_EX_GRIDLINES
	theLeftPanel.obj.show();

	//add right panel
	theMainWnd.addControl(&theRightPanel.obj, WC_LISTVIEW, {
		{ "size",vec(mainwnd_w / 2,mainwnd_h - 50) },
		{ "style",(long)LVS_REPORT | LVS_SHOWSELALWAYS | LVS_SINGLESEL | LVS_NOCOLUMNHEADER  },
		{ "exstyle", WS_EX_CLIENTEDGE },
		{ "pos",vec(mainwnd_w / 2 + 20,50) }
	});
	theRightPanel.obj.addColumn(0).set(L"Key",4).set(120).update();
	theRightPanel.obj.addColumn(1).set(L"Value", 4).set(175).update();

	theRightPanel.obj.extendStyle(LVS_EX_FULLROWSELECT| LVS_EX_GRIDLINES);//LVS_EX_GRIDLINES
	theRightPanel.obj.show();

	//add up button
	theMainWnd.addControl(&theButton.obj, WC_BUTTON, {
		{ "size",vec(mainwnd_w / 2,20) },
		{ "pos",vec(20,20) },
	});
	theButton.obj.show();

	//set edit callback
	theEdit.obj.setRecv(&command);

	//finished
	updateLayout(&theMainWnd,0,0);
	theMainWnd.show();
	updateItemlist();
}

void ExpWnd::updateItemlist(LPARAM param)
{
	theLeftPanel.param = param;

	BulletChain chain(3);
	chain.first()->fill("items");
	chain.at()->fill(param);
	theData->pull(&chain);

	//the up button
	chain.first(); chain.at(); chain.at();
	if (chain.at()->data<int>()) return;
	
	chain.line();
	SetWindowText(theButton.obj.wnd(), chain.at()->data<TCHAR>());
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
	updateAttlist();
}

void ExpWnd::updateAttlist(LPARAM param)
{
	theRightPanel.obj.clear();
	if (param == 0) return;
	BulletChain chain(2);
	chain.first()->fill("read");
	chain.at()->fill(param);
	theRightPanel.param = param;
	theData->pull(&chain);

	chain.first();
	while (chain.line())
	{
		Bullet *key = chain.at();
		Bullet *value = chain.at();

		autownd::List::LSet set = theRightPanel.obj.at();
		set.setText(key->data<TCHAR>(), key->size() / sizeof(TCHAR)).update();
		set.setText(1, value->data<TCHAR>());
	}
	theRightPanel.obj.at().update();
}

void ExpWnd::beNotified(LPNMHDR data)
{
	if (data->hwndFrom==theLeftPanel.obj.wnd())//left panel
	{
		LPNMITEMACTIVATE temp = (LPNMITEMACTIVATE)data;
		if (data->code == LVN_ITEMCHANGED)//on change
		{
			if(temp->uNewState) updateAttlist(temp->lParam);
		}
		if (data->code == NM_DBLCLK) //on double click
		{
			if (temp->iItem == -1) return;
			updateItemlist(theLeftPanel.obj.at(temp->iItem).setParam(0).sync()->lParam);
		}
		if (data->code == LVN_ENDLABELEDIT)
		{
			NMLVDISPINFO* info = (NMLVDISPINFO*)data;
			if (info->item.pszText == nullptr) return;
			theData->push({ { "setstr",info->item.pszText },{ "item",info->item.lParam } });
			ListView_SetItemText(theLeftPanel.obj.wnd(), info->item.iItem, info->item.iSubItem, info->item.pszText);
		}
	}

	if (data->hwndFrom == theRightPanel.obj.wnd())//right panel
	{
		LPNMITEMACTIVATE temp = (LPNMITEMACTIVATE)data;
		if (data->code == NM_CLICK)
		{
			TCHAR buff[255]; RECT rect;
			if (temp->iSubItem == -1) return;
			if (temp->iItem == -1) {
				temp->iItem = theRightPanel.obj.getCount() - 1;
				temp->iSubItem = 0;
			}
			List::LSet set= theRightPanel.obj.at(temp->iItem);

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
				{"parent",theRightPanel.obj.wnd()},
				{"rect",&rect},
				{"buff",&theEdit.str[temp->iSubItem][0]}
			});
		}
	}
}

void ExpWnd::clickButton(HWND wnd, int msg)
{
	if (wnd == theButton.obj.wnd())
	{
		updateItemlist(theButton.param);
	}
}

void ExpWnd::setAttribute(TCHAR * value)
{
	if (theEdit.subitem)
	{
		theData->push({
			{ "setkey",theEdit.str[0].c_str() },
			{ "key",theEdit.str[0].c_str() },
			{ "value",value },
			{ "item",theRightPanel.param }
		});
	}
	else
	{
		theData->push({
			{ "setkey",theEdit.str[0].c_str() },
			{ "key",value },
			{ "value",theEdit.str[1].c_str() },
			{ "item",theRightPanel.param }
		});
	}

	updateAttlist(theRightPanel.param);
	theEdit.str[0].clear(); theEdit.str[1].clear();
}

int ExpWnd::updateLayout(WndObj *obj, WPARAM wp, LPARAM lp)
{
	RECT rect;
	GetClientRect(theMainWnd.wnd(), &rect);
	OffsetRect(&rect, GetSystemMetrics(SM_CXBORDER), GetSystemMetrics(SM_CXBORDER));

	const int edge_blank = 10;
	const int client_w = rect.right - rect.left - 2 * edge_blank, client_h = rect.bottom - rect.top - 2 * edge_blank;
	const int button_h = 30;
	const int panel_w = client_w / 2 - edge_blank / 2;

	MoveWindow(theLeftPanel.obj.wnd(),
		edge_blank, edge_blank * 2 + button_h, panel_w, client_h - edge_blank - button_h, TRUE);
	theLeftPanel.obj.resizeColumn(0, client_w / 2 - 3 * edge_blank);

	MoveWindow(theRightPanel.obj.wnd(),
		edge_blank + client_w / 2, edge_blank * 2 + button_h, panel_w, client_h - edge_blank - button_h, TRUE);
	theRightPanel.obj.resizeColumn(0, panel_w / 2 - 4);
	theRightPanel.obj.resizeColumn(1, panel_w / 2);

	MoveWindow(theButton.obj.wnd(),
		edge_blank, edge_blank, panel_w, button_h, TRUE);

	return 1;
}
