#include "stdafx.h"
#include "DisplayObj.h"
#include "WndBoard.h"

using namespace autownd;
using namespace std;

void LayoutManager::addAutoObj(IMoveableWndObj * obj, autownd::vec pos, autownd::vec size)
{
	theAutoObjList.push_back({ obj, pos, size });
}

void LayoutManager::addFixedObj(IMoveableWndObj * obj, autownd::vec pos, autownd::vec size)
{
	theFixedObjList.push_back({ obj, pos, size });
}

void LayoutManager::moveAutoObj(RECT * rect)
{
	for (list<ObjDetail>::iterator it = theAutoObjList.begin(); it != theAutoObjList.end(); it++)
	{
		int width = rect->right - rect->left, height = rect->bottom - rect->top;

		it->obj->drag(rect->left + width * it->pos.first / 100, rect->top + height*it->pos.second / 100,
			width*it->size.first / 100, height*it->size.second / 100);
	}
}

void LayoutManager::moveFixedObj(RECT * rect)
{
	for (list<ObjDetail>::iterator it = theFixedObjList.begin(); it != theFixedObjList.end(); it++)
	{
		it->obj->drag(rect->left + it->pos.first, rect->top + it->pos.second,
			it->size.first, it->size.second);
	}
}

/////////////////////////////////////

WndBoard::WndBoard()
{
	//init a base msgmap
	theMsgSet.addMsgPair(WM_DESTROY, &msg_quit);
	theMsgSet.addMsgProc(WM_SIZE, this, &WndBoard::updateLayout);
}

WndBoard::~WndBoard()
{
}

int WndBoard::initialize()
{
	//init main wnd
	Seed mainSeed;
	mainSeed.init({});

	if (mainSeed.create(&theMainWnd, &theMsgSet, {
		{ "size",vec(1000,650) },
		{ "title",L"XMLEditor" }
	})) return 1;
	else
		return 0;
}

int WndBoard::updateLayout(WPARAM wp, LPARAM lp)
{
	RECT rect;
	GetClientRect(theMainWnd.wnd(), &rect);
	OffsetRect(&rect, GetSystemMetrics(SM_CXBORDER), GetSystemMetrics(SM_CXBORDER));

	theLayoutMgr.moveFixedObj(&rect);
	rect.top += 30;// the top 30 pix are for buttons
	theLayoutMgr.moveAutoObj(&rect);

	return 0;
}

int WndBoard::connectDisplayObj(IMoveableWndObj * obj, autownd::vec pos, autownd::vec size, int area)
{
	obj->initialize(&theMainWnd);
	if(area) theLayoutMgr.addAutoObj(obj, pos, size);
	else theLayoutMgr.addFixedObj(obj, pos, size);

	return 0;
}

void WndBoard::update()
{
	theMainWnd.show(SW_HIDE);
	theMainWnd.show();
}
