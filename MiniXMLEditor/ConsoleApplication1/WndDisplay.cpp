#include "stdafx.h"
#include "WndDisplay.h"
#include "resource.h"

using namespace autownd;

WndDisplay::WndDisplay()
{

}

WndDisplay::~WndDisplay()
{
	//clear the list
	for (ObjectList::iterator it = theObjLst.begin(); it != theObjLst.end(); it++)
	{
		delete *it;
	}
}

int WndDisplay::initialize()
{
	//build main wnd
	if (buildMainWnd()) return 1;

	//add panels
	addObj(new ItemPanel, { 50,95 }, { 0,5 });
	addObj(new ItemPanel, { 50,50 }, { 50,5 });
	addObj(new AttPanel, { 50,45 }, { 50,55 });
	addObj(new DisplayButton, { 5,5 }, { 0,0 });

	//final
	theMainWnd.show();
	return 0;
}

MsgSet * WndDisplay::getMap()
{
	return &theMsgMap;
}

int WndDisplay::updateLayout(memory::ParamChain params)
{
	RECT rect;
	GetClientRect(theMainWnd.wnd(), &rect);
	OffsetRect(&rect, GetSystemMetrics(SM_CXBORDER), GetSystemMetrics(SM_CXBORDER));

	for (auto it : theObjLst) {
		it->move(&rect);
	}
	return 0;
}

void WndDisplay::addObj(DisplayObj * obj, autownd::vec size, autownd::vec pos)
{
	obj->setRect(size, pos);
	obj->initialize(&theMainWnd);
	theObjLst.push_back(obj);
}

int WndDisplay::buildMainWnd()
{
	//init a base msgmap
	static MsgProc<WndDisplay> msg_size(this, &WndDisplay::updateLayout);
	theMsgMap.addMsgPair(WM_DESTROY, &msg_quit);
	theMsgMap.addMsgPair(WM_SIZE, &msg_size);

	//init main wnd
	Seed mainSeed;
	mainSeed.init({});

	if(mainSeed.create(&theMainWnd, &theMsgMap, {
		{ "size",vec(1000,650) },
		{ "title",L"XMLEditor" }
	})) return 1;
	else
		return 0;
}

//////////////////Panel classes//////////////////

void WndDisplay::DisplayObj::setRect(autownd::vec & size, autownd::vec & pos)
{
		theSize = size; thePos = pos;
}

void WndDisplay::DisplayObj::move(const RECT * clientRect)
{
	int width = clientRect->right - clientRect->left, height = clientRect->bottom - clientRect->top;

	MoveWindow(wnd(),
		clientRect->left + width * thePos.first / 100, clientRect->top + height*thePos.second / 100,
		width*theSize.first / 100, height*theSize.second / 100,
		TRUE
		);

	onSize(width*theSize.first / 100, height*theSize.second / 100);
}

int WndDisplay::ItemPanel::initialize(autownd::WndObj * parent)
{
	//create the control
	if(parent->addControl(&theObj, WC_LISTVIEW, {
		{ "style", (long)LVS_EDITLABELS | LVS_REPORT | LVS_SHOWSELALWAYS | LVS_SINGLESEL | LVS_NOCOLUMNHEADER },
		{ "exstyle", WS_EX_CLIENTEDGE },
	}))
		return 1;

	//add columns
	theObj.addColumn(0).set(L"Column", 6).set(-2).update();

	//add icons
	theObj.buildImageList(16, 16);
	theObj.addIcon(IDI_FILE);
	theObj.addIcon(IDI_FOLDER);

	//show
	theObj.show();

	return 0;
}

int WndDisplay::ItemPanel::onSize(int width, int height)
{
	theObj.resizeColumn(0, width - 4);
	return 1;
}

int WndDisplay::AttPanel::initialize(autownd::WndObj * parent)
{
	//create the control
	if (parent->addControl(&theObj, WC_LISTVIEW, {
		{ "style", (long)LVS_REPORT | LVS_SHOWSELALWAYS | LVS_SINGLESEL | LVS_NOCOLUMNHEADER },
		{ "exstyle", WS_EX_CLIENTEDGE },
	}))
		return 1;

	//add columns
	theObj.addColumn(0).set(L"Column", 6).set(-2).update();
	theObj.addColumn(1).set(L"Column", 6).set(-2).update();

	theObj.extendStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	//show
	theObj.show();
	
	return 0;
}

int WndDisplay::AttPanel::onSize(int width, int height)
{
	theObj.resizeColumn(0, (width - 4) / 2);
	theObj.resizeColumn(1, (width - 4) / 2);
	return 1;
}

int WndDisplay::DisplayButton::initialize(autownd::WndObj * parent)
{
	if(	parent->addControl(&theObj, WC_BUTTON, {{ "title", L"Up" }})) return 1;
	theObj.show();

	return 0;
}

int WndDisplay::DisplayButton::onSize(int width, int height)
{
	return 0;
}
