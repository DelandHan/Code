#include "stdafx.h"
#include "CenterUnit.h"

using namespace std;

BaseCenterUnit::BaseCenterUnit()
	:theSelected(0), theUI(nullptr)
{
	theCurrent[0] = 0;
	theCurrent[1] = 0;
}

BaseCenterUnit::~BaseCenterUnit()
{
}

int BaseCenterUnit::connect(IUIHub * ui, IDataHub* data)
{
	theUI = ui;
	theData = data;
	
	setCurrent(0, 0);
	setCurrent(0, 1);
	setSelection(0);

	return ui == nullptr || data == nullptr ? 1 : 0;
}

int BaseCenterUnit::refreshCurrent()
{
	ItemPool pool;

	if (theData->getChildItemData(theCurrent[0], &pool) == 0)
	{
		theUI->refreshItemPanel(&pool, 0);
		refreshChild();
		refreshAtt();
		return 0;
	}
	else
		return 1;

}

int BaseCenterUnit::refreshChild()
{
	ItemPool itempool;
	theData->getChildItemData(theCurrent[1], &itempool);
	theUI->refreshItemPanel(&itempool, 1);
	return 0;
}

int BaseCenterUnit::refreshAtt()
{
	AttPool attpool;
	theData->getItemAtt(theSelected, &attpool);
	theUI->refreshAttPanel(&attpool);

	return 0;
}

void BaseCenterUnit::setCurrent(LPARAM param, int i)
{
	if (param == 0 && i == 0)
	{
		ItemData data = { L"",0,0 };
		theData->queryItem(&data);
		theCurrent[i] = data.param;
	}
	else
		theCurrent[i] = param;
	if (i == 0) theCurrent[1] = 0;
}

void BaseCenterUnit::setSelection(LPARAM param)
{
	theSelected = param;
}

//////////////////////////////////////////////////////

CenterUnit::CenterUnit()
{
}

CenterUnit::~CenterUnit()
{
}


int CenterUnit::goHighLevel()
{
	setCurrent(datapool()->queryParent(current(0)),0);
	setCurrent(0, 1);
	setSelection(0);
	return refreshCurrent();
}

int CenterUnit::select(LPARAM param, int panelId)
{
	setSelection(param);

	if (panelId) {
		refreshAtt();
	}
	else {
		setCurrent(param, 1);
		refreshChild();
		refreshAtt();
	}
	return 0;
}

int CenterUnit::dbClick(LPARAM param)
{
	setCurrent(param, 0);
	setCurrent(0, 1);
	setSelection(0);
	return refreshCurrent();
}

int CenterUnit::edit(LPARAM param, std::wstring &str)
{
	ItemData data = { str,0,param };

	if (datapool()->setItem(&data) == 0)
	{
		datapool()->queryItem(&data);
		str = data.str;
		return 0;
	}
	return 1;
}

int CenterUnit::updateAtt(std::wstring * oldkey, std::wstring * value, std::wstring * nekey)
{
	if (*oldkey == L"")
		datapool()->setItemAtt(selection(), nekey, value, nekey);
	else
		datapool()->setItemAtt(selection(), oldkey, value, nekey);
	
	refreshAtt();
	return 0;
}

