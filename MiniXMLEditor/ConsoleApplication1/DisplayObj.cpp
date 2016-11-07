#include "stdafx.h"
#include "DisplayObj.h"
#include "resource.h"

using namespace std;
using namespace autownd;

int ItemPanel::initialize(autownd::WndObj * parent)
{
	//create the control
	if (parent->addControl(this, WC_LISTVIEW, {
		{ "style", (long)LVS_EDITLABELS | LVS_REPORT | LVS_SHOWSELALWAYS | LVS_NOCOLUMNHEADER }, //LVS_SINGLESEL
		{ "exstyle", WS_EX_CLIENTEDGE },
	}))
	return 1;

	//add columns
	addColumn(0).set(L"Column", 6).set(-2).update();

	//add icons
	buildImageList(16, 16);
	addIcon(IDI_FILE);
	addIcon(IDI_FOLDER);

	//show
	show();

	return 0;
}

void ItemPanel::drag(int x, int y, int width, int height)
{
	MoveWindow(wnd(), x, y, width, height, TRUE);
	resizeColumn(0, width - 4);
}

void ItemPanel::addItems(wstring & str, int image, LPARAM param)
{
	at().setText(&str[0], str.size()).setImage(image).setParam(param).update();
}

void ItemPanel::setItemText(int item, int subitem, TCHAR * str)
{
	ListView_SetItemText(wnd(), item, subitem, str);
}

LPARAM ItemPanel::getParam(int item)
{
	return at(item).setParam(0).sync()->lParam;
}

/////////////////////////////////////////////////////////////////////////

AttPanel::AttPanel()
	:theOnEditing(-1), theCallBack(this,&AttPanel::finishEditing), theParentCallBack(nullptr)
{
}

int AttPanel::initialize(autownd::WndObj * parent)
{
	//create the control
	if (parent->addControl(this, WC_LISTVIEW, {
		{ "style", (long)LVS_REPORT | LVS_SHOWSELALWAYS |  LVS_NOCOLUMNHEADER },
		{ "exstyle", WS_EX_CLIENTEDGE },
	}))
	return 1;

	//add columns
	addColumn(0).set(L"Column", 6).set(-2).update();
	addColumn(1).set(L"Column", 6).set(-2).update();

	extendStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	//show
	show();

	return 0;
}

void AttPanel::drag(int x, int y, int width, int height)
{
	MoveWindow(wnd(), x, y, width, height, TRUE);
	resizeColumn(0, (width - 4) / 2);
	resizeColumn(1, (width - 4) / 2);
}

void AttPanel::startEditing(int item, int subitem, autownd::IMsgProcess *proc)
{
	TCHAR temp[255]; RECT rect;
	if (subitem == -1 || item == -1) return;

	List::LSet set = at(item);

	set.getText(temp, 255, 0); theBuff[0]= temp;
	set.getText(temp, 255, 1); theBuff[1] = temp;
	theOnEditing = subitem;

	set.getRect(&rect, subitem);
	//adjust pos
	if (subitem == 0) {
		rect.left += 3;
		rect.right = rect.left + (rect.right - rect.left) / 2 - 4;
	}
	else rect.left += 5;
	rect.bottom -= 1;
		
	theEdit.init(wnd(), &rect, &theBuff[subitem][0], &theCallBack);
	theParentCallBack = proc;
}

void AttPanel::addAttribute(wstring & key, wstring & value)
{
	List::LSet set = at();
	set.setText(&key[0], key.size()).update();
	set.setText(1, &value[0]);
}

std::wstring * AttPanel::getEditResult()
{
	if (theOnEditing == -1) return nullptr;
	theOnEditing = -1;
	return theBuff;
}

int AttPanel::finishEditing(WPARAM wp, LPARAM lp)
{
	TCHAR *data = (TCHAR*)wp;
	if (theOnEditing == 0) theBuff[2] = data;
	if (theOnEditing == 1) theBuff[1] = data;

	theParentCallBack->handleMsg(0, (LPARAM)wnd());

	return 0;
}

/////////////////////////////////////////////////////////////////////////

int DisplayButton::initialize(autownd::WndObj * parent)
{
	if (parent->addControl(this, WC_BUTTON, { { "title", L"Up" } })) return 1;
	show();
	return 0;
}

void DisplayButton::drag(int x, int y, int width, int height)
{
	MoveWindow(wnd(), x, y, width, height, TRUE);
}

DisplayCmdBox::DisplayCmdBox()
	:theBoxRect{0,0,0,0}, theParent(nullptr)
{
}

DisplayCmdBox::~DisplayCmdBox()
{
}

int DisplayCmdBox::initialize(autownd::WndObj * parent)
{
	theParent = parent->wnd();
	return 0;
}

void DisplayCmdBox::drag(int x, int y, int width, int height)
{
	theBoxRect.left = x;
	theBoxRect.top = y;
	theBoxRect.right = x + width;
	theBoxRect.bottom = y + height;
}

void DisplayCmdBox::startEditing(autownd::IMsgProcess * proc)
{
	theEdit.init(theParent, &theBoxRect, L"", proc);
}
