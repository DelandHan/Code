#include "stdafx.h"
#include "DisplayObj.h"
#include "resource.h"

using namespace std;
using namespace autownd;

int ItemPanel::initialize(autownd::WndObj * parent)
{
	//create the control
	if (parent->addControl(&theObj, WC_LISTVIEW, {
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

void ItemPanel::drag(int x, int y, int width, int height)
{
	MoveWindow(theObj.wnd(), x, y, width, height, TRUE);
	theObj.resizeColumn(0, width - 4);
}

void ItemPanel::addItems(wstring & str, int image, LPARAM param)
{
	theObj.at().setText(&str[0], str.size()).setImage(image).setParam(param).update();
}

void ItemPanel::clear()
{
	theObj.clear();
}

void ItemPanel::setItemText(int item, int subitem, TCHAR * str)
{
	ListView_SetItemText(theObj.wnd(), item, subitem, str);
}

LPARAM ItemPanel::getParam(int item)
{
	return theObj.at(item).setParam(0).sync()->lParam;
}

/////////////////////////////////////////////////////////////////////////

int AttPanel::initialize(autownd::WndObj * parent)
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

void AttPanel::drag(int x, int y, int width, int height)
{
	MoveWindow(theObj.wnd(), x, y, width, height, TRUE);
	theObj.resizeColumn(0, (width - 4) / 2);
	theObj.resizeColumn(1, (width - 4) / 2);
}

void AttPanel::clear()
{
	theObj.clear();
}

void AttPanel::addAttribute(wstring & key, wstring & value)
{
	List::LSet set = theObj.at();
	set.setText(&key[0], key.size()).update();
	set.setText(1, &value[0]);
}

/////////////////////////////////////////////////////////////////////////

int DisplayButton::initialize(autownd::WndObj * parent)
{
	if (parent->addControl(&theObj, WC_BUTTON, { { "title", L"Up" } })) return 1;
	theObj.show();
	return 0;
}

void DisplayButton::drag(int x, int y, int width, int height)
{
	MoveWindow(theObj.wnd(), x, y, width, height, TRUE);
}


