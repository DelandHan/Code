#include "stdafx.h"
#include "DisplayObj.h"
#include "WndDisplay.h"

using namespace autownd;
using namespace std;

void WndDisplay::convertToWStr(std::wstring &dest, const std::string source) {
	dest.resize(source.size());
	std::copy(source.begin(), source.end(), dest.begin());
}

void WndDisplay::LayoutManager::addAutoObj(IMoveableWndObj * obj, autownd::vec pos, autownd::vec size)
{
	theAutoObjList.push_back({ obj, pos, size });
}

void WndDisplay::LayoutManager::addFixedObj(IMoveableWndObj * obj, autownd::vec pos, autownd::vec size)
{
	theFixedObjList.push_back({ obj, pos, size });
}

void WndDisplay::LayoutManager::moveAutoObj(RECT * rect)
{
	for (list<ObjDetail>::iterator it = theAutoObjList.begin(); it != theAutoObjList.end(); it++)
	{
		int width = rect->right - rect->left, height = rect->bottom - rect->top;

		it->obj->drag(rect->left + width * it->pos.first / 100, rect->top + height*it->pos.second / 100,
			width*it->size.first / 100, height*it->size.second / 100);
	}
}

void WndDisplay::LayoutManager::moveFixedObj(RECT * rect)
{
	for (list<ObjDetail>::iterator it = theFixedObjList.begin(); it != theFixedObjList.end(); it++)
	{
		it->obj->drag(rect->left + it->pos.first, rect->top + it->pos.second,
			it->size.first, it->size.second);
	}
}

/////////////////////////////////////

WndDisplay::WndDisplay()
{
	//init a base msgmap
	theMsgSet.addMsgProc(WM_DESTROY, &msg_quit);
	theMsgSet.addMsgProc(WM_SIZE, this, &WndDisplay::updateLayout);

}

WndDisplay::~WndDisplay()
{
}

int WndDisplay::initialize()
{
	//build main wnd
	if (buildMainWnd()) return 1;

	//init objs
	thePrimaryPanel.initialize(&theMainWnd);
	theLayoutMgr.addAutoObj(&thePrimaryPanel, { 0,0 }, { 50,100 });

	theSecondaryPanel.initialize(&theMainWnd);
	theLayoutMgr.addAutoObj(&theSecondaryPanel, { 50,0 }, { 50,50 });

	theAttPanel.initialize(&theMainWnd);
	theLayoutMgr.addAutoObj(&theAttPanel, { 50,50 }, { 50,50 });

	theUpButton.initialize(&theMainWnd);
	theLayoutMgr.addFixedObj(&theUpButton, { 5,5 }, { 20,20 });

	//final
	theMainWnd.show();
	return 0;
}

void WndDisplay::addToPrimaryList(const char * str, int type, LPARAM param)
{
	wstring buff;
	convertToWStr(buff, str);
	thePrimaryPanel.addItems(buff, type, param);
}

void WndDisplay::addToPrimaryList(TCHAR * str, int type, LPARAM param)
{
	thePrimaryPanel.addItems(wstring(str), type, param);
}

void WndDisplay::addToSecondaryList(const char * str, int type, LPARAM param)
{
	wstring buff;
	convertToWStr(buff, str);
	theSecondaryPanel.addItems(buff, type, param);
}

void WndDisplay::addToAttributeList(const char * key, const char * value)
{
	wstring buff_k, buff_v;
	convertToWStr(buff_k, key);
	convertToWStr(buff_v, value);

	theAttPanel.addAttribute(buff_k, buff_v);
}

int WndDisplay::updateLayout(WPARAM wp, LPARAM lp)
{
	RECT rect;
	GetClientRect(theMainWnd.wnd(), &rect);
	OffsetRect(&rect, GetSystemMetrics(SM_CXBORDER), GetSystemMetrics(SM_CXBORDER));

	theLayoutMgr.moveFixedObj(&rect);
	rect.top += 30;// the top 30 pix are for buttons
	theLayoutMgr.moveAutoObj(&rect);

	return 0;
}

int WndDisplay::buildMainWnd()
{
	//init main wnd
	Seed mainSeed;
	mainSeed.init({});

	if(mainSeed.create(&theMainWnd, theMsgSet.map(), {
		{ "size",vec(1000,650) },
		{ "title",L"XMLEditor" }
	})) return 1;
	else
		return 0;
}

//////////////////messages//////////////////

WndMsgSet::~WndMsgSet()
{
	for (auto it : theList) delete it;
}

void WndMsgSet::addMsgProc(UINT msg, autownd::IMsgProcess * msgproc)
{
	theMsgMap.addMsgPair(msg, msgproc);
}

