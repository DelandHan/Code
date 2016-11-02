#include "stdafx.h"
#include "DisplayObj.h"
#include "WndBoard.h"
#include "resource.h"

using namespace autownd;
using namespace std;


LayoutManager::LayoutManager(int type)
	:theType(type)
{
}

LayoutManager::~LayoutManager()
{
}

void LayoutManager::addObj(IMoveableWndObj * obj, autownd::vec pos, autownd::vec size)
{
	theList.push_back({ obj, pos, size });
}

void LayoutManager::move(RECT * rect)
{
	switch (theType)
	{
	case 0:
	{
		moveAutoObj(rect);
	}
	break;
	case 1:
	{
		moveFixedObj(rect);
	}
	break;
	default:
		break;
	}
}


void LayoutManager::moveAutoObj(RECT * rect)
{
	for (list<ObjDetail>::iterator it = theList.begin(); it != theList.end(); it++)
	{
		int width = rect->right - rect->left, height = rect->bottom - rect->top;

		it->obj->drag(rect->left + width * it->pos.first / 100, rect->top + height*it->pos.second / 100,
			width*it->size.first / 100, height*it->size.second / 100);
	}
}

void LayoutManager::moveFixedObj(RECT * rect)
{
	for (list<ObjDetail>::iterator it = theList.begin(); it != theList.end(); it++)
	{
		it->obj->drag(rect->left + it->pos.first, rect->top + it->pos.second,
			it->size.first, it->size.second);
	}
}

/////////////////////////////////////

WndBoard::WndBoard()
	:theLayoutMgr{ 0,0 }
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
		{ "title",L"XMLEditor" },
		{ "menu", IDR_MENU1 }
	})) return 1;
	else
		return 0;
}

int WndBoard::updateLayout(WPARAM wp, LPARAM lp)
{
	RECT rect;
	GetClientRect(theMainWnd.wnd(), &rect);
	OffsetRect(&rect, GetSystemMetrics(SM_CXBORDER), GetSystemMetrics(SM_CXBORDER));

	RECT fixedarea = { rect.left,rect.top,rect.right,rect.top + 30 };
	theLayoutMgr[0].move(&fixedarea);
	rect.top += 30;// the top 30 pix are for buttons
	theLayoutMgr[1].move(&rect);

	return 0;
}

int WndBoard::connectDisplayObj(IMoveableWndObj * obj, autownd::vec pos, autownd::vec size, int area)
{
	obj->initialize(&theMainWnd);
	if (area > 1) area = 1;
	theLayoutMgr[area].addObj(obj, pos, size);

	return 0;
}

void WndBoard::update()
{
	theMainWnd.show(SW_HIDE);
	theMainWnd.show();
}
